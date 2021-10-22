#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "instru.h"

#define NMAX 50000		/* peut etre modifie si necessaire */



/*
generation_aleatoire
Donnees : t : tableau d'entiers de taille > n, n : entier > 0
Resultat : les valeurs t[0..n-1] ont ete initialisees avec n valeurs aleatoires
           prises dans [0,2147483647] selon une loi uniforme.
Pre-condition : le generateur aleatoire doit avoir ete initialise avec "srand"
*/

void generation_aleatoire(int t[], int n) {
  volatile int i ;

  for (i=0 ; i<n ; i++) {
	  t[i] = rand() ;
  } ;
}

/*
generation_aleatoire_non_uniforme
Donnees : t : tableau d'entiers de taille > n, n : entier > 0
Resultat : les valeurs t[0..n-1] ont ete initialisees avec n valeurs aleatoires
           prises dans [0,2147483647].
Pre-condition : le generateur aleatoire doit avoir ete initialise avec "srand"
*/
void generation_aleatoire_non_uniforme(int t[], int n) {
  int i;
  int x = 0;

  for (i=0; i < n; i++) {
    t[i] = x;
    x = x + ((rand() % 10) - 2);
  }
}

/*
tri_insertion
Donnees : t : tableau d'entiers de taille > n, n : entier > 0
Resultat : le tableau t est trie en ordre croissant
*/
void tri_insertion(int t[], int n) {
  int i,j;
  int Clef;

  for(i=1; i < n; i++) {
    Clef = t[i];
    
    j = i - 1;
    
    /* Decalage des valeurs du tableau */
    while((j >= 0) && incrCmp(t[j] > Clef)) {
      t[j+1] = t[j];
      j = j - 1;
    }
    
    /* insertion de la clef */
    t[j+1] = Clef;
  }
}

void echanger(int* a, int* b) {
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

int partitionner(int t[], int n, int pivot) {
  echanger(&t[pivot], &t[n - 1]);
  int j = 0;
  for (int i = 0; i < n; i++) {
    if (t[i] <= t[n - 1]) {
      echanger(&t[i], &t[j]);
      j++;
    }
  }
  echanger(&t[n - 1], &t[j]);
  return j;
}

/*
tri_segmentation
Donnees : t : tableau d'entiers de taille > n, n : entier > 0
Resultat : le tableau t est trie en ordre croissant
*/
void tri_segmentation(int t[], int n) {
  int pivot = 0;
  pivot = partitionner(t, n, pivot);
  tri_segmentation(t, pivot - 1);
  tri_segmentation(&t[pivot + 1], n - pivot);
}

typedef struct {
  int N;
  int X;
} config;

#define N_CAS 11
config CAS[N_CAS] = {
  { .N = 1, .X = 1 },
  { .N = 5000, .X = 100 },
  { .N = 10000, .X = 10 },
  { .N = 15000, .X = 9 },
  { .N = 20000, .X = 8 },
  { .N = 25000, .X = 7 },
  { .N = 30000, .X = 6 },
  { .N = 35000, .X = 5 },
  { .N = 40000, .X = 4 },
  { .N = 45000, .X = 3 },
  { .N = 50000, .X = 3 },
};

void mesure_tri(int X, int N) {
  static int T[NMAX];

  instruInit(X);
  for (int attempt = 0; attempt < X; attempt++) {
    generation_aleatoire(T, N) ; /* initialisation du tableau T */
    tri_insertion(T, N) ;	/* tri de T */
    instruCommit();
  }
}

void lancer_mesures() {
  unsigned int germe ;
  int X;
  int N;
  int res;

  printf("Valeur du germe pour la fonction de tirage aleatoire ? ") ;
  res=scanf("%d", &germe) ;
  assert(res==1);
  srand(germe) ;

  printf("Valeur de X ? ");
  res = scanf("%d", &X);
  assert(res == 1);

  do {
  	printf("Valeur de N ? ") ;
  	res=scanf("%d", &N) ;
	assert(res==1);
  } while (N<1 || N > NMAX) ;

  mesure_tri(X, N);

  float avg = instruAvg();
  float et = instruEcartType();
  float ic = 0.95 * et / sqrt(N);
  printf("moyenne des comparaisons = %f\necart-type = %f\nIC = [%f; %f]\n? = %.2f\n", avg, et, avg - ic, avg + ic, ic / avg * 100);
}

void lancer_mesures_gnuplot() {
  srand(23);
  for (int i = 0; i < N_CAS; i++) {
    fprintf(stderr, "[gnuplot output] i=%d\n", i);
    config cas = CAS[i];
    mesure_tri(cas.X, cas.N);

    float avg = instruAvg();
    float et = instruEcartType();
    float ic = 0.95 * et / sqrt(cas.N);

    printf("%d %f %f %f\n", cas.N, instruAvg(), avg - ic, avg + ic);
  }
}
