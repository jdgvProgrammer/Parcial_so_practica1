#include "MLQScheduler.h"
#include <algorithm>
#include <climits>

MLQScheduler::MLQScheduler(std::vector<Process> procs, std::vector<std::unique_ptr<SchedulingQueue>> q)
    : processes(std::move(procs)), queues(std::move(q)) {}

void MLQScheduler::admitNewProcesses() {
    for (auto& p : processes) {
        // Si el proceso llega en el tiempo actual y aún no ha sido admitido
        if (p.arrivalTime == currentTime && p.state == ProcessState::READY && p.responseTime == -1) {
            queues[p.queueLevel - 1]->addProcess(&p);
        }
    }
}

Process* MLQScheduler::selectNextProcess() {
    // Busca un proceso en las colas, respetando la prioridad (de la cola 0 en adelante)
    for (auto& queue : queues) {
        Process* next = queue->getNextProcess();
        if (next != nullptr) {
            return next;
        }
    }
    return nullptr;
}

void MLQScheduler::releaseProcessFromQueue(Process* p) {
    if (p != nullptr) {
        queues[p->queueLevel - 1]->removeCurrentProcess();
    }
}

void MLQScheduler::run() {
    size_t totalProcesses = processes.size();

    while (finishedProcesses.size() < totalProcesses) {
        admitNewProcesses();

        // Si no hay proceso en ejecución, selecciona el siguiente
        if (runningProcess == nullptr) {
            runningProcess = selectNextProcess();
            if (runningProcess != nullptr) {
                runningProcess->state = ProcessState::RUNNING;
                timeInQuantum = 0;
                // Si es la primera vez que se ejecuta, calcula el Response Time
                if (runningProcess->responseTime == -1) {
                    runningProcess->responseTime = currentTime - runningProcess->arrivalTime;
                }
            }
        }

        // Si hay un proceso listo para ejecutar
        if (runningProcess != nullptr) {
            runningProcess->remainingBurstTime--;
            timeInQuantum++;

            int queueIdx = runningProcess->queueLevel - 1;
            int quantum = queues[queueIdx]->getQuantum();
            bool quantumExpired = (quantum > 0 && timeInQuantum >= quantum);

            // Verifica si el proceso ha terminado
            if (runningProcess->remainingBurstTime == 0) {
                runningProcess->state = ProcessState::FINISHED;
                runningProcess->completionTime = currentTime + 1;
                runningProcess->calculateMetrics();
                finishedProcesses.push_back(*runningProcess);
                releaseProcessFromQueue(runningProcess);
                runningProcess = nullptr;
            }
            // Verifica si el quantum ha expirado
            else if (quantumExpired) {
                runningProcess->state = ProcessState::READY;
                queues[queueIdx]->onTickEnd(runningProcess, true);
                runningProcess = nullptr;
            }
        }

        currentTime++;

        // Optimización: si no hay procesos en ejecución ni en colas,
        // avanza el tiempo hasta la llegada del próximo proceso.
        if (runningProcess == nullptr) {
            bool allQueuesEmpty = true;
            for(const auto& q : queues) {
                if (!q->empty()) {
                    allQueuesEmpty = false;
                    break;
                }
            }
            if (allQueuesEmpty && finishedProcesses.size() < totalProcesses) {
                int nextArrival = INT_MAX;
                for (const auto& p : processes) {
                    if (p.state == ProcessState::READY && p.arrivalTime > currentTime) {
                        nextArrival = std::min(nextArrival, p.arrivalTime);
                    }
                }
                if (nextArrival != INT_MAX) {
                    currentTime = nextArrival;
                }
            }
        }
    }
}

const std::vector<Process>& MLQScheduler::getFinishedProcesses() const {
    return finishedProcesses;
}
