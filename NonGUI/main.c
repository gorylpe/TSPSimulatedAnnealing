#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "util.h"
#include "simulatedannealing.h"
#include "startingcycle.h"

void firstCycleNearestInsertionMax(int n, int* cycle, float** E) {
    firstCycleAscending(n, cycle);
    double len = E(0,1) + E(1,0);
    for (int i = 2; i < n; ++i) {
        int insertIt = i; //dla kolejnych i, cycle[i] = i
        double maxLen = len - E(cycle[insertIt - 1],0) + E(cycle[i - 1],cycle[i]) + E(i,0); //cycle[insertIt % i] = 0 because last element of cycle is 0th;
        for (int j = insertIt - 1; j >= 1; --j) {
            double tmpLen = len - E(cycle[j - 1],cycle[j]) + E(cycle[j - 1],cycle[i]) + E(cycle[i],cycle[j]);
            if (tmpLen > maxLen) {
                maxLen = tmpLen;
                insertIt = j;
            }
        }
        if (insertIt < i) {
            memmove(&cycle[insertIt + 1], &cycle[insertIt], (i - insertIt) * sizeof(int));
            cycle[insertIt] = i;
        }
        len = maxLen;
    }
}

void firstCycleNearestInsertionMin(int n, int* cycle, float** E) {
    firstCycleAscending(n, cycle);
    double len = E(0,1) + E(1,0);
    for (int i = 2; i < n; ++i) {
        int insertIt = i; //dla kolejnych i, cycle[i] = i
        double minLen = len - E(cycle[insertIt - 1],0) + E(cycle[i - 1],cycle[i]) + E(i,0); //cycle[insertIt % i] = 0 because last element of cycle is 0th;
        for (int j = insertIt - 1; j >= 1; --j) {
            double tmpLen = len - E(cycle[j - 1],cycle[j]) + E(cycle[j - 1],cycle[i]) + E(cycle[i],cycle[j]);
            if (tmpLen < minLen) {
                minLen = tmpLen;
                insertIt = j;
            }
        }
        if (insertIt < i) {
            memmove(&cycle[insertIt + 1], &cycle[insertIt], (i - insertIt) * sizeof(int));
            cycle[insertIt] = i;
        }
        len = minLen;
    }
}

double averageEdgeLength(int n, float** E){
    double sum = 0.0;
    double numOfElements = n*(n-1)/2;
    for(int i = n - 1; i > 0; --i){
        for(int j = i - 1; j >= 0; --j){
            sum += E(i,j)/numOfElements;
        }
    }
    return sum;
}

void testFirstTemp(FILE *input, FILE *output, FILE *error){
    int n;
    fscanf(input, "%d", &n);
    printf("%d\n", n);

    //save memory
    float** E;
    E = malloc(n * sizeof(float*));
    for (int i = n - 1; i >= 0; --i) {
        E[i] = malloc((i + 1) * sizeof(float));
    }
    initializeEdges(n, E, input);

    int timeLimit;
    fscanf(input, "%d", &timeLimit);
    timeLimit *= CLOCKS_PER_SEC;
    timeLimit += clock();

    int* cycle = malloc(n * sizeof(int));
    firstCycleNearestInsertionMin(n, cycle, E);
    double minInsertion = cycleLen(n, cycle, E);
    printf("min insertion: %f\n", minInsertion);
    firstCycleNearestInsertionMax(n, cycle, E);
    double maxInsertion = cycleLen(n, cycle, E);
    printf("max insertion: %f\n", maxInsertion);
    printf("avg insertion: %f\n", (maxInsertion + minInsertion)/2);
    double avgEdge = averageEdgeLength(n, E);
    printf("avg edge: %f\n", avgEdge);
    printf("avg n*edge: %f\n", avgEdge * n);

    double avgRandom = 0.0;
    for(int i = 0; i < 1000; ++i){
        firstCycleRandom(n, cycle);
        avgRandom += cycleLen(n, cycle, E) / 1000.0;
    }
    printf("avg random of 1000: %f\n", avgRandom);
}

void mainProgram(FILE* input, FILE* output, FILE* error){
    int n;
    fscanf(input, "%d", &n);

    //save memory
    float** E;
    E = malloc(n * sizeof(float*));
    for (int i = n - 1; i >= 0; --i) {
        E[i] = malloc((i + 1) * sizeof(float));
    }
    initializeEdges(n, E, input);

    int timeLimit;
    fscanf(input, "%d", &timeLimit);
    timeLimit *= CLOCKS_PER_SEC;
    //time for end app and i/o
    timeLimit -= CLOCKS_PER_SEC * n / 10000;

    int* cycle = malloc(n * sizeof(int));
    //firstCycleAscending(n, cycle);
    firstCycleRandom(n, cycle);

    double* cycleLength = malloc(sizeof(double));
    *cycleLength = cycleLen(n, cycle, E);

    simulatedAnnealing(n, E, cycle, cycleLength, n * averageEdgeLength(n, E), timeLimit);

    fprintf(output, "%f\n", *cycleLength);
    int start;
    for (start = 0; start < n && cycle[start] != 0; ++start);
    for (int i = 0; i < n; ++i) {
        fprintf(error, "%d\n", cycle[(start + i)%n] + 1);
    }
    fprintf(error, "%d\n", cycle[start] + 1);

    for (int i = 0; i < n; ++i) {
        free(E[i]);
    }
    free(E);
    free(cycle);
    free(cycleLength);
}

int main(int argc, char* argv[]) {
    FILE* input = stdin;
    FILE* output = stdout;
    FILE* error = stderr;

    srand(time(NULL));

    if(argc > 1)
        input = fopen(argv[1], "r");

    mainProgram(input, output, error);
    //testFirstTemp(input, output, error);

    if(argc > 1)
        fclose(input);
    return 0;
}