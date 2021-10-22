CCFLAGS=-O3 -Wall

all: fibonacci

.PHONY: clean run

fibo.o: fibo.c
	$(CC) -c $^ -o $@ $(CCFLAGS)

fibonacci.o: fibonacci.c
	$(CC) -c $^ -o $@ $(CCFLAGS)

fibonacci: fibo.o fibonacci.o
	$(CC) $^ -o $@ $(CCFLAGS)

fibonacci-bm.o: fibonacci.c
	$(CC) -c -DBENCHMARK $^ -o $@ $(CCFLAGS)

fibonacci-bm: fibo.o fibonacci-bm.o
	$(CC) $^ -o $@ $(CCFLAGS)

clean:
	rm fibonacci fibonacci-bm *.o

run: fibonacci
	./$<