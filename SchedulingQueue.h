#ifndef SCHEDULINGQUEUE_H
#define SCHEDULINGQUEUE_H

#include "Process.h"

class SchedulingQueue {
public:
    virtual ~SchedulingQueue() = default;

    // Métodos puros virtuales que cada tipo de cola debe implementar
    virtual void addProcess(Process* p) = 0;
    virtual Process* getNextProcess() = 0;
    virtual void removeCurrentProcess() = 0;
    virtual bool empty() const = 0;

    // Método virtual para manejar la lógica al final de un "tick" del reloj
    // Esencial para Round Robin para re-encolar un proceso si su quantum expira
    virtual void onTickEnd(Process* currentProcess, bool quantumExpired) {}

    // Devuelve el quantum de la cola. Devuelve 0 si no es aplicable (ej. FCFS)
    virtual int getQuantum() const { return 0; }
};

#endif
