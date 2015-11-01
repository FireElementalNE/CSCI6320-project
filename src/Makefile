CC=g++
CFLAGS=-g -Wall
all: server.cpp proxy.cpp client.cpp
	$(CC) -o server server.cpp $(CFLAGS)
	$(CC) -o client client.cpp $(CFLAGS)
	$(CC) -o proxy proxy.cpp $(CFLAGS)	
clean:
	rm server client proxy
