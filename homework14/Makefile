.PHONY: all clean

all: client_UDP client_TCP server

client_UDP: client_UDP.o
	gcc -o client_UDP client_UDP.o -g

client_UDP.o: client_UDP.c
	gcc -c client_UDP.c -g

client_TCP: client_TCP.o
	gcc -o client_TCP client_TCP.o -g

client_TCP.o: client_TCP.c
	gcc -c client_TCP.c -g

server: server.o
	gcc -o server server.o -g

server.o: server.c
	gcc -c server.c -g

clean:
	rm client_TCP client_TCP.o client_UDP client_UDP.o server server.o