#include "stdio.h"
#define N 10
int T[N] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

int cOp = 0;
int cComp = 0;

inline int incrComp(int x) {
    cComp++;
    return x;
}

void f() {
    int x, i, s;
    float m;
    i = 0 ; s = 0 ;
    while (incrComp(i<N)) {
        i = i+1 ;
        cOp++; // instru
        x = T[i] ; 
        if (incrComp(x != -1)) {
            s = s + x ;
            cOp++; // instru
        }
    } ;
    m = s/i ;
    cOp++; // instru
    printf ("%f\n",m);
}

int main() {
    f();
    printf("op=%d cmp=%d", cOp, cComp);
}