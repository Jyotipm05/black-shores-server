#include "CritExit.h"

void CritExit::asyncHandleHttpRequest(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)
{
    // write your application logic here
    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(k200OK);
    resp->setContentTypeCode(CT_TEXT_PLAIN);
    resp->setBody("This is a critical exit endpoint. The server will shut down now.");
    callback(resp);
    app().getLoop()->runAfter(1.0, []() {
        std::cout << "Critical exit initiated. Shutting down the server..." << std::endl;
        app().quit();
    });
}
