//
// Created by jmbab on 22-09-2025.
//
#include <Drogon/Drogon.h>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
// #include <cstdlib>
#include "IpCollector.hpp"
#include "dotenv.hpp"
#include "create_cert.hpp"
#include "pathFinder.hpp"
#include "controllers/PathManager.hpp"
// linux ubuntu or debian
#ifdef __linux__
#include <locale>
#endif


using namespace drogon;
using namespace std;
namespace fs = std::filesystem;

int main() {
    //initial setup
#if defined(_WIN32) || defined(_WIN64)
    SetConsoleOutputCP(CP_UTF8);
#elif defined(__linux__)
    std::locale::global(std::locale("C.UTF-8"));
#endif
    fs::path cp = fs::current_path();
    cout << "Current working directory: " << cp << endl;
    system("cd");
    dotenv::init("../.env");
    //input
    //1) ip address and port-------------------------------------------------------
    //i) *** IP address ***
    cout << "Do you want to host it on local network🛜[1] or host machine only💻[2] or custom🛠️[3]?" << endl;
    cout << "Type 1, 2, or 3 and press enter (default is 2): ";
    string temp, ipAddress;
    getline(cin, temp);
    int option = 2; // default
    try {
        if (!temp.empty()) option = stoi(temp);
    } catch (...) {
        option = 2;
    } //input validation

    // ii) *** fetch IP address ***
    switch (option) {
        case 1: {
            //local network
            try {
                IpCollector ipCollector;
                ipAddress = ipCollector.getIP();
                cout << "Hosting on local network. Current IP:" << ipAddress << endl;
            } catch (runtime_error &e) {
                cerr << "Error: " << e.what() << endl;
                cout << "Hosting on localhost only." << endl;
                ipAddress = "localhost";
            }
            break;
        }
        case 3: {
            //custom
            cout << "Enter custom IP address: ";
            getline(cin, ipAddress);
            if (ipAddress.empty()) ipAddress = "localhost";
            cout << "Hosting on custom IP: " << ipAddress << endl;
            break;
        }
        case 2:
        default: {
            //host machine only
            ipAddress = "localhost";
            cout << "Hosting on localhost only." << endl;
            break;
        }
    }

    //iv) *** SSL ***
    bool useSSL = false;
    cout << "Do you want to enable SSL? (y/n, default is n): ";
    getline(cin, temp);
    if (temp == "y" || temp == "Y") useSSL = true;
    else useSSL = false;

    //iii) *** port ***
    cout << "Enter port number (default is " << (useSSL ? 443 : 80) << "): ";
    getline(cin, temp);
    int port = temp.empty() ? (useSSL ? 443 : 80) : stoi(temp);
    if (port < 0 || port > 65535) port = (useSSL ? 443 : 80); //port range check


    //2)setup server------------------------------------------
    //i) *** Checking all files and directories ***
    fs::path root_dir = findParent(dotenv::getenv("CNF_DIR", "config"));
    string root = root_dir.string();
    PathManager::instance().setRootPath(root);
    string crt = root + "/" + dotenv::getenv("CRT_PATH", "config/cert.pem");
    string key = root + "/" + dotenv::getenv("KEY_PATH", "config/cert-key.pem");
    string doc_root = root + "/" + dotenv::getenv("ROOT_DIR", "webapp/root");
    for (const auto &i: initializer_list<string>{crt, key, doc_root}) {
        if ((useSSL && (i == crt || i == key)) && !fs::exists(i)) {
            cerr << "Error: File or directory not found - " << i << endl;
        }
    }
    //ii) *** setting up server ***
    doc_root = reSlash(doc_root);
    string log_root = root + "\\logs";
    log_root = reSlash(log_root);
    crt = reSlash(crt);
    key = reSlash(key);

    if (useSSL) {
        CertCreator::getInstance().create_cert(ipAddress, port, root);
        app().addListener(ipAddress, port, true, crt, key);
    } else {
        app().addListener(ipAddress, port);
    }

    app().setDocumentRoot(doc_root);
    app().setLogPath(log_root);
    app().setLogLevel(trantor::Logger::LogLevel::kFatal);
    string p_str = ((useSSL && port == 443) || (!useSSL && port == 80)) ? "" : (":" + to_string(port));
    cout << "listener added on " << (useSSL ? "https://" : "http://") << ipAddress << p_str << endl;
    cout << "WebSocket added on " << (useSSL ? "wss://" : "ws://") << ipAddress << p_str << "/chat" << endl;
    app().run();
    return 0;
}
