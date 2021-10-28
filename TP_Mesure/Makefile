
# -O2 pour que "inline" fonctionne bien par défaut
CFLAGS= -Wall -O3 -lm -g
CC=gcc

all: mesure_tris

mesure_tris: mesure_tris.o tris.o
	$(CC) $^ -o mesure_tris $(CFLAGS)

tests: tests.o tris.o
	$(CC) $^ -o $@ $(CFLAGS)

clean:
	rm -f mesure_tris tests *.o

plot_seg.txt: mesure_tris
	./mesure_tris > $@
