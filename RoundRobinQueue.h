#ifndef ROUNDROBINQUEUE_H
#define ROUNDROBINQUEUE_H

#include "SchedulingQueue.h"
#include <queue>

class RoundRobinQueue : public SchedulingQueue {
private:
    std::queue<Process*> q;
    int quantum;

public:
    explicit RoundRobinQueue(int quantum) : quantum(quantum) {}

    void addProcess(Process* p) override;
    Process* getNextProcess() override;
    void removeCurrentProcess() override;
    bool empty() const override;
    int getQuantum() const override { return quantum; }
    void onTickEnd(Process* currentProcess, bool quantumExpired) override;
};

#endif
