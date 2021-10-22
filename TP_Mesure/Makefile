
# -O2 pour que "inline" fonctionne bien par défaut
CFLAGS= -Wall -Werror -O1 -lm -g
CC=gcc

all: mesure_tris

mesure_tris: mesure_tris.o tris.o instru.o
	$(CC) $(CFLAGS) $^ -o mesure_tris

clean:
	rm -f mesure_tris *.o plot.txt
