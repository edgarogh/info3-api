#!/bin/bash

make huff_decode huff_encode

if (( $# != 1 )); then
    echo "Usage: ./test.sh <fichier>"
    exit 1
fi

ENCODED=$(mktemp)
DECODED=$(mktemp)

./huff_encode "$1" $ENCODED
./huff_decode "$ENCODED" > $DECODED

original_size=$(stat -c %s "$1")
encoded_size=$(stat -c %s "$ENCODED")

if diff $1 $DECODED; then
    echo "Les fichiers sont identiques"
    echo "Fichier original: $original_size octets"
    echo "Fichier compressé: $encoded_size octets"
    if (( $original_size != 0 )); then
        echo "Ratio de compression:" $((100 * $encoded_size / $original_size)) %
    else
        echo "Ratio de compression: ∞ (le fichier original a une taille nulle)"
    fi
fi
