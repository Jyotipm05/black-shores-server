module;
#include <print>
#include <iostream>
#include <format>
#include <sstream>

#include "../GetIPv4IPv6.hpp"
#include "../IpCollector.hpp"
#include "../IPvalidator.hpp"

export module Data_Manager:TUI;

using std::cout, std::format, std::cin, std::print, std::println, std::endl;



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

namespace tui {
    export class TUI {
        IPAddress &ip;
        bool &SSL;
        int &port;
        bool &localhost;

        void IPQuery() const;

        void SSLQuery() const;

        void portQuery() const;

        void setLocalHost() const;

    public:
        explicit TUI(IPAddress &ip, bool &SSL, int &port, bool &local) : ip(ip), SSL(SSL), port(port), localhost(local) {
        }

        ~TUI() = default;

        void handleQuery() const;
    };

    void TUI::handleQuery() const {
        if (ip.address.empty()) {
            IPQuery();
        }
        if (port == -1)
            portQuery();
    }

    // i) *** IP address ***---------------------------------------------------------------------------------
    void TUI::IPQuery() const {
        println("Do you want to host on localhost only💻[1], device IP🛜[2], or custom🛠️[3]?");
        println("Type 1, 2, or 3 and press enter (default is 1): ");
        string temp;
        getline(cin, temp);
        int option = 1; // default
        try {
            if (!temp.empty()) option = stoi(temp);
        } catch (...) {
            option = 1;
        } //input validation
        bool failed = false;
        switch (option) {
            case 2: {
                // Device IP - extract only first valid IPv4
                try {
                    auto _ip = IpCollector::getIP();
                    if (const IP ver = validIP(_ip); ver == IPv4) {
                        ip = {_ip, ver};
                        println("Hosting on device IP. Current IP:");
                        cout << format("IP address {} - IPv4", _ip) << endl;

                        // Ask if localhost should be added
                        println("Do you also want to add localhost? (y/n, default is n): ");
                        string addLocalhost;
                        getline(cin, addLocalhost);
                        if (addLocalhost == "y" || addLocalhost == "Y") {
                            setLocalHost();
                        }
                    } else {
                        std::cerr << "Error: Invalid IP - " << _ip << std::endl;
                        println("Falling back to localhost only.");
                        failed = true;
                        port = 80;
                        setLocalHost();
                    }
                } catch (std::runtime_error &e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                    println("Falling back to localhost only.");
                    failed = true;
                    port = 80;
                    setLocalHost();
                }
                break;
            }
            case 3: {
                // Custom IP - single IPv4
                println("Enter custom IPv4 address: ");
                string ipAddress;
                getline(cin, ipAddress);
                if (const IP ver = validIP(ipAddress); ver == IPv4) {
                    ip = {ipAddress, ver};
                    println("Hosting on custom IP. Current IP:");
                    cout << format("IP address {} - IPv4", ipAddress) << endl;

                    // Ask if localhost should be added
                    println("Do you also want to add localhost? (y/n, default is n): ");
                    string addLocalhost;
                    getline(cin, addLocalhost);
                    if (addLocalhost == "y" || addLocalhost == "Y") {
                        setLocalHost();
                    }
                } else {
                    std::cerr << "Error: Invalid or non-IPv4 address - " << ipAddress << std::endl;
                    println("Falling back to localhost only.");
                    failed = true;
                    port = 80;
                    setLocalHost();
                }
                break;
            }
            case 1:
            default: {
                // Localhost only
                println("Hosting on localhost only.");
                setLocalHost();
                port = 80;
                break;
            }
        }
        if (option > 1 && !failed) {
            SSLQuery();
        }
    }

    // ii) *** SSL ***---------------------------------------------------------------------------------
    void TUI::SSLQuery() const {
        println("Do you want to enable SSL? (y/n, default is n): ");
        string temp;
        getline(cin, temp);
        SSL = temp == "y" || temp == "Y";
    }


    // iii) *** port ***---------------------------------------------------------------------------------
    void TUI::portQuery() const {
        cout << format("Enter port number (default is {}): ",
                       (SSL ? "443" : "80")) << endl;
        string temp;
        getline(cin, temp);
        if (temp.empty())
            port = (SSL ? 443 : 80);
        else
            port = stoi(temp);
    }

    void TUI::setLocalHost() const {
        localhost = true;
        cout << "Localhost will be added." << endl;
    }
}
