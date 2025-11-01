#include "ChatWS.h"
#include <iostream>

using namespace std;

void ChatWS::handleNewMessage(const WebSocketConnectionPtr &wsConnPtr, std::string &&message,
                              const WebSocketMessageType &type) {
    // write your application logic here
    if (message != "")
        cout << message << endl;
    wsConnPtr->send("Thank You");
}

void ChatWS::handleNewConnection(const HttpRequestPtr &req, const WebSocketConnectionPtr &wsConnPtr) {
    // write your application logic here
    cout << "New connection established." << endl;
}

void ChatWS::handleConnectionClosed(const WebSocketConnectionPtr &wsConnPtr) {
    // write your application logic here
    cout << "Connection closed." << endl;
}
