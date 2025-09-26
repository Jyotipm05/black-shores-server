//
// Created by jmbab on 22-09-2025.
//
#include <Drogon/Drogon.h>
#include <iostream>
#include <string>
#include <filesystem>
// #include <cstdlib>
#include "IpCollector.h"
#include "dotenv.hpp"
#include "create_cert.hpp"

using namespace drogon;
using namespace std;
namespace fs = std::filesystem;

int main() {
    //initial setup
    SetConsoleOutputCP(CP_UTF8);
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

    //iii) *** port ***
    cout << "Enter port number (default is 5555): ";
    getline(cin, temp);
    int port = temp.empty() ? 5555 : stoi(temp);
    if (port < 0 || port > 65535) port = 5555; //port range check

    //iv) *** SSL ***
    bool useSSL = false;
    cout << "Do you want to enable SSL? (y/n, default is n): ";
    getline(cin, temp);
    if (temp == "y" || temp == "Y") useSSL = true;
    else useSSL = false;


    //2)setup server------------------------------------------
    //i) *** Checking all files and directories ***
    string crt = dotenv::getenv("CRT_PATH", "../config/cert.csr");
    string key = dotenv::getenv("KEY_PATH", "../config/cert-key.pem");
    string doc_root = dotenv::getenv("ROOT_DIR", "../webapp/root");
    for (const auto &i: initializer_list<string>{crt, key, doc_root}) {
        if ((useSSL && (i == crt || i == key)) && !fs::exists(i)) {
            cerr << "Error: File or directory not found - " << i << endl;
        }
    }
    //ii) *** setting up server ***
    if (useSSL) {
        CertCreator::getInstance().create_cert(ipAddress, port);
        app().addListener(ipAddress.c_str(), port, true, crt, key);
    } else {
        app().addListener(ipAddress.c_str(), port);
    }
    app().setDocumentRoot(doc_root);
    app().setLogPath("../logs");
    app().setLogLevel(trantor::Logger::LogLevel::kWarn);
    cout << "listener added on " << (useSSL ? "https://" : "http://") << ipAddress << ":" << port << endl;
    app().run();
    return 0;
}
