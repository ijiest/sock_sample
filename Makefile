CFLAGS=-Wall
CC=g++


all: server client

server: server.cpp
	$(CC) $(CFLAGS) -o server server.cpp

client: client.cpp
	$(CC) $(CFLAGS) -o client client.cpp

.PHONY: clean
clean:
	$(RM) server
	$(RM) client
