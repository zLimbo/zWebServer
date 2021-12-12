#include "WebServer.h"

#include <arpa/inet.h>
#include <error.h>
#include <netinet/in.h>
#include <spdlog/spdlog.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

#include <string>

#include "common.h"

WebServer::~WebServer() {
    close(mListenFd);
    close(mEpollFd);
}

WebServer::WebServer(int port, TRIGGER_MODE mode, int threadPoolSize)
    : mPort(port), mTriggerMode(mode) {
    mThreadPool = std::make_unique<ThreadPool>(threadPoolSize);
    eventListen();
    eventLoop();
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
    setNonblocking(fd);
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
    ret = listen(mListenFd, LISTEN_QUEUE_SIZE);
    assert(ret >= 0);

    mEpollFd = epoll_create(EPOLL_SIZE);
    addEvent(mListenFd, TRIGGER_MODE::ET == mTriggerMode);
    spdlog::set_level(spdlog::level::debug);
    spdlog::debug("listenFd: {}, epollFd: {}", mListenFd, mEpollFd);
}

void WebServer::eventLoop() {
    while (true) {
        spdlog::info("Server wait event ...");

        struct epoll_event events[MAX_EVENT_SIZE];
        int eventNum = epoll_wait(mEpollFd, events, MAX_EVENT_SIZE, -1);
        spdlog::debug("event num: {}", eventNum);
        for (int i = 0; i < eventNum; ++i) {
            struct epoll_event &event = events[i];
            spdlog::debug("event idx: {}, event fd: {}, event kind: {}", i,
                          (int)event.data.fd, (int)event.events);
            if (event.data.fd == mListenFd) {
                spdlog::debug("[Connect Event]");
                handleConnectEvent(event);
                // mThreadPool->put(&WebServer::handleConnectEvent, this,
                // event);
            } else if (event.events & EPOLLIN) {
                spdlog::debug("[Read Event]");
                mThreadPool->put(&WebServer::handleReadEvent, this, event);
            } else if (event.events & EPOLLOUT) {
                spdlog::debug("[Write Event]");
                mThreadPool->put(&WebServer::handleWriteEvent, this, event);
            }
        }
    }
}

void WebServer::handleConnectEvent(struct epoll_event &event) {
    std::string currentThreadId = getCurrentThreadId();
    spdlog::debug("thread {} handle connect", currentThreadId);
    struct sockaddr_in clientSock;
    socklen_t clientSockLen = sizeof(clientSock);
    while (true) {
        int clientFd =
            accept(mListenFd, (struct sockaddr *)&clientSock, &clientSockLen);
        if (clientFd < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                break;
            }
            throw std::runtime_error("accept error!!!");
        }
        spdlog::debug("connect {}:{}", inet_ntoa(clientSock.sin_addr),
                      ntohs(clientSock.sin_port));
        addEvent(clientFd, TRIGGER_MODE::ET == mTriggerMode);
    }
    spdlog::debug("thread {} handle connect over", currentThreadId);
}

void WebServer::handleReadEvent(struct epoll_event &event) {
    int clientFd = event.data.fd;
    std::string currentThreadId = getCurrentThreadId();
    spdlog::debug("thread {} handle request {}", currentThreadId, clientFd);

    std::string data;
    char buf[4];
    while (true) {
        bzero(buf, sizeof(buf));
        int sz = read(clientFd, buf, sizeof(buf) - 1);
        spdlog::debug("thread {} read from {}({} bytes): [{}]", currentThreadId,
                      clientFd, sz, buf);
        if (sz < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                break;
            }
            throw std::runtime_error("read exception!!!");
        } else if (sz == 0) {
            break;
        }
        data.append(buf, sz);
    }
    spdlog::info("thread {} read from {}, data: [{}]", currentThreadId,
                 clientFd, data);
    write(clientFd, data.c_str(), data.size());
    close(clientFd);
    spdlog::debug("thread {} handle over", currentThreadId);
}

void WebServer::handleWriteEvent(struct epoll_event &event) {}
