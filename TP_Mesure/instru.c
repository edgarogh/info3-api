#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "assert.h"

long cmpTotal = 0;
long totalTries = 0;

int* cmpList = NULL;

void instruInit(int X) {
    cmpTotal = 0;
    totalTries = 0;
    if (cmpList) free(cmpList);
    cmpList = malloc(sizeof(int) * X);
}

int incrCmp(int x) {
    cmpTotal++;
    cmpList[totalTries]++;
    return x;
}

void instruCommit() {
    totalTries++;
}

float instruAvg() {
    return ((float) cmpTotal) / ((float) (totalTries + 1));
}

float instruEcartType() {
    assert(totalTries > 0);

    float avg = instruAvg();
    float esperance = 0;
    for (int i  = 0; i <= totalTries; i++) {
        long x = avg - cmpList[i];
        esperance += x*x;
    }
    esperance /= (totalTries + 1);

    return sqrtf(esperance);
}
