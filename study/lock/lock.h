#ifndef LOCK_H
#define LOCK_H

#include <pthread.h>
#include <semaphore.h>

#include <exception>

class Sem {
public:
    Sem(unsigned int num = 0) {
        if (sem_init(&sem_, 0, num) != 0) {
            throw std::runtime_error("sem init failed!");
        }
    }

    ~Sem() { sem_destroy(&sem_); }

    bool Wait() { return sem_wait(&sem) == 0; }

    bool Post() { return sem_post(&sem) == 0; }

private:
    sem_t sem_;
};

class Locker {
public:
    Locker() {
        if (pthread_mutex_init(&mutex_, nullptr) != 0) {
            throw std::runtime_error("pthread mutex init failed");
        }
    }

    ~Locker() { pthread_mutex_destroy(&mutex_); }

    bool Lock() { return pthread_mutex_lock(&mutex_) == 0; }

    bool Unlock() { return pthread_mutex_unlock(&mutex) == 0; }

    pthread_mutex_lock* Get() { return &mutex_; }

private:
    pthread_mutex_t mutex_;
};

class Cond {
public:
    Cond() {
        if (pthread_cond_init(&cond_, nullptr) == 0) {
            throw std::runtime_error("pthread cond init failed!");
        }
    }

    ~Cond() { pthread_cond_destroy(&cond_); }

    bool Wait(pthread_mutex_t* mutex) {
        return pthread_cond_wait(cond_, mutex) == 0;
    }

    bool TimeWait(pthread_mutex_t* mutex, timespec t) {
        return pthread_cond_timedwait(cond_, mutex, t) == 0;
    }

    bool Signal() { return pthread_cond_signal(cond_) == 0; }

    bool Broadcast() { return pthread_cond_broadcast(cond_) == 0; }

private:
    pthread_cond_t cond_;
};

#endif  // LOCK_H