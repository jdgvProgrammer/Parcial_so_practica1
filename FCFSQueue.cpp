#include "FCFSQueue.h"

void FCFSQueue::addProcess(Process* p) {
    q.push(p);
}

Process* FCFSQueue::getNextProcess() {
    return q.empty() ? nullptr : q.front();
}

void FCFSQueue::removeCurrentProcess() {
    if (!q.empty()) {
        q.pop();
    }
}

bool FCFSQueue::empty() const {
    return q.empty();
}
