#pragma once
#include <iostream>
#include <vector>
#include <string>
// #include <cstring>

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netdb.h>
    #include <arpa/inet.h>
    #include <unistd.h>
#endif

using std::string, std::vector;

enum IP {
    IPv4,
    IPv6,
    Invalid
};

struct IPAddress {
    std::string address;
    IP version; // "IPv4" or "IPv6"
};
/*
inline std::vector<IPAddress> getLocalIPs() {
    std::vector<IPAddress> foundAddresses;

#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) return foundAddresses;
#endif

    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) != 0) return foundAddresses;

    addrinfo hints{}, *res = nullptr;
    // memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(hostname, nullptr, &hints, &res) == 0) {
        for (struct addrinfo* p = res; p != nullptr; p = p->ai_next) {
            char ipstr[INET6_ADDRSTRLEN];
            void* addr;
            IP ver;

            if (p->ai_family == AF_INET) {
                addr = &(reinterpret_cast<struct sockaddr_in *>(p->ai_addr)->sin_addr);
                ver = IP::IPv4;
            } else if (p->ai_family == AF_INET6) {
                addr = &(reinterpret_cast<struct sockaddr_in6 *>(p->ai_addr)->sin6_addr);
                ver = IP::IPv6;
            } else continue;

            if (inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr))) {
                foundAddresses.push_back({std::string(ipstr), ver});
            }
        }
        freeaddrinfo(res);
    }

#ifdef _WIN32
    WSACleanup();
#endif
    return foundAddresses;
}
*/

// int main() {
//     std::vector<IPAddress> myIPs = getLocalIPs();
//
//     for (const auto& ip : myIPs) {
//         std::cout << "[" << ip.version << "] " << ip.address << std::endl;
//     }
//
//     return 0;
// }

inline vector<IPAddress> getLocalIPs() {
    vector<IPAddress> foundAddresses;
    foundAddresses.emplace_back("127.0.0.1", IPv4);
    foundAddresses.emplace_back("::1", IPv6);
    return foundAddresses;
}