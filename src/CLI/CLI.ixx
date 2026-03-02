module;
#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <functional>
#include <variant>
#include <map>
#include <ranges>
export module CLI;

using std::string;

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
            return alpha == str || beta == str;
        }
    };

    export class CLI {
    public:
        CLI() = default;

        explicit CLI(int n) {
            // params.reserve(n + 1);
        }

        void expire() = delete;

        param &add(const string &name, const string &alpha, const string &beta) {
            params[name] = param(alpha, beta);
            return params[name];
        }

        void parse(const std::vector<std::string> &args, const int &argc) {
            for (size_t i = 0; i < argc; ++i) {
                for (auto &val: params | std::views::values) {
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

    private:
        std::map<string, param> params;
    };
}
