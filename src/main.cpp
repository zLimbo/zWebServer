#include "TaskPool.h"
#include "WebServer.h"

int main() {
    WebServer &webServer = WebServer::getInstance();
    TaskPool &taskPool = TaskPool::getInstance();

    taskPool.init(20);

    webServer.init(8001);
    webServer.eventListen();
    webServer.eventLoop();

    return 0;
}