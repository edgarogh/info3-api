#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "tris.h"

int main(int argc, char** argv) {
  bool interactive = false;

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-i") == 0) interactive = true;
  }

  if (interactive) lancer_mesures();
  else lancer_mesures_gnuplot();

  return 0;
}
