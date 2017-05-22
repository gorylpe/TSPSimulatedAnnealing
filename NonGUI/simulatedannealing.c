#include "simulatedannealing.h"
//#define DEBUG

static inline double prob(double T, double diff){
    return exp(-diff/T);
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
    *tmpLengthPtr = tmpLength;
    return foundNew;
}

int simulatedAnnealing(int n, float** E, int* bestCycle, double* bestCycleLength, double Tstart, int timeLimit){
    int* tmpCycle = malloc(n * sizeof(int));
    int* tmpCycleSecondary = malloc(n * sizeof(int));
    double tmpLength = *bestCycleLength;
    memcpy(tmpCycle, bestCycle, n * sizeof(int));

    /*
     * Temps approximated from plot of cycle length to iteration number,
     * temperature at start was always increasing to value which hesitate
     * around n * average edge length value.
     * For every cooling rate temperatures were the same,
     * i.e. for cooling rate = 0.99995
     * for n = 100
     * starting temperature looked best at temperature after 0 iterations
     * so start T = Tstart * startTempDelta ^ 0
     * and after 200000 iterations so at the temp firstMinTemp * Tstart,
     * cycle length wasn't differ much and calculations were expensive, so
     * we can end there
     * next, for n = 1000 we could see at the plots that
     * starting temperature was hesitating around average cycle length longer,
     * so we start after 50000 iterations, at temperature
     * T = Tstart * startTempDelta ^ 1 (startTempDelta = (coolingrate)^50000
     * and ending after 75000 more iterations than n=100 so after 225000, at 27500 iteration.
     * Temperature then will equals Tstart * firstMinTemp * minTempDelta ^ 1
     * finally for n = 10000, reading the plot we set
     * start T = Tstart * startTempDelta ^ 2
     * Tmin = Tstart * firstMinTemp * minTempDelta ^ 2
     *
     * for cooling rate 0.99999 had 5 more times iterations at every stage of calculating
     * temperature (0.99995)^(1/5) = 0.99999, and corresponding temperatures was the same
     * when looking at the plot
     */
    const double startTempDelta = 0.0821;
    const double minTempDelta = 0.0235;
    const double firstMinTemp = 0.000045389;

    double deltaMin = pow(minTempDelta, log10((double)n) - 2.0) * firstMinTemp;
    double deltaStart = pow(startTempDelta, log10((double)n) - 2.0);
    double T = Tstart * deltaStart;
    double Tmin = Tstart * deltaMin;
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