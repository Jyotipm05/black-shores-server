//
// Created by jmbab on 29-09-2025.
//

#ifndef BLACK_SHORES_SERVER_HOME_H
#define BLACK_SHORES_SERVER_HOME_H
#include <drogon/HttpController.h>
using namespace drogon;

class home: public drogon::HttpController<home>
{
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_VIA_REGEX(home::handleHome, "/home(.*)", Get);
    METHOD_LIST_END

    void handleHome(const drogon::HttpRequestPtr &req,
                    std::function<void (const drogon::HttpResponsePtr &)> &&callback);
};


#endif //BLACK_SHORES_SERVER_HOME_H