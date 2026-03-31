module;
#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <variant>
#include <map>
#include <unordered_map>
#include <ranges>
export module Data_Manager:CLI;

using std::string, std::conditional_t;

namespace cli {
    export struct param {
        std::string alpha, beta;
        std::string val;

        param() = default;

        param(string a, string b) : alpha(std::move(a)), beta(std::move(b)) {
            alpha = "--" + alpha;
            beta = "-" + beta;
        }

        void setVal(const string &val) {
            this->val = val;
        }

        bool operator==(const string &str) const {
            return str == alpha || str == beta;
        }
    };

    export template <bool reserve=false,int N=0>
    class CLI {
        using nmap = std::conditional_t<reserve, std::unordered_map<string, param>, std::map<string, param>>;
        nmap params;
    public:
        CLI() {
            if constexpr (reserve) {
                params.reserve(N + 1);
            }
        }

        void expire() = delete;

        param &add(const string &name, const string &alpha, const string &beta) {
            params[name] = param(alpha, beta);
            return params[name];
        }

        void parse(const std::vector<std::string> &args, const int &argc) {
            for (size_t i = 0; i < argc; ++i) {
                for (auto &[key, val]: params) {
                    if (val == args[i]) {
                        if (i + 1 < argc && args[i + 1][0] != '-') {
                            // Check if next argument is a value
                            val.setVal(args[i + 1]);
                            ++i; // Skip the value
                        } else {
                            std::cout << "Error: Missing value for " << args[i] << std::endl;
                        }
                    }
                }
            }
        }

        string get(const string &name) {
            if (params.contains(name)) {
                return params[name].val;
            }
            return "";
        }
    };
}
