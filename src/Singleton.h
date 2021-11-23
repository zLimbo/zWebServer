#ifndef __SINGLETON_H__
#define __SINGLETON_H__

template <typename T>
class Singleton {
public:
    static T& getInstance() {
        static T t;
        return t;
    }

public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

protected:
    Singleton() = default;
    ~Singleton() = default;
};

#endif  // __SINGLETON_H__