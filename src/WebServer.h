#ifndef __WEBSERVER_H__
#define __WEBSERVER_H__

#include "Singleton.h"

class WebServer : public Singleton<WebServer> {
public:
    void init(int port);
    void eventListen();
    void eventLoop();

private:
    int mPort;
    int mListenFd;
    int mEventFd;
};

#endif  // __WEBSERVER_H__