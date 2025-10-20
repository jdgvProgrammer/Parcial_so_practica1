cmake_minimum_required(VERSION 3.20)
project(MLQ_Scheduler_CLion)

set(CMAKE_CXX_STANDARD 17)

# Añade todos los archivos .cpp al ejecutable
add_executable(MLQ_Scheduler_CLion
        main.cpp
        RoundRobinQueue.cpp
        FCFSQueue.cpp
        MLQScheduler.cpp
)
