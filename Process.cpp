#include "Process.h"
#include <sstream>
#include <iomanip>

void Process::calculateMetrics() {
    turnaroundTime = completionTime - arrivalTime;
    waitingTime = turnaroundTime - burstTime;

    // Si aún no se calculó responseTime (por seguridad)
    if (responseTime < 0) {
        responseTime = completionTime - arrivalTime - burstTime;
    }
}

// (Opcional, pero útil para depuración)
std::string Process::toString() const {
    std::ostringstream ss;
    ss << std::left << std::setw(5) << id
       << " BT=" << std::setw(3) << burstTime
       << " AT=" << std::setw(3) << arrivalTime
       << " Q=" << std::setw(2) << queueLevel
       << " Pr=" << std::setw(2) << priority
       << " WT=" << std::setw(3) << waitingTime
       << " CT=" << std::setw(3) << completionTime
       << " RT=" << std::setw(3) << responseTime
       << " TAT=" << std::setw(3) << turnaroundTime;
    return ss.str();
}
