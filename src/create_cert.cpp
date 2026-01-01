#include <mutex>
#include <string>
#include <filesystem>
#include <iostream>
#include <cstdlib>
#include <utility>
#include <fstream>
// #include <TargetConditionals.h>
#include "create_cert.hpp"
#include "dotenv.hpp"

using namespace std;
namespace fs = std::filesystem;

// Define static members
std::string CertCreator::ca;
std::string CertCreator::ca_key;
std::string CertCreator::csr;
std::string CertCreator::key;
std::string CertCreator::passphrase;
std::string CertCreator::root;


namespace Color {
    static string Green(string text) {
        return "\033[32m" + text + "\033[0m";
    }

    static string Red(string text) {
        return "\033[31m" + text + "\033[0m";
    }

    static string Yellow(string text) {
        return "\033[33m" + text + "\033[0m";
    }
};

CertCreator::CertCreator(string pass) {
    passphrase = std::move(pass);
}

CertCreator &CertCreator::getInstance(string pass) {
    static CertCreator instance(std::move(pass));
    return instance;
}

void CertCreator::create_cert(string IPAddress, int port, const string &root) {
    CertCreator::root = root;
    dotenv::init((root + "/.env").c_str());
    handleCAKey(ca_key = root + "/" + dotenv::getenv("CA_KEY", "/config/ca-key.pem"));
    handleCACertificate(ca = root + "/" + dotenv::getenv("CA_CRT", "/config/ca.pem"));
    handleKey(key = root + "/" + dotenv::getenv("KEY_PATH", "/config/cert-key.pem"));
    handleCSR(csr = root + "/" + dotenv::getenv("CSR_PATH", "/config/cert.csr"));
    //generating certificate
    addEXTFile(IPAddress);
    // system(
    //     ("cd ../config; openssl x509 -req -sha256 -days 365 -in cert.csr -CA ca.pem -CAkey ca-key.pem -out cert.pem -extfile extfile.cnf -CAcreateserial -passin pass:\""
    //      + passphrase + "\"").c_str());
    system(
        ("cd /d \"" + root + "\" && cd config &&" +
         " openssl x509 -req -sha256 -days 365 -in cert.csr -CA ca.pem -CAkey ca-key.pem -out cert.pem -extfile extfile.cnf -CAcreateserial -passin pass:\""
         + passphrase + "\"").c_str());
}

void CertCreator::handleCACertificate(std::string ca) {
    if (fs::exists(ca)) {
        cout << Color::Green(ca + " exists.") << endl;
    } else {
        cout << Color::Red(ca) << Color::Yellow(" Creating one.") << endl;
#if defined(_WIN32) || defined(_WIN64) || defined(__linux__) || defined(__unix__) || defined(__unix) || defined(__APPLE__)
        string conf = root + "/config";
        system(
            ("cd /d " + conf +
             " && openssl req -x509 -new -nodes -keyout ca-key.pem -sha256 -days 1024 -out ca.pem -subj \"/C=IN/ST=WB/L=KGP/O=Black_Shores/CN=Black_Shores\" -passin pass:"
             + passphrase + "\"")
            .c_str()
        );
#else
        cout << "Please create a CA certificate manually and place it at " << ca << endl;
#endif
        cout << "Add " << ca << " to your browser's trusted root certificates to avoid security warnings." << endl;
    }
}

void CertCreator::handleCAKey(std::string ca_key) {
    if (fs::exists(ca_key)) {
        cout << ca_key << " exists." << endl;
    } else {
        cout << ca_key << " does not exist. Creating one." << endl;
#if defined(_WIN32) || defined(_WIN64) || defined(__linux__) || defined(__unix__) || defined(__unix) || defined(__APPLE__)
        system(
            (
                "cd /d \"" + root + "/config\" && openssl genrsa -aes256-out ca-key.pem 4096 -passout pass:\"" +
                passphrase +
                "\""
            ).c_str()
        );
#else
        cout << "Please create a CA certificate manually and place it at " << ca_key << endl;
#endif
    }
}

void CertCreator::handleCSR(std::string cert) {
    if (fs::exists(cert)) {
        cout << cert << " exists." << endl;
    } else {
        cout << cert << " does not exist. Creating one." << endl;
#if defined(_WIN32) || defined(_WIN64) || defined(__linux__) || defined(__unix__) || defined(__unix) || defined(__APPLE__)
        system(
            ("cd /d \"" + root +
             "/config\" && openssl req -new -sha256 -subj \"/CN=Black Shores\" -key cert-key.pem -out cert.csr -passin pass:\""
             + passphrase + "\"").c_str()
        );
#else
        cout << "Please create a CA certificate manually and place it at " << cert << endl;
#endif
    }
}

void CertCreator::handleKey(std::string key) {
    if (fs::exists(key)) {
        cout << key << " exists." << endl;
    } else {
        cout << key << " does not exist. Creating one." << endl;
#if defined(_WIN32) || defined(_WIN64) || defined(__linux__) || defined(__unix__) || defined(__unix) || defined(__APPLE__)
        system(("cd " + root + "/config && openssl genrsa -out cert-key.pem 4096").c_str());
#else
        cout << "Please create a CA certificate manually and place it at " << key << endl;
#endif
    }
}

void CertCreator::addEXTFile(std::string &IPAdress) {
    ofstream out(root + "/config/extfile.cnf", ios::binary);
    out << "\xEF\xBB\xBF"; // UTF-8 BOM
    out << "subjectAltName=DNS:your-dns.record,IP:" << IPAdress << endl;
    out.close();
}
