#ifndef MLQSCHEDULER_H
#define MLQSCHEDULER_H

#include <vector>
#include <memory>
#include "Process.h"
#include "SchedulingQueue.h"

class MLQScheduler {
private:
    std::vector<Process> processes; // Almacén principal de todos los procesos
    std::vector<std::unique_ptr<SchedulingQueue>> queues;
    std::vector<Process> finishedProcesses;

    int currentTime = 0;
    Process* runningProcess = nullptr;
    int timeInQuantum = 0;

    void admitNewProcesses();
    Process* selectNextProcess();
    void releaseProcessFromQueue(Process* p);

public:
    MLQScheduler(std::vector<Process> procs, std::vector<std::unique_ptr<SchedulingQueue>> q);
    void run();
    const std::vector<Process>& getFinishedProcesses() const;
};

#endif
