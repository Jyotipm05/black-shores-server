//
// Created by jmbab on 22-02-2026.
//

#include "stream_test.hpp"
#include "../pathFinder.hpp"
#include <drogon/HttpResponse.h>
#include <filesystem>
#include <string>
#include <fstream>

using namespace std;

static drogon::ContentType guessMime(const std::string &path) {
    // Very small MIME mapping sufficient for video files used in this project
    if (path.ends_with(".mp4")) return CT_VIDEO_MP4;
    if (path.ends_with(".webm")) return CT_VIDEO_WEBM;
    if (path.ends_with(".ogg")) return CT_VIDEO_OGG;
    return CT_APPLICATION_OCTET_STREAM;
}

void stream_test::handleStreamTest(const drogon::HttpRequestPtr &req,
                                   std::function<void (const drogon::HttpResponsePtr &)> &&callback) {
    auto path = req->getParameter("file");
    string root = findParent("src").string();
    string filePath = root + "/media/" + path;
    auto fail = [] {
        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setStatusCode(drogon::k404NotFound);
        resp->setBody("File not found");
        resp->setContentTypeCode(CT_TEXT_PLAIN);
        return resp;
    };
    if (!filesystem::exists(filePath)) {
        fail();
    }
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        fail();
    }
    auto resp = drogon::HttpResponse::newStreamResponse(
        [&file](char* buf, size_t maxLen) -> size_t {
            file.read(buf, maxLen);
            return file.gcount();
        }, "", guessMime(filePath)
    );
    resp->addHeader("Accept-Ranges", "bytes");
    callback(resp);
}
