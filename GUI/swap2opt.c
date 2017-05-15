#include "swap2opt.h"

void swap2Opt(int n, int* dest, int* source, int al, int a, int b, int br) {
    /*
        little improvement
        memcpy is faster that assigning using for loop
        we should invert using loop smaller part of cycle
        inverting (a;b) is an equal operation on cycle as inverting (br;n-1)U(0;al)
    */
    int halfN = n / 2;
    int braDiff = br - a;
    if (braDiff < halfN) {
        memcpy(dest, source, a * sizeof(int)); //size of (0;al) = (al - 0) + 1 = a,
        for (int i = a, k = b; i <= b; ++i, --k) {
            dest[i] = source[k];
        }
        memcpy(dest + br, source + br, (n - br) * sizeof(int));
    }
    else {
        memcpy(dest + a, source + a, braDiff * sizeof(int));
        for (int i = br, k = al; i != al; --k) {
            dest[i] = source[k];
            if (k <= 0) k = n;
            ++i;
            if (i >= n) i = 0;
        }
        dest[al] = source[br];
    }
}
