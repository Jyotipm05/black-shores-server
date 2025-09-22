//
// Created by jmbab on 22-09-2025.
//
#include <Drogon/Drogon.h>
#include <iostream>
#include <string>

using namespace drogon;
using namespace std;

int main() {
    string ipAddress;
    int port = 0;
    getline(cin, ipAddress);
    cin >> port;
    if (port <= 0 || port > 65535) {
        cerr << "Error: Invalid port number. Must be between 1 and 65535." << endl;
        return 1;
    }
    const char* listenAddr = ipAddress != "" ? ipAddress.c_str() : "localhost";
    app().addListener(listenAddr, port);
    cout << "listener added on " << listenAddr << ":" << port << endl;
    app().run();
    return 0;
}
