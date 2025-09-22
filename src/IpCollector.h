//
// Created by jmbab on 22-09-2025.
//

#ifndef BLACK_SHORES_SERVER_IPCOLLECTOR_H
#define BLACK_SHORES_SERVER_IPCOLLECTOR_H
#include <string>
#include <iostream>
#include <string>
#include <winsock2.h> // For Windows
#include <ws2tcpip.h> // For Windows
#pragma comment(lib, "ws2_32.lib") // Link with ws2_32.lib for Windows

// For Linux/macOS, use these headers:
// #include <unistd.h>
// #include <netdb.h>
// #include <arpa/inet.h>

class IpCollector {
public:
    std::string getIP() {
#ifdef _WIN32
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            throw std::runtime_error("WSAStartup failed");
        }
#endif

        char hostbuffer[256];
        char *IPbuffer;
        struct hostent *host_entry;
        int hostname_result;

        // Get the local hostname
        hostname_result = gethostname(hostbuffer, sizeof(hostbuffer));
        if (hostname_result == -1) {
            throw std::runtime_error("Error getting hostname");
#ifdef _WIN32
            WSACleanup();
#endif
        }
        // Get host information by hostname
        host_entry = gethostbyname(hostbuffer);
        if (host_entry == NULL) {
            throw std::runtime_error("Error getting host information");
#ifdef _WIN32
            WSACleanup();
#endif
        }
        // Convert the Internet network address to an ASCII string
        IPbuffer = inet_ntoa(*((struct in_addr *) host_entry->h_addr_list[0]));
        if (IPbuffer == NULL) {
            throw std::runtime_error("Error converting IP address to string");
#ifdef _WIN32
            WSACleanup();
#endif
        }

        // Store the IP address in a std::string
        std::string ipAddressString = IPbuffer;

        // std::cout << "Local IP Address: " << ipAddressString << std::endl;

        // Clean up Winsock for Windows
#ifdef _WIN32
        WSACleanup();
#endif
        return ipAddressString;
    }
};


#endif //BLACK_SHORES_SERVER_IPCOLLECTOR_H
