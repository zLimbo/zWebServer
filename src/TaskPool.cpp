#include "TaskPool.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <thread>

#include "common.h"
#include "spdlog/spdlog.h"

void TaskPool::handleRequest(int clientFd) {
    std::string currentThreadId = getCurrentThreadId();
    spdlog::info("thread {} handle request {}", currentThreadId, clientFd);

    char buf[1024];
    int sz = read(clientFd, buf, sizeof(buf));
    spdlog::info("thread {} read from {}({} bytes): {}", currentThreadId, clientFd, sz, buf);
    write(clientFd, buf, strlen(buf) + 1);
    close(clientFd);
    spdlog::info("thread {} handle over", currentThreadId);
}

void TaskPool::init(int threadNum) {
    assert(threadNum > 0);
    for (int i = 0; i < threadNum; ++i) {
        std::thread(worker, this).detach();
    }
}

void TaskPool::run() {
    while (true) {
        int clientFd = -1;
        {
            std::unique_lock lock(mMutex);
            mCv.wait(lock, [&]() {
                bool canRun = !mRequestQueue.empty();
                return canRun;
            });
            clientFd = mRequestQueue.front();
            mRequestQueue.pop();
        }
        handleRequest(clientFd);
    }
}

void TaskPool::addTask(int clientFd) {
    std::unique_lock lock(mMutex);
    mRequestQueue.push(clientFd);
    spdlog::info("add task and notify one thread");
    mCv.notify_one();
}
