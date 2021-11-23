#include "WebServer.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <string>

#include "spdlog/spdlog.h"

void WebServer::init(int port) { port_ = port; }

void WebServer::eventListen() {
    listen_fd_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    assert(listen_fd_ > 0);

    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port_);

    int flag = 1;
    setsockopt(listen_fd_, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
    int ret = bind(listen_fd_, (struct sockaddr *)&address, sizeof(address));
    assert(ret >= 0);
    ret = listen(listen_fd_, 5);
    assert(ret >= 0);

    while (true) {
        struct sockaddr_in conn_sock;
        socklen_t conn_sock_len = sizeof(conn_sock);
        int conn_fd = accept(listen_fd_, (struct sockaddr *)&conn_sock, &conn_sock_len);
        if (conn_fd < 0) {
            spdlog::error("accept error: errno is: {}", errno);
            continue;
        }

        char buf[1024];
        int sz = recv(conn_fd, buf, sizeof(buf), 0);
        fmt::print("");

        
    }
}

void WebServer::eventLoop() {}
