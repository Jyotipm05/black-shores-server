#include "RootCtrl.h"

void RootCtrl::asyncHandleHttpRequest(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback)
{
    // write your application logic here
    auto resp = HttpResponse::newRedirectionResponse("/home/");
    resp->setStatusCode(k302Found);
    callback(resp);
}
