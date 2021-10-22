#include "fibo.h"

#define SQRT5 (2.23606797749979)
#define PHI ((1. + SQRT5) / 2.)
#define DELTA ((1. - SQRT5) / 2.)

num fibo_rec(num n) {
    if (n <= 1) return n;
    else return fibo_rec(n - 2) + fibo_rec(n - 1);
}

num fibo_iter(num n) {
    num a = 0;
    num b = 1;

    while (n--) {
        num tmp = a + b;
        a = b;
        b = tmp;
    }

    return a;
}

static num fibo_rec2_aux(const num nm1, const num nm2, const int n) {
    if (n == 0) return nm1;
    return fibo_rec2_aux(nm2, nm2 + nm1, n - 1);
}

num fibo_rec2(const num n) {
    return fibo_rec2_aux(0, 1, n);
}

double puissance(double x, num n) {
    if (n == 1) return x;
    else if (n & 1) return x * puissance(x*x, (n - 1) / 2);
    else return puissance(x*x, n / 2);
}

double fibo_golden(num n) {
    return (1./SQRT5) * (puissance(PHI, n) - puissance(DELTA, n));
}

static matrix22 matrix22_mul(matrix22 a, matrix22 b) {
    return (matrix22) {
        a.a * a.a + a.b * b.c, a.a * b.b + a.b * b.d,
        a.c * b.a + a.d * b.c, a.c * b.b + a.d * b.d,
    };
}

static matrix22 matrix22_puissance(matrix22 m, num n) {
    if (n == 1) return m;
    else if (n & 1) return matrix22_mul(m, matrix22_puissance(matrix22_mul(m, m), (n - 1) / 2));
    else return matrix22_puissance(matrix22_mul(m, m), n / 2);
}

num fibo_matrix(num n) {
    static const matrix22 FIBO_MATRIX = { 0, 1, 1, 1 };
    matrix22 res = matrix22_puissance(FIBO_MATRIX, n - 1);
    return res.d;
}
