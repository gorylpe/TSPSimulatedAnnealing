cmake_minimum_required(VERSION 3.7)
project(TSPSimulatedAnnealingGUI)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c util.h util.c startingcycle.h startingcycle.c simulatedannealing.h simulatedannealing.c swap2opt.c swap2opt.h)

FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)

INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS})
LINK_DIRECTORIES(${GTK3_LIBRARY_DIRS})
ADD_DEFINITIONS(${GTK3_CFLAGS_OTHER})

add_executable(TSPSimulatedAnnealingGUI ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(TSPSimulatedAnnealingGUI ${GTK3_LIBRARIES})