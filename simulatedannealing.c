#include "simulatedannealing.h"
//#define DEBUG

static inline double prob(double T, double diff){
    return exp(-diff/T);
}

static inline int partialSumToN(int n){
    int sum = 0;
    for(int i = 1; i <= n; ++i){
        sum += i;
    }
    return sum;
}

void plotTemp(double Tstart, double Tmin, double coolingRate){
    double T = Tstart;
    while(T > Tmin){
        printf("%f ", T);
        T *=coolingRate;
    }
    printf("\n");
}

bool findAcceptedNeighborSwap(int n, double T, double Tstart, float** E, int *tmpCycle, int* tmpCycleSecondary, double *tmpLengthPtr, int timeLimit){
    double tmpLength = *tmpLengthPtr;
    bool foundNew = false;
    int iterations = 0;
    //new cycle is being searched until it will be accepted
    //it could look that at some point it will last very low number of swaps that could be accepted
    //but at start, number of iterations of this loop will be 1, then it will increase logarythmically
    /*                                  _____________________________
     *                       _________/
     *                   _ /
     *                 /
     * ______________/
     * T = T0        T = T0 / 2
     */
    while(clock() < timeLimit && !foundNew){
        ++iterations;
        //br has to be <= n-1
        //a < b
        //al == br is checked in cycleLen2OptDiff
        int b = rand() % (n - 2) + 1;
        int a = rand() % b;
        int al = (a == 0 ? n - 1 : a - 1);
        int br = b + 1;

        double diff = cycleLen2OptDiff(tmpCycle, E, al, a, b, br);
        if(diff < 0.0){
            foundNew = true;
            swap2Opt(n, tmpCycleSecondary, tmpCycle, al, a, b, br);
            tmpLength += diff;
        } else {
            double randomVal = (double)rand()/(double)RAND_MAX;
            double acceptanceProbability = prob(T, diff);
            if(randomVal > acceptanceProbability || acceptanceProbability < 0.01){
                //do nothing
            } else {
                foundNew = true;
                swap2Opt(n, tmpCycleSecondary, tmpCycle, al, a, b, br);
                tmpLength += diff;
            }
        }
    }
    //printf("temp\%: %f iterations: %d\n", T/Tstart, iterations);
    //printf("%d ", iterations);
    *tmpLengthPtr = tmpLength;
    return foundNew;
}

int simulatedAnnealing(int n, float** E, int* bestCycle, double* bestCycleLength, double Tstart, int timeLimit){
    int* tmpCycle = malloc(n * sizeof(int));
    int* tmpCycleSecondary = malloc(n * sizeof(int));
    double tmpLength = *bestCycleLength;
    memcpy(tmpCycle, bestCycle, n * sizeof(int));

    const double startTempDelta = 0.0821;
    const double minTempDelta = 0.0235;
    const double firstMinTemp = 0.000045389;

    double deltaMin = pow(minTempDelta, log10((double)n) - 2.0) * firstMinTemp;
    double deltaStart = pow(startTempDelta, log10((double)n) - 2.0);
    double T = Tstart * deltaStart;
    double Tmin = deltaMin * Tstart;
    double coolingRate = 0.99995;

    while(clock() < timeLimit && T > Tmin){
        findAcceptedNeighborSwap(n, T, Tstart, E, tmpCycle, tmpCycleSecondary, &tmpLength, timeLimit);

        int* swapCycle = tmpCycle;
        tmpCycle = tmpCycleSecondary;
        tmpCycleSecondary = swapCycle;

        if(tmpLength < *bestCycleLength){
            memcpy(bestCycle, tmpCycle, n * sizeof(int));
            *bestCycleLength = tmpLength;
        }

#ifdef DEBUG
        printf("%.2f ", tmpLength);
#endif

        T *= coolingRate;
    }
#ifdef DEBUG
    printf("\n");
    printf("%d\n", clock());
#endif
}