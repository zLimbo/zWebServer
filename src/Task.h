#ifndef __TASK_H__
#define __TASK_H__

#include "Singleton.h"

class Task : public Singleton<Task> {
public:
    void handle(int client_fd);
};

#endif  // __TASK_H__