.PHONY: all clear

all: named_pipe unnamed_pipe

named_pipe: named_pipe.o
	gcc -o named_pipe named_pipe.o

named_pipe.o: named_pipe.c
	gcc -c named_pipe.c

unnamed_pipe: unnamed_pipe.o
	gcc -o unnamed_pipe unnamed_pipe.o

unnamed_pipe.o: unnamed_pipe.c
	gcc -c unnamed_pipe.c

clear:
	rm named_pipe named_pipe.o unnamed_pipe unnamed_pipe.o