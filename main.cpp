#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <memory>

#include "Process.h"
#include "MLQScheduler.h"
#include "RoundRobinQueue.h"
#include "FCFSQueue.h"

// Función para parsear una línea del archivo y crear un objeto Process
Process parseProcess(const std::string& line) {
    std::stringstream ss(line);
    std::string segment;
    std::vector<std::string> seglist;
    while (std::getline(ss, segment, ';')) {
        // Elimina espacios en blanco residuales
        segment.erase(std::remove_if(segment.begin(), segment.end(), isspace), segment.end());
        seglist.push_back(segment);
    }
    return Process(seglist[0], std::stoi(seglist[1]), std::stoi(seglist[2]),
                   std::stoi(seglist[3]), std::stoi(seglist[4]));
}

int main() {
    std::string inputFilename = "mlq001.txt";
    std::string outputFilename = "output_mlq001.txt";

    std::ifstream inputFile(inputFilename);
    if (!inputFile.is_open()) {
        std::cerr << "Error al abrir archivo " << inputFilename << std::endl;
        return 1;
    }

    std::vector<Process> allProcesses;
    std::string line;
    // Omitir las dos primeras líneas de cabecera
    std::getline(inputFile, line);
    std::getline(inputFile, line);

    while (std::getline(inputFile, line)) {
        if (!line.empty() && line[0] != '#') {
            allProcesses.push_back(parseProcess(line));
        }
    }
    inputFile.close();

    // Crear las colas según el esquema RR(3), RR(5), FCFS
    std::vector<std::unique_ptr<SchedulingQueue>> queues;
    queues.push_back(std::make_unique<RoundRobinQueue>(3)); // Cola 1: RR con quantum=3
    queues.push_back(std::make_unique<RoundRobinQueue>(5)); // Cola 2: RR con quantum=5
    queues.push_back(std::make_unique<FCFSQueue>());        // Cola 3: FCFS

    MLQScheduler scheduler(std::move(allProcesses), std::move(queues));
    scheduler.run();

    // Obtener y ordenar los procesos finalizados por su ID para la salida
    auto finished = scheduler.getFinishedProcesses();
    std::sort(finished.begin(), finished.end(),
              [](const Process& a, const Process& b) { return a.id < b.id; });

    // --- ESCRITURA DE RESULTADOS ---
    std::ofstream outputFile(outputFilename);
    if (!outputFile.is_open()) {
        std::cerr << "Error al crear archivo " << outputFilename << std::endl;
        return 1;
    }

    outputFile << "# archivo: " << inputFilename << std::endl;
    outputFile << "# etiqueta; BT; AT; Q; Pr; WT; CT; RT; TAT\n";

    double totalWT = 0, totalCT = 0, totalRT = 0, totalTAT = 0;
    for (const auto& p : finished) {
        outputFile << p.id << ";" << p.burstTime << ";" << p.arrivalTime << ";" << p.queueLevel << ";"
                   << p.priority << ";" << p.waitingTime << ";" << p.completionTime << ";"
                   << p.responseTime << ";" << p.turnaroundTime << "\n";
        totalWT += p.waitingTime;
        totalCT += p.completionTime;
        totalRT += p.responseTime;
        totalTAT += p.turnaroundTime;
    }

    outputFile << std::fixed << std::setprecision(1);
    outputFile << "WT=" << totalWT / finished.size() << "; "
               << "CT=" << totalCT / finished.size() << "; "
               << "RT=" << totalRT / finished.size() << "; "
               << "TAT=" << totalTAT / finished.size() << ";\n";
    outputFile.close();

    std::cout << "Simulación completada. Resultados en " << outputFilename << std::endl;

    return 0;
}
