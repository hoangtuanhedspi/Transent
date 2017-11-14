CC=gcc
CFLAGS=-w -std=c99

all: server client

server: server.c session.c mypoll.c
	$(CC) -o $@ $^ $(CFLAGS)

client: client.c directory.c
	$(CC) -o $@ $^ $(CFLAGS)

clean: *
	rm -rf client server *.o