typedef unsigned long long num;

typedef struct {
    num a, b, c, d;
} matrix22;

num fibo_rec(num n);
num fibo_iter(num n);
num fibo_rec2(const num n);
double puissance(double x, num n);
double fibo_golden(num n);
num fibo_matrix(num n);
