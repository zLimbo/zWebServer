#ifndef BLOCKING_QUEUE_H
#define BLOCKING_QUEUE_H

#include <condition_variable>
#include <mutex>
#include <queue>

template <typename T>
class BlockingQueue {
public:
    BlockingQueue(size_t maxSize) : mMaxSize(maxSize) {}

    template <typename... Args>
    void enqueue(Args&&... args) {
        std::unique_lock<std::mutex> locker(mMutex);
        mNotFullCv.wait(locker, [this] { return mQueue.size() < mMaxSize; });

        mQueue.emplace(std::forward<Args>(args)...);
        mNotEmptyCv.notify_one();
    }

    T dequeue() {
        std::unique_lock<std::mutex> locker(mMutex);
        mNotEmptyCv.wait(locker, [this] { return !mQueue.empty(); });
        T elem = std::move(mQueue.front());
        mQueue.pop();
        return elem;
    }

private:
    std::queue<T> mQueue;
    std::mutex mMutex;
    std::condition_variable mNotEmptyCv;
    std::condition_variable mNotFullCv;
    size_t mMaxSize;
};

#endif  // BLOCKINGQUEUE_H