#include "WebServer.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <spdlog/spdlog.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

#include <string>

#include "TaskPool.h"

WebServer::~WebServer() {
    close(mListenFd);
    close(mEpollFd);
}

void WebServer::init(int port, TRIGGER_MODE mode) {
    mPort = port;
    mTriggerMode = mode;
}

int WebServer::setNonblocking(int fd) {
    int oldFlag = fcntl(fd, F_GETFL);
    int newFlag = oldFlag | O_NONBLOCK;
    fcntl(fd, F_SETFL, newFlag);
    return oldFlag;
}

int WebServer::addEvent(int fd, bool enableET) {
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN;
    if (enableET) {
        event.events |= EPOLLET;
    }
    int ret = epoll_ctl(mEpollFd, EPOLL_CTL_ADD, fd, &event);
    return ret;
}

void WebServer::eventListen() {
    mListenFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    assert(mListenFd >= 0);

    struct sockaddr_in serverAddr;
    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(mPort);

    int flag = 1;
    int ret = -1;
    setsockopt(mListenFd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
    ret = bind(mListenFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    assert(ret >= 0);
    ret = listen(mListenFd, 5);
    assert(ret >= 0);

    mEpollFd = epoll_create(EPOLL_SIZE);
    addEvent(mListenFd, false);
}

void WebServer::eventLoop() {
    while (true) {
        spdlog::info("wait event ...");

        struct epoll_event events[MAX_EVENT_SIZE];
        int eventNum = epoll_wait(mEpollFd, events, MAX_EVENT_SIZE, -1);

        for (int i = 0; i < eventNum; ++i) {
            struct epoll_event &event = events[i];
            if (event.data.fd == mListenFd) {
                handleConnectEvent(event);
                continue;
            }
            if (event.events & EPOLLIN) {
                handleReadEvent(event);
            }
            if (event.events & EPOLLOUT) {
                handleWriteEvent(event);
            }
        }
    }
}

void WebServer::handleConnectEvent(struct epoll_event &event) {
    spdlog::info("[handleConnectEvent]");
    struct sockaddr_in clientSock;
    socklen_t clientSockLen = sizeof(clientSock);
    int clientFd = accept(mListenFd, (struct sockaddr *)&clientSock, &clientSockLen);
    if (clientFd < 0) {
        spdlog::error("accept error: errno is: {}", errno);
        return;
    }
    spdlog::info("connect {}:{}", inet_ntoa(clientSock.sin_addr), ntohs(clientSock.sin_port));
    addEvent(clientFd, TRIGGER_MODE::ET == mTriggerMode);
}

void WebServer::handleReadEvent(struct epoll_event &event) {
    TaskPool::getInstance().addTask(event.data.fd);
}

void WebServer::handleWriteEvent(struct epoll_event &event) {}
