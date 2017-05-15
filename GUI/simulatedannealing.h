#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "swap2opt.h"
#include "math.h"
#include "util.h"
#include "startingcycle.h"

int simulatedAnnealing(int n, float** E, int* cycle, double* cycleLength, double Tstart, int timeLimit);