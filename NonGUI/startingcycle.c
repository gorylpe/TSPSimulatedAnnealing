#include "startingcycle.h"

void firstCycleAscending(int n, int* cycle) {
    for (int i = 0; i < n; ++i) {
        cycle[i] = i;
    }
}

void permuteCycle(int n, int* cycle){
    for(int i = n - 1; i >= 0; --i){
        int j = rand()%(i + 1);
        swap(&cycle[i], &cycle[j]);
    }
}

void firstCycleRandom(int n, int* cycle) {
    firstCycleAscending(n, cycle);
    permuteCycle(n, cycle);
}