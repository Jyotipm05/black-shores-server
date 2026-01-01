#ifndef PATH_FINDER_HPP
#define PATH_FINDER_HPP

#include <string>
#include <filesystem>

namespace fs = std::filesystem;
using namespace std;

inline fs::path findParent(string target) {
    fs::path p = fs::current_path();
    while (!fs::exists(p / target)) {
        p = p.parent_path();
        if (p == p.root_path()) {
            throw runtime_error("Could not find '" + target + "' directory in any parent directories.");
        }
    }
    return p;
}

string reSlash(string path) {// replace all '\\' with '/'
    for (size_t i = 0; i < path.length(); i++) {
        if (path[i] == '\\') {
            path[i] = '/';
        }
    }
    return path;

}

#endif
