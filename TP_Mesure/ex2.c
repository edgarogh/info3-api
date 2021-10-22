#include "stdio.h"

int g(int x);
int g1(int x);
int g2(int x);

int cmp = 0;

inline int incrCmp(int x) {
    cmp++;
    return x;
}

int main() {
    double x, y ;
    scanf ("%f %f", &x, &y) ;
    while (x != 0) {
        if ((g(x)>0) && incrCmp(x != y)) { x = g1(x) ; } else { y = g2(y) ; }
    } ;
    printf ("%lf\n",x);
    return 0;
}
