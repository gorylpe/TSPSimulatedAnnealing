#pragma once
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

//save memory
#define Ed(x,y) ((x) > (y) ? E[(x)][(y)] : E[(y)][(x)])

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

static inline void swap(int* a, int* b){
    int c = *a;
    *a = *b;
    *b = c;
}

static inline int int_pow(int base, int exp){
    int result = 1;
    while(exp){
        if(exp & 1)
            result *= base;
        exp /= 2;
        base *= base;
    }
    return result;
}

float calculateEdgeLength(int a, int b, float **pos);

void printCycle(FILE* stream, int n, int* cycle);

void printEdges(FILE* stream, int n, float** E);

double cycleLen(int n, int* cycle, float** E);

void calculateEdges(int n, float** E, float** pos);

void loadEdges(int n, float** E);

void initializeEdgesAndPositions(int n, float** E, float** pos, FILE* input);

void loadPositions(int n, float** pos, FILE* input);

int calculateBirthdayParadoxValue(int n, float percent);
