CC=g++
CFLAGS=-lssl -lcrypto -g -Wall
all: server proxy client
sockets: server proxy client
client: client.o client_main.cpp crypto.o gen_functions.o
	$(CC) -o client client_main.cpp client.o crypto.o gen_functions.o $(CFLAGS)
server: server.o server_main.cpp crypto.o gen_functions.o
	$(CC) -o server server_main.cpp server.o crypto.o gen_functions.o $(CFLAGS)
proxy: proxy.cpp
	$(CC) -o proxy proxy.cpp $(CFLAGS)
clean:
	rm -f server client proxy test-ssl sha-test *.o
