#include "WebServer.h"

int main() {
    WebServer &webServer = WebServer::getInstance();

    webServer.init(8001);
    webServer.eventListen();
    webServer.eventLoop();

    return 0;
}