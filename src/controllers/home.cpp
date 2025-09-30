//
// Created by jmbab on 29-09-2025.
//

#include "home.h"
#include <drogon/HttpResponse.h>
#include <drogon/HttpAppFramework.h>
#include <thread>
#include <fstream>
#include <sstream>

void home::handleHome(const drogon::HttpRequestPtr &req,
                      std::function<void (const drogon::HttpResponsePtr &)> &&callback) {

    // Use C++ native threading system
    std::thread worker([callback]() {
        // Create the response in the worker thread
        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setStatusCode(drogon::k200OK);
        resp->setContentTypeCode(drogon::CT_TEXT_HTML);

        std::ifstream file("../webapp/root/index.html");
        if (!file.is_open()) {
            resp->setStatusCode(drogon::k500InternalServerError);
            resp->setBody("Error: Something went wrong while loading the page.");
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        resp->setBody(buffer.str());
        file.close();
        callback(resp);
    });

    // Detach the thread so it runs independently
    worker.detach();
}