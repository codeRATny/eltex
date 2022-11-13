.PHONY: all clean

all: client_broadcast client_multicast server_broadcast server_multicast

client_broadcast: client_broadcast.o
	gcc -o client_broadcast client_broadcast.o -g

client_broadcast.o: client_broadcast.c
	gcc -c client_broadcast.c -g

client_multicast: client_multicast.o
	gcc -o client_multicast client_multicast.o -g

client_multicast.o: client_multicast.c
	gcc -c client_multicast.c -g

server_broadcast: server_broadcast.o
	gcc -o server_broadcast server_broadcast.o -g

server_broadcast.o: server_broadcast.c
	gcc -c server_broadcast.c -g

server_multicast: server_multicast.o
	gcc -o server_multicast server_multicast.o -g

server_multicast.o: server_multicast.c
	gcc -c server_multicast.c -g

clean:
	rm client_broadcast client_broadcast.o client_multicast client_multicast.o server_broadcast server_broadcast.o server_multicast server_multicast.o