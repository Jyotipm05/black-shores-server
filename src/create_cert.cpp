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

void CertCreator::create_cert(string IPAdress, int port) {
    dotenv::init("../.env");
    handleCAKey(ca_key = dotenv::getenv("CA_KEY", "../config/ca-key.pem"));
    handleCACertificate(ca = dotenv::getenv("CA_CRT", "../config/ca.pem"));
    handleKey(key = dotenv::getenv("KEY_PATH", "../config/cert-key.pem"));
    handleCSR(csr = dotenv::getenv("CSR_PATH", "../config/cert.csr"));
    //generating certificate
    addEXTFile(IPAdress);
    // system(
    //     ("cd ../config; openssl x509 -req -sha256 -days 365 -in cert.csr -CA ca.pem -CAkey ca-key.pem -out cert.pem -extfile extfile.cnf -CAcreateserial -passin pass:\""
    //      + passphrase + "\"").c_str());
    system(
        ("openssl x509 -req -sha256 -days 365 -in ../config/cert.csr -CA ../config/ca.pem -CAkey ../config/ca-key.pem -out ../config/cert.pem -extfile ../config/extfile.cnf -CAcreateserial -passin pass:\""
         + passphrase + "\"").c_str());
}

void CertCreator::handleCACertificate(std::string ca) {
    if (fs::exists(ca)) {
        cout << Color::Green(ca + " exists.") << endl;
    } else {
        cout << Color::Red(ca) << Color::Yellow(" Creating one.") << endl;
#if defined(_WIN32) || defined(_WIN64) || defined(__linux__) || defined(__unix__) || defined(__unix) || defined(__APPLE__)
        system(
            ("cd ../config; openssl req -x509 -new -nodes -keyout ca-key.pem -sha256 -days 1024 -out ca.pem -subj \"/C=IN/ST=WB/L=KGP/O=Black_Shores/CN=Black_Shores\" -passin pass:"
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
                "cd ../config; openssl genrsa -aes256-out ca-key.pem 4096 -passout pass:\"" + passphrase + "\""
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
            ("cd ../config; openssl req -new -sha256 -subj \"/CN=Black Shores\" -key cert-key.pem -out cert.csr -passin pass:\""
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
        system("cd ../config; openssl genrsa -out cert-key.pem 4096");
#else
        cout << "Please create a CA certificate manually and place it at " << key << endl;
#endif
    }
}

void CertCreator::addEXTFile(std::string& IPAdress) {
    ofstream out("../config/extfile.cnf", ios::binary);
    out << "\xEF\xBB\xBF"; // UTF-8 BOM
    out << "subjectAltName=DNS:your-dns.record,IP:" << IPAdress << endl;
    out.close();
}
