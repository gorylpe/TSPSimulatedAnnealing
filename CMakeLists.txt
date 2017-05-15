cmake_minimum_required(VERSION 3.7)
project(TSPSimulatedAnnealing)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c util.h util.c startingcycle.h startingcycle.c simulatedannealing.h simulatedannealing.c swap2opt.c swap2opt.h)
add_executable(TSPSimulatedAnnealing ${SOURCE_FILES})