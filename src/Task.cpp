#include "Task.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <thread>

#include "spdlog/spdlog.h"

void Task::handle(int client_fd) {
    std::thread th([=]() {
        char buf[1024];
        int sz = read(client_fd, buf, sizeof(buf));
        spdlog::info("read from {}: {}\n", client_fd, buf);
        write(client_fd, buf, strlen(buf));
        close(client_fd);
    });
    th.detach();
}
