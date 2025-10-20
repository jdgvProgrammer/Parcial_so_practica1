#ifndef FCFSQUEUE_H
#define FCFSQUEUE_H

#include "SchedulingQueue.h"
#include <queue>

class FCFSQueue : public SchedulingQueue {
private:
    std::queue<Process*> q;

public:
    void addProcess(Process* p) override;
    Process* getNextProcess() override;
    void removeCurrentProcess() override;
    bool empty() const override;
};

#endif
