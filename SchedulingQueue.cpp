// Sugerencia para SchedulingQueue.h
#ifndef SCHEDULINGQUEUE_H
#define SCHEDULINGQUEUE_H

#include "Process.h"

// Clase base abstracta para todas las colas de planificación
class SchedulingQueue {
public:
    virtual ~SchedulingQueue() = default;

    // Añade un proceso a la cola
    virtual void addProcess(Process* p) = 0;

    // Devuelve el siguiente proceso a ejecutar SIN sacarlo de la cola
    virtual Process* getNextProcess() = 0;

    // Saca el proceso actual de la cola (se llama cuando el proceso termina o es expropiado)
    virtual void removeCurrentProcess() = 0;

    // Devuelve true si no hay procesos en la cola
    virtual bool empty() const = 0;

    // Función para manejar la lógica específica de la cola al final de un tick
    // (Ej: mover un proceso RR al final de su propia cola)
    // El scheduler principal le pasa el proceso que se estaba ejecutando.
    virtual void onTickEnd(Process* currentProcess, bool quantumExpired) {}

    // Devuelve el quantum de la cola (si aplica)
    virtual int getQuantum() const { return 0; }
};

#endif