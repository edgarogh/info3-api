
#include "arbrebin.h"
#include "bfile.h"
#include "fap.h"
#include "huffman_code.h"
#include <assert.h>
#include <stdio.h>

typedef struct {
    int tab[256];
} TableOcc_t;

struct code_char HuffmanCode[256];

void ConstruireTableOcc(FILE *fichier, TableOcc_t *TableOcc) {
    for (int i = 0; i < 256; i++) {
        TableOcc->tab[i] = 0;
    }

    int c;
    while ((c = fgetc(fichier)) != EOF) {
        assert(c >= 0 && c < 256);
        TableOcc->tab[c] += 1;
    };

    for (int i = 0; i < 256; i++) {
        if (TableOcc->tab[i] != 0)
            fprintf(stderr, "Occurences du caractere %c (code %d) : %d\n", i, i,
                   TableOcc->tab[i]);
    }
}

fap InitHuffman(TableOcc_t *TableOcc) {
    fap f = creer_fap_vide();

    for (int i = 0; i < 256; i++) {
        f = inserer(f, NouveauNoeud(NULL, i, NULL), TableOcc->tab[i]);
    }

    return f;
}

Arbre ConstruireArbre(fap file) {
    while (1) {
        if (est_fap_vide(file)) return ArbreVide();
        Arbre left;
        int leftPri;
        file = extraire(file, &left, &leftPri);
        if (est_fap_vide(file)) return left;
        Arbre right;
        int rightPri;
        file = extraire(file, &right, &rightPri);

        file = inserer(file, NouveauNoeud(left, 0, right), leftPri + rightPri);
    }
}

static void ConstruireCodeRec(Arbre huff, struct code_char* parent) {
    if (EstVide(huff)) return;

    Arbre left = FilsGauche(huff);
    if (left != NULL) {
        parent->code[parent->lg++] = '0';
        ConstruireCodeRec(left, parent);

        Arbre right = FilsDroit(huff);
        parent->code[parent->lg - 1] = '1';
        ConstruireCodeRec(right, parent);

        parent->lg--;
    } else {
        HuffmanCode[Etiq(huff)] = *parent;
    }
}

void ConstruireCode(Arbre huff) {
    struct code_char base = { .lg = 0 };
    ConstruireCodeRec(huff, &base);
}

void Encoder(FILE *fic_in, FILE *fic_out, Arbre ArbreHuffman) {
    EcrireArbre(fic_out, ArbreHuffman);
    
    BFILE* fic_bin = bstart(fic_out, "w");

    int c;
    while ((c = fgetc(fic_in)) != EOF) {
        struct code_char* code = &HuffmanCode[c];
        for (int i = 0; i < code->lg; i++) {
            bitwrite(fic_bin, code->code[i] == '1');
        }
    }

    bstop(fic_bin);
}

int main(int argc, char *argv[]) {

    TableOcc_t TableOcc;
    FILE *fichier;
    FILE *fichier_encode;

    fichier = fopen(argv[1], "r");
    /* Construire la table d'occurences */
    ConstruireTableOcc(fichier, &TableOcc);
    fclose(fichier);

    /* Initialiser la FAP */
    fap file = InitHuffman(&TableOcc);

    /* Construire l'arbre d'Huffman */
    Arbre ArbreHuffman = ConstruireArbre(file);

#ifdef DEBUG_ARBRE
    AfficherArbre(ArbreHuffman);
#endif

    /* Construire la table de codage */
    ConstruireCode(ArbreHuffman);

    for (int i = 0; i < 256; i++) {
        struct code_char* code = &HuffmanCode[i];
        if (i < 32 || i > 127) fprintf(stderr, "[%d] => ", i);
        else fprintf(stderr, "%c => ", (char) i);
        for (int j = 0; j < code->lg; j++) {
            fprintf(stderr, "%c", code->code[j]);
        }
        fprintf(stderr, "\n");
    }

    /* Encodage */
    fichier = fopen(argv[1], "r");
    fichier_encode = fopen(argv[2], "w");

    Encoder(fichier, fichier_encode, ArbreHuffman);

    fclose(fichier_encode);
    fclose(fichier);

    return 0;
}
