#ifndef PROCESS_H
#define PROCESS_H

#include <string>

// Enumera los estados posibles de un proceso
enum class ProcessState {
    READY,
    RUNNING,
    FINISHED
};

class Process {
public:
    // Atributos del proceso según el archivo de entrada
    std::string id;
    int burstTime;
    int arrivalTime;
    int queueLevel;
    int priority;

    // Variables para la simulación y métricas
    int remainingBurstTime;
    ProcessState state;

    // Métricas de rendimiento
    int completionTime = 0;
    int turnaroundTime = 0;
    int waitingTime = 0;
    int responseTime = -1; // -1 indica que no ha sido ejecutado

    // Constructor
    Process(const std::string& processId, int bt, int at, int q, int p)
        : id(processId), burstTime(bt), arrivalTime(at), queueLevel(q), priority(p),
          remainingBurstTime(bt), state(ProcessState::READY) {}

    // Calcula las métricas de rendimiento cuando el proceso finaliza
    void calculateMetrics() {
        turnaroundTime = completionTime - arrivalTime;
        waitingTime = turnaroundTime - burstTime;
    }
};

#endif
