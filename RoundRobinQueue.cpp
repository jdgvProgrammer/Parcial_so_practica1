#include "RoundRobinQueue.h"

void RoundRobinQueue::addProcess(Process* p) {
    q.push(p);
}

Process* RoundRobinQueue::getNextProcess() {
    return q.empty() ? nullptr : q.front();
}

void RoundRobinQueue::removeCurrentProcess() {
    if (!q.empty()) {
        q.pop();
    }
}

bool RoundRobinQueue::empty() const {
    return q.empty();
}

void RoundRobinQueue::onTickEnd(Process* currentProcess, bool quantumExpired) {
    // Si el quantum expiró, el proceso no ha terminado y sigue al frente de la cola,
    // se mueve al final de la misma.
    if (quantumExpired && currentProcess != nullptr && !q.empty() && q.front() == currentProcess) {
        q.pop();
        q.push(currentProcess);
    }
}
