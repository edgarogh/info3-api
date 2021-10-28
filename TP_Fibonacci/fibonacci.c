#include "stdio.h"
#include "time.h"

#include "fibo.h"

#ifdef BENCHMARK
#define FIBOFN fibo_rec
#define FIBORET num
#define FIBO(n) do { num nn = n;clock_t start = clock(); __attribute__((unused)) volatile FIBORET res = FIBOFN(nn); printf("f(%ld) = %ld, dure %lfs\n", nn, res, (double)(clock() - start) / CLOCKS_PER_SEC); } while (0)
#endif

int main() {
    #ifdef BENCHMARK
    FIBO(39);
    FIBO(40);
    // FIBO(75);
    // FIBO(100);
    // FIBO(1000000000);
    return 0;
    #endif

    // Activer le test d'exec > 1s
    char stepGt1 = 1;

    printf("--- test de cohérence entre les diff. algos ---\n");
    printf(
        "fibonacci(10) = { rec = %lld; iter = %lld, rec2 = %lld, golden = %lf\n",
        fibo_rec(10),
        fibo_iter(10),
        fibo_rec2(10),
        fibo_golden(10)
    );

    if (stepGt1) {
        printf("--- 2.2) test d'exécution >= 1s ---\n");
        num n;
        double elapsed;
        for (n = 20;; n++) {
            printf(".");
            fflush(stdout);
            clock_t start = clock();
            __attribute__((unused)) volatile num _res = fibo_rec(n);
            clock_t end = clock();
            elapsed = (double)(end - start) / CLOCKS_PER_SEC;
            if (elapsed >= 1.0) break;
        }
        printf("\nfibo_rec(%lld) prend %lfs à calculer\n", n, elapsed);
        for (n = 1000000000;; n += 10000000) {
            printf(".");
            fflush(stdout);
            clock_t start = clock();
            __attribute__((unused)) volatile num _res = fibo_iter(n);
            clock_t end = clock();
            elapsed = (double)(end - start) / CLOCKS_PER_SEC;
            if (elapsed >= 1.0) break;
        }
        printf("\nfibo_iter(%lld) prend %lfs à calculer\n", n, elapsed);
    }

    printf("\n--- 2.3) qu'est ce qui ne va pas ? (dépassement: fixé) ---\n");
    printf("fibonacci(45) = %lld\n", fibo_iter(45));
    printf("fibonacci(46) = %lld\n", fibo_iter(46));
    printf("fibonacci(47) = %lld\n", fibo_iter(47));
    printf("fibonacci(48) = %lld\n", fibo_iter(48));

    printf("\n--- 2.4) rec2, plus performant ---\n");
    printf("fibonacci(60) = %lld\n", fibo_iter(60));
    printf("fibonacci(75) = %lld\n", fibo_iter(75));
    printf("fibonacci(100) = %lld\n", fibo_iter(100));

    printf("\n--- 2.5) interlude exponentielle ---\n");
    printf("quick_power(5., 3) = %lf\n", puissance(5, 3));
    printf("quick_power(0.5, 4) = %lf\n", puissance(.5, 4));

    printf("\n--- 2.6) golden, O(1) mais imprécis à cause des flottants ---\n");
    printf("fibonacci(60) = %lf\n", fibo_golden(60));
    printf("fibonacci(75) = %lf\n", fibo_golden(75));
    printf("fibonacci(100) = %lf\n", fibo_golden(100));

    printf("\n--- 2.7) matrices ---\n");
    printf("fibonacci(60) = %lld\n", fibo_matrix(60));
    printf("fibonacci(75) = %lld\n", fibo_matrix(75));
    printf("fibonacci(100) = %lld\n", fibo_matrix(100));
}
