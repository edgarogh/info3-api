#include <stdlib.h>
#include <stdio.h>

#include "tris.h"

int main(int argc, char** argv) {
  if (argc <= 1) lancer_mesures();
  else lancer_mesures_gnuplot();

  return 0;
}
