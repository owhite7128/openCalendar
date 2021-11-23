all: server

server : server.o
	gcc -g server.o -o server

server.o : server.c
	gcc -c server.c -o server.o

clean: 
	rm -rf *.o server