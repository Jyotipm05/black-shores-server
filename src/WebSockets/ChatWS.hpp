#pragma once

#include <drogon/WebSocketController.h>
#include <vector>

using namespace drogon;

class ChatWS : public drogon::WebSocketController<ChatWS>
{
  public:
     void handleNewMessage(const WebSocketConnectionPtr&,
                                  std::string &&,
                                  const WebSocketMessageType &) override;
    void handleNewConnection(const HttpRequestPtr &,
                                     const WebSocketConnectionPtr&) override;
    void handleConnectionClosed(const WebSocketConnectionPtr&) override;
    WS_PATH_LIST_BEGIN
    // list path definitions here;
    // WS_PATH_ADD("/path", "filter1", "filter2", ...);
    WS_ADD_PATH_VIA_REGEX("/chat(.*)");
    WS_PATH_LIST_END
    // static std::vector<WebSocketConnectionPtr> users;
};
