#ifndef __WEBSERVER_H__
#define __WEBSERVER_H__

#include "Singleton.h"

class WebServer : public Singleton<WebServer> {
public:
    void init(int port);
    void eventListen();
    void eventLoop();

private:
    int port_;
    int listen_fd_;
    int event_fd_;
};

#endif  // __WEBSERVER_H__