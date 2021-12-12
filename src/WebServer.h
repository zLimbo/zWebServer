#ifndef __WEBSERVER_H__
#define __WEBSERVER_H__

#include <sys/epoll.h>

#include <memory>

#include "Singleton.h"
#include "ThreadPool.h"

class WebServer {
public:
    constexpr static int EPOLL_SIZE = 1024;
    constexpr static int MAX_EVENT_SIZE = 1024;
    constexpr static int DEFAULT_THREAD_POOL_SIZE = 4;
    constexpr static int LISTEN_QUEUE_SIZE = 5;

    enum TRIGGER_MODE { LT, ET };

public:
    WebServer(int port, TRIGGER_MODE mode = LT,
              int threadPoolSize = DEFAULT_THREAD_POOL_SIZE);
    WebServer &operator=(const WebServer &) = delete;
    ~WebServer();

private:
    void eventListen();
    void eventLoop();
    int addEvent(int fd, bool enableET = false);
    void handleConnectEvent(struct epoll_event &event);
    void handleReadEvent(struct epoll_event &event);
    void handleWriteEvent(struct epoll_event &event);

private:
    static int setNonblocking(int fd);

private:
    int mPort;
    int mTriggerMode;
    int mListenFd;
    int mEpollFd;
    std::unique_ptr<ThreadPool> mThreadPool;
};

#endif  // __WEBSERVER_H__