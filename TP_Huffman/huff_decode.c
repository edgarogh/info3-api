#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arbrebin.h"
#include "assert.h"
#include "bfile.h"

void Decoder(FILE *fichier_encode, Arbre ArbreHuffman) {
    BFILE* fic_bin = bstart(fichier_encode, "r");

    Arbre a = ArbreHuffman;
    while (!beof(fic_bin)) {
        int c = bitread(fic_bin);
        if (c == -1) break;
        
        if (c) a = a->fd;
        else a = a->fg;

        if (EstVide(a->fg)) {
            fputc(Etiq(a), stdout);
            a = ArbreHuffman;
        }
    }

    bstop(fic_bin);
}

int main(int argc, char *argv[]) {
    FILE *fichier_encode = fopen(argv[1], "r");

    Arbre ArbreHuffman = LireArbre(fichier_encode);

    Decoder(fichier_encode, ArbreHuffman);
    fflush(stdout);

    fclose(fichier_encode);

    return 0;
}
