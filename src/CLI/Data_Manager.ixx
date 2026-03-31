module;
#include "../GetIPv4IPv6.hpp"
#include "../IPvalidator.hpp"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <format>
export module Data_Manager;

import :CLI;
import :TUI;

using std::string, std::cout, std::cerr, std::endl, std::vector, std::format;

bool isInteger(const string &s);

export auto localIP = std::initializer_list<IPAddress>{
        {"127.0.0.1", IPv4},
        {"::1", IPv6}
};

namespace dm {
    export class DataManager {
        IPAddress main_ip{};
        bool SSL;
        int port;
        cli::CLI<true, 3> parser_obj{};
        cli::param &ipParam, &SSLParam, &portParam;
        bool localhost = false;

    public:
        DataManager(const bool &SSL_, const int &port);

        void expire() = delete;


        void acquireData(const std::vector<std::string> &args, const int &argc);

        [[nodiscard]] IPAddress getIP() const { return main_ip; }
        [[nodiscard]] bool getSSL() const { return SSL; }
        [[nodiscard]] const int &getPort() const { return port; }
        [[nodiscard]] bool getHost() const { return localhost; }
    };

    DataManager::DataManager(const bool &SSL_, const int &ports_) : SSL(SSL_), port(ports_),
                                                                            parser_obj(cli::CLI<true, 3>()),
                                                                            ipParam(parser_obj.add("ip", "ip", "i")),
                                                                            SSLParam(parser_obj.add("ssl", "ssl", "s")),
                                                                            portParam(parser_obj.add(
                                                                                "port", "port", "p")) {
    }

    inline void DataManager::acquireData(const std::vector<std::string> &args, const int &argc) {
        parser_obj.parse(args, argc);
        tui::TUI tObj(main_ip, SSL, port, localhost);
        tObj.handleQuery();
    }
}

inline bool isInteger(const string &s) {
    try {
        std::size_t pos;
        int _ = std::stoi(s, &pos);
        return pos == s.length();
    } catch (std::out_of_range &_) {
        // Value is a valid integer representation but outside the 'int' range
        return false;
    } catch (std::exception &_) {
        // Not a valid integer
        return false;
    }
}
