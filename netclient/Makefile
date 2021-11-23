all: client

client : client.o
	gcc -g client.o -o client

client.o : client.c
	gcc -c client.c -o client.o

clean: 
	rm -rf *.o client