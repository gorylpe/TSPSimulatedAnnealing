#pragma once
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "util.h"
/*
	al = a - 1;
	br = b + 1;
	swap2opt is swapping endpoints of edges a and b in this way:
	(a-1;a) -> (a-1;b)
	(b;b+1) -> (a;b+1)
	a and b are switched so a->b tour has to be reversed
	(...,a-2,a-1,a,a1+1,...,b-1,b,b+1,...) into (...,a-2,a-1,b,b-1,b-2,...,a+2,a+1,a,b+1,b+2,...)

	should be guaranteed br >= 2 and <= n - 1
	al can be equal n-1
*/
void swap2Opt(int n, int* dest, int* source, int al, int a, int b, int br);

//calculate diff in const time
static inline float cycleLen2OptDiff(int* cycle, float** E, int al, int a, int b, int br){
    if(a == b)
        return 0.0;
    if(al == br)
        return 0.0;
    if(al == b)
        return 0.0;
    float diff;
    diff = -(E(cycle[al],cycle[a]) + E(cycle[b],cycle[br]));
    diff += E(cycle[al],cycle[b]) + E(cycle[a],cycle[br]);
    /*printf("(%d;%d) -> (%d;%d)", cycle[al], cycle[a], cycle[b], cycle[br]);
    printf("diff: -%f-%f+%f+%f=%f\n", E(cycle[al],cycle[a]), E(cycle[b],cycle[br]), E(cycle[al],cycle[b]), E(cycle[a],cycle[br]), diff);*/
    return diff;
}