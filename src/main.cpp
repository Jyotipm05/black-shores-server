//
// Created by jmbab on 22-09-2025.
//
#include <Drogon/drogon.h>
#include <iostream>
#include <string>

using namespace drogon;
using namespace std;

int main() {
    string ipAddress;
    int port = 0;
    getline(cin, ipAddress);
    cin >> port;
    app().addListener(ipAddress != "" ? ipAddress.c_str() : "localhost", port);
    cout<<"listener added on "<<ipAddress<<":"<<port<<endl;
    app().run();
    return 0;
}
