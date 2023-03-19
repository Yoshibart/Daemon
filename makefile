CC=gcc
objects = server_daemon.o client_daemon.o

client : client_daemon.o
	$(CC) -o client client_daemon.o

server : server_daemon.o
	$(CC) -o server server_daemon.o -lpthread

client_daemon.o : client_daemon.c
	$(CC) -c client_daemon.c

server_daemon.o : server_daemon.c
	$(CC) -c server_daemon.c
clean:
	rm $(objects) client server



