#include <arpa/inet.h>
#include <netinet/in.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cassert>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <thread>
#include <vector>

#include "../src/common.h"
#include "spdlog/spdlog.h"

int main(int argc, char **argv) {
    assert(argc >= 3);

    struct sockaddr_in serverAddr;
    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(std::stoi(argv[1]));

    int clientNum = std::stoi(argv[2]);
    std::vector<std::thread> ths;
    for (int i = 0; i < clientNum; ++i) {
        std::thread th(
            [&](int i) {
                int clientFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

                spdlog::info("thread {} start build connect...",
                             getCurrentThreadId());
                int ret = connect(clientFd, (sockaddr *)&serverAddr,
                                  sizeof(serverAddr));
                assert(ret >= 0);

                std::string text = std::string("client ") + std::to_string(i);
                spdlog::info("connect success! send text: {}", text);
                ret = write(clientFd, text.c_str(), text.size() + 1);
                char response[1024];
                ret = read(clientFd, response, sizeof(response));
                spdlog::info("server response: {}", response);
                close(clientFd);
            },
            i);
        ths.push_back(std::move(th));
    }

    for (std::thread &th : ths) {
        th.join();
    }

    return 0;
}