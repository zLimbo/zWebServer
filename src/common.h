#ifndef __COMMON_H__
#define __COMMON_H__

#include <sstream>
#include <string>
#include <thread>

inline std::string getCurrentThreadId() {
    std::stringstream ss;
    ss << std::this_thread::get_id();
    return ss.str();
}

#endif  // __COMMON_H__