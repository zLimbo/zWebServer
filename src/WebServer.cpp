#include "WebServer.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <string>

#include "TaskPool.h"
#include "spdlog/spdlog.h"

void WebServer::init(int port) { mPort = port; }

void WebServer::eventListen() {
    mListenFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    assert(mListenFd > 0);

    struct sockaddr_in serverAddr;
    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(mPort);

    int flag = 1;
    setsockopt(mListenFd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
    int ret =
        bind(mListenFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    assert(ret >= 0);
    ret = listen(mListenFd, 5);
    assert(ret >= 0);

    while (true) {
        spdlog::info("server listen...");
        struct sockaddr_in clientSock;
        socklen_t clientSockLen = sizeof(clientSock);
        int clientFd =
            accept(mListenFd, (struct sockaddr *)&clientSock, &clientSockLen);
        if (clientFd < 0) {
            spdlog::error("accept error: errno is: {}", errno);
            continue;
        }
        spdlog::info("connect {}:{}", inet_ntoa(clientSock.sin_addr),
                     ntohs(clientSock.sin_port));
        TaskPool::getInstance().addTask(clientFd);
    }
}

void WebServer::eventLoop() {}
