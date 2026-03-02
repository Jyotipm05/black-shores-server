module;
#include "../GetIPv4IPv6.hpp"
#include <Drogon/Drogon.h>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <variant>
export module Data_Manager;

import CLI;

using std::string, std::cout, std::cerr, std::endl, std::vector, drogon::app, std::regex;

bool isInteger(const string &s);

IP validIP(const string &s);

vector<string> split(const string &s, char delimiter = ' ');

namespace dm {
    export class DataManager {
        vector<IPAddress> ips;
        bool &SSL;
        vector<int> ports;
        cli::CLI parser_obj{};
        cli::param &ipParam, &SSLParam, &portParam;

    public:
        DataManager(bool SSL_, const vector<int> ports_);

        void expire() = delete;

        void acquireData(const std::vector<std::string> &args, const int &argc);

        bool getSSL() const { return SSL; }
        const vector<int> &getPorts() const { return ports; }
        vector<IPAddress> getIPs() const { return ips; }
    };

    inline DataManager::DataManager(bool SSL_, const vector<int> ports_) : SSL(SSL_), ports(ports_),
                                                                           parser_obj(cli::CLI(3)),
                                                                           ipParam(parser_obj.add("ip", "ip", "i")),
                                                                           SSLParam(parser_obj.add("ssl", "ssl", "s")),
                                                                           portParam(parser_obj.add("port", "port", "p")) {}

    inline void DataManager::acquireData(const std::vector<std::string> &args, const int &argc) {
        parser_obj.parse(args, argc);
        if (ipParam.val == "local") {
            ips.emplace_back(IP::IPv4, "127.0.0.1");
            ips.emplace_back(IP::IPv6, "::1");
        } else if (ipParam.val == "network") {
            auto localIPs = getLocalIPs();
            if (localIPs.empty()) {
                cerr << "Error: No local IP addresses found. Defaulting to localhost." << endl;
                ips.emplace_back(IP::IPv4, "127.0.0.1");
                ips.emplace_back(IP::IPv6, "::1");
            }
            for (const auto &ip: localIPs) {
                cout << "Found " << ip.version << " address: " << ip.address << endl;
            }
            ips = std::move(localIPs);
        } else if (!ipParam.val.empty()) {
            vector<string> ips = split(ipParam.val, ',');
            for (const auto &ip: ips) {
                IP ver = validIP(ip);
                if (ver != IP::Invalid) {
                    this->ips.emplace_back(ver, ip);
                } else {
                    cerr << "Warning: Invalid IP address skipped - " << ip << endl;
                }
            }
        }
        if (ips.empty()) {
            cout << "Do you want to host it on local network🛜[1] or host machine only💻[2] or custom🛠️[3]?" << endl;
            cout << "Type 1, 2, or 3 and press enter (default is 2): ";
            string temp;
            getline(std::cin, temp);
            int option = 2; // default
            try {
                if (!temp.empty()) option = stoi(temp);
            } catch (...) {
                option = 2;
            } //input validation
            switch (option) {
                case 1: {
                    //local network
                    try {
                        ips = getLocalIPs();
                    } catch (...) {
                        cerr << "Error: No local IP addresses found. Defaulting to localhost." << endl;
                        ips.emplace_back(IP::IPv4, "127.0.0.1");
                        ips.emplace_back(IP::IPv6, "::1");
                    }
                }
                break;
                case 3: {
                    string ipAddress;
                    getline(std::cin, ipAddress);
                    while (ipAddress.empty() || validIP(ipAddress) == IP::Invalid) {
                        cout << "Enter custom IP address: ";
                        getline(std::cin, ipAddress);
                    }
                    while (!ipAddress.empty()) {
                        if (IP ver = validIP(ipAddress); ver != IP::Invalid) {
                            ips.emplace_back(ver, ipAddress);
                        } else {
                            cerr << "Warning: Invalid IP address skipped - " << ipAddress << endl;
                        }
                        cout << "Enter custom IP address (or press enter to finish): ";
                        getline(std::cin, ipAddress);
                    }
                }
                break;
                default: {
                    ips.emplace_back(IP::IPv4, "127.0.0.1");
                    ips.emplace_back(IP::IPv6, "::1");
                }
            }
        }
        ports.resize(ips.size());
        SSL = SSLParam.val == "y" || SSLParam.val == "Y" || SSLParam.val == "true";
        if (SSLParam.val.empty()) {
            cout << "Do you want to enable SSL? (y/n, default is n): ";
            string temp;
            getline(std::cin, temp);
            SSL = temp == "y" || temp == "Y";
        }
        if (portParam.val == "default") {
            for (size_t i = 0; i < ports.size(); ++i) {
                ports[i] = SSL ? 443 : 80;
            }
        } else if (!portParam.val.empty()) {
            vector<string> portStrs = split(portParam.val, ',');
            for (size_t i = 0; i < ports.size() && i < portStrs.size(); i++) {
                if (isInteger(portStrs[i])) {
                    if (const int port = std::stoi(portStrs[i]); port >= 0 && port <= 65535) {
                        ports[i] = port;
                    } else {
                        cerr << "Warning: Port number out of range (0-65535) - " << port << ". Using default." << endl;
                        ports[i] = SSL ? 443 : 80;
                    }
                } else {
                    cerr << "Warning: Invalid port number - " << portStrs[i] << ". Using default." << endl;
                    ports[i] = SSL ? 443 : 80;
                }
            }
        } else {
            cout << "Enter port numbers for each IP address (comma separated, default is " << (SSL ? "443" : "80") <<
                    "): ";
            string temp;
            getline(std::cin, temp);
            vector<string> portStrs = split(temp, ',');
            for (size_t i = 0; i < ports.size() && i < portStrs.size(); ++i) {
                if (isInteger(portStrs[i])) {
                    int port = std::stoi(portStrs[i]);
                    if (port >= 0 && port <= 65535) {
                        ports[i] = port;
                    } else {
                        cerr << "Warning: Port number out of range (0-65535) - " << port << ". Using default." << endl;
                        ports[i] = SSL ? 443 : 80;
                    }
                } else {
                    cerr << "Warning: Invalid port number - " << portStrs[i] << ". Using default." << endl;
                    ports[i] = SSL ? 443 : 80;
                }
            }
        }
    }
}

inline bool isInteger(const string &s) {
    try {
        std::size_t pos;
        int a = std::stoi(s, &pos);
        return pos == s.length();
    } catch (std::out_of_range &e) {
        // Value is a valid integer representation but outside the 'int' range
        return false;
    } catch (std::exception &e) {
        // Not a valid integer
        return false;
    }
}

inline IP validIP(const string &s) {
    regex ipv4_pattern(
        R"(^((25[0-5]|(2[0-4]|1\d|[1-9]|)\d)\.?\b){4}$)");
    regex ipv6_pattern(
        R"(^(([0-9a-fA-F]{1,4}:){7}([0-9a-fA-F]{1,4}|:))|(([0-9a-fA-F]{1,4}:){1,7}:)|(([0-9a-fA-F]{1,4}:){1,6}:[0-9a-fA-F]{1,4})|(([0-9a-fA-F]{1,4}:){1,5}(:[0-9a-fA-F]{1,4}){1,2})|(([0-9a-fA-F]{1,4}:){1,4}(:[0-9a-fA-F]{1,4}){1,3})|(([0-9a-fA-F]{1,4}:){1,3}(:[0-9a-fA-F]{1,4}){1,4})|(([0-9a-fA-F]{1,4}:){1,2}(:[0-9a-fA-F]{1,4}){1,5})|([0-9a-fA-F]{1,4}:)((:[0-9a-fA-F]{1,4}){1,6})|:((:[0-9a-F]{1,4}){1,7}|:))$)"
    );
    if (std::regex_match(s, ipv4_pattern)) {
        cout << "Valid IPv4 address: " << s << endl;
        return IP::IPv4;
    }
    if (std::regex_match(s, ipv6_pattern)) {
        cout << "Valid IPv6 address: " << s << endl;
        return IP::IPv6;
    }
    cerr << "Invalid IP address format: " << s << endl;
    return IP::Invalid;
}

inline vector<string> split(const string &s, const char delimiter) {
    vector<string> tokens;
    string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    return tokens;
}
