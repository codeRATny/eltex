.PHONY: all run clean

all: bin/main

bin/main: build/main.o
		gcc -o bin/main build/main.o

build/main.o: src/main.c
		gcc -c -o build/main.o src/main.c

run:
	./bin/main

clean:
	rm build/* bin/*