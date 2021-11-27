#ifndef __TASKPOOL_H__
#define __TASKPOOL_H__

#include <condition_variable>
#include <mutex>
#include <queue>

#include "Singleton.h"

class TaskPool : public Singleton<TaskPool> {
public:
    void handleRequest(int clientFd);

    void init(int threadNum);

    void run();

    void addTask(int clientFd);

    static void worker(TaskPool *taskPool) { taskPool->run(); }

private:
    std::queue<int> mRequestQueue;

    std::mutex mMutex;

    std::condition_variable mCv;
};

#endif  // __TASKPOOL_H__