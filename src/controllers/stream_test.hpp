//
// Created by jmbab on 22-02-2026.
//

#ifndef BLACK_SHORES_SERVER_STREAM_TEST_HPP
#define BLACK_SHORES_SERVER_STREAM_TEST_HPP
#include <drogon/HttpController.h>
#include <string>

using namespace drogon;

class stream_test : public drogon::HttpController<stream_test> {
public:
    METHOD_LIST_BEGIN
        ADD_METHOD_VIA_REGEX(stream_test::handleStreamTest, "/stream_test", Get);
    METHOD_LIST_END

    // Drogon's macro expects path parameter as std::string (by value). Use std::string here.
    void handleStreamTest(const drogon::HttpRequestPtr &req,
                          std::function<void (const drogon::HttpResponsePtr &)> &&callback);
};


#endif //BLACK_SHORES_SERVER_STREAM_TEST_HPP
