.PHONY: all clean

all: client server

client: client.o
	gcc -o client client.o -g

client.o: client.c
	gcc -c client.c -g

server: server.o
	gcc -o server server.o -g

server.o: server.c
	gcc -c server.c -g

clean:
	rm client client.o server server.o