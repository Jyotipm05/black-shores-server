//
// Created by jmbab on 29-09-2025.
//

#include "home.hpp"
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
        auto page = drogon::HttpResponse::newFileResponse("../webapp/root/index.html");
        page->setContentTypeCode(drogon::CT_TEXT_HTML);
        page->setStatusCode(drogon::k200OK);
        callback(page);
    });

    // Detach the thread so it runs independently
    worker.detach();
}
