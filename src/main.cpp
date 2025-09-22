//
// Created by jmbab on 22-09-2025.
//
#include <Drogon/Drogon.h>
#include <iostream>
#include <string>
#include "IpCollector.h"

using namespace drogon;
using namespace std;

int main() {
    SetConsoleOutputCP(CP_UTF8);
    cout << "Do you want to host it on local network🛜[1] or host machine only💻[2] or custom🛠️[3]?" << endl;
    cout << "Type 1, 2, or 3 and press enter (default is 2): ";
    string temp, ipAddress = "";
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
    getline(cin, temp="");
    int port = temp == "" ? 5555 : stoi(temp);
    if (port < 0 || port > 65535) port = 5555;
    app().addListener(ipAddress.c_str(), port);
    cout << "listener added on http://" << ipAddress << ":" << port << endl;
    app().run();
    return 0;
}
