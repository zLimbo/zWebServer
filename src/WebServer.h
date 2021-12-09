#ifndef __WEBSERVER_H__
#define __WEBSERVER_H__

#include <sys/epoll.h>

#include "Singleton.h"

class WebServer : public Singleton<WebServer> {
public:
    constexpr static int EPOLL_SIZE = 1024;
    constexpr static int MAX_EVENT_SIZE = 1024;

    enum TRIGGER_MODE { LT, ET };

public:
    ~WebServer();
    void init(int port, TRIGGER_MODE mode = LT);
    void eventListen();
    void eventLoop();
    void handleConnectEvent(struct epoll_event &event);
    void handleReadEvent(struct epoll_event &event);
    void handleWriteEvent(struct epoll_event &event);

    int setNonblocking(int fd);
    int addEvent(int fd, bool enableET = false);

private:
    int mPort;
    int mTriggerMode;
    int mListenFd;
    int mEpollFd;
};

#endif  // __WEBSERVER_H__