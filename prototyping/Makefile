CC=g++
CFLAGS=-lssl -lcrypto -g -Wall -std=c++11
OBJ=server.o client.o crypto.o bank.o gen_functions.o account.o
all: server proxy client bank
sockets: server proxy client
client: client_main.cpp server.o client.o crypto.o bank.o gen_functions.o account.o
	$(CC) -o client client_main.cpp $(OBJ) $(CFLAGS)
server: server_main.cpp server.o client.o crypto.o bank.o gen_functions.o account.o
	$(CC) -o server server_main.cpp $(OBJ) $(CFLAGS)
proxy: proxy.cpp
	$(CC) -o proxy proxy.cpp $(CFLAGS)
server.o:
	$(CC) -c -o server.o server.cpp $(CFLAGS)
client.o:
	$(CC) -c -o client.o client.cpp $(CFLAGS)
gen_functions.o:
	$(CC) -c -o gen_functions.o gen_functions.cpp $(CFLAGS)
crypto.o:
	$(CC) -c -o crypto.o crypto.cpp $(CFLAGS)
bank.o:
	$(CC) -c -o bank.o bank.cpp $(CFLAGS)
account.o: account.cpp
	$(CC) -c -o account.o account.cpp $(CFLAGS)
clean:
	rm -f server client proxy test-ssl sha-test *.o
