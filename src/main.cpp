//
// Created by jmbab on 22-09-2025.
//
#include <Drogon/Drogon.h>
#include <iostream>
#include <string>
#include <filesystem>
#include <cstdlib>
#include "IpCollector.h"
#include "dotenv.hpp"

using namespace drogon;
using namespace std;
namespace fs = std::filesystem;

int main() {
    SetConsoleOutputCP(CP_UTF8);
    dotenv::init("../.env");
    cout << "Do you want to host it on local network🛜[1] or host machine only💻[2] or custom🛠️[3]?" << endl;
    cout << "Type 1, 2, or 3 and press enter (default is 2): ";
    string temp;
    string ipAddress;
    getline(cin, temp);
    int option = 2; // default
    try {
        if (!temp.empty()) option = stoi(temp);
    } catch (...) {
        option = 2;
    }
    switch (option) {
        case 1: {
            IpCollector ipCollector;
            try {
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
            cout << "Enter custom IP address: ";
            getline(cin, ipAddress);
            if (ipAddress.empty()) ipAddress = "localhost";
            cout << "Hosting on custom IP: " << ipAddress << endl;
            break;
        }
        case 2:
        default: {
            ipAddress = "localhost";
            cout << "Hosting on localhost only." << endl;
            break;
        }
    }
    cout << "Enter port number (default is 5555): ";
    getline(cin, temp);
    int port = temp.empty() ? 5555 : stoi(temp);
    if (port < 0 || port > 65535) port = 5555;

    string configPath = "../config/config.json";
    cout << fs::current_path() << endl;

    string crt = dotenv::getenv("CRT_PATH", "../config/localhost.crt");
    string key = dotenv::getenv("KEY_PATH", "../config/localhost.key");
    string doc_root = dotenv::getenv("ROOT_DIR", "../webapp/root");
    for (const auto& i : initializer_list<string>{crt, key, doc_root}) {
        if (!fs::exists(i)) {
            cerr << "Error: File or directory not found - " << i << endl;
        }
    }
    try {
        app().addListener(ipAddress.c_str(), port, true, crt, key);
    } catch (const std::exception& ex) {
        cerr << "Failed to add listener: " << ex.what() << endl;
        return 1;
    }
    app().setDocumentRoot(doc_root);
    cout << "listener added on http://" << ipAddress << ":" << port << endl;
    cout << "listener added on https://" << ipAddress << ":" << port << endl;
    // try {
    //     app().run(); // Must be called from main thread only
    // } catch (const std::exception& ex) {
    //     cerr << "Server run failed: " << ex.what() << endl;
    //     return 1;
    // }
    app().run();
    return 0;
}
