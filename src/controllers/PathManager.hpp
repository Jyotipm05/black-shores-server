//
// Created by jmbab on 26-11-2025.
//

#ifndef BLACK_SHORES_SERVER_PATHMANAGER_HPP
#define BLACK_SHORES_SERVER_PATHMANAGER_HPP

#include <string>
#include <filesystem>

namespace fs = std::filesystem;
using namespace std;

class PathManager {
public:
    static PathManager& instance() {
        static PathManager inst;
        return inst;
    }

    const std::string& getRootPath() const {
        return rootPath_;
    }

    void setRootPath(const std::string& path) {
        rootPath_ = path;
    }

    void setRootPath(std::string&& path) {
        rootPath_ = std::move(path);
    }

private:
    PathManager() = default;
    ~PathManager() = default;

    PathManager(const PathManager&) = delete;
    PathManager& operator=(const PathManager&) = delete;
    PathManager(PathManager&&) = delete;
    PathManager& operator=(PathManager&&) = delete;

    std::string rootPath_;
};

#endif //BLACK_SHORES_SERVER_PATHMANAGER_HPP