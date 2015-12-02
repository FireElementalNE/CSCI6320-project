CC=g++
CFLAGS=-g -Wall -std=c++14
CRYPTO_CFLAGS=-lssl -lcrypto 
OBJ=server.o client.o crypto.o bank.o gen_functions.o account.o constants.o
all: server proxy client
client: client_main.cpp $(OBJ)
	$(CC) -o client client_main.cpp $(OBJ) $(CFLAGS) $(CRYPTO_CFLAGS)
server: server_main.cpp $(OBJ)
	$(CC) -o server server_main.cpp $(OBJ) $(CFLAGS) $(CRYPTO_CFLAGS)
proxy: proxy.cpp
	$(CC) -o proxy proxy.cpp $(CFLAGS)
server.o: server.cpp
	$(CC) -c -o server.o server.cpp $(CFLAGS) $(CRYPTO_CFLAGS)
client.o: client.cpp
	$(CC) -c -o client.o client.cpp $(CFLAGS) $(CRYPTO_CFLAGS)
gen_functions.o: gen_functions.cpp 
	$(CC) -c -o gen_functions.o gen_functions.cpp $(CFLAGS)
crypto.o: crypto.cpp
	$(CC) -c -o crypto.o crypto.cpp $(CFLAGS) $(CRYPTO_CFLAGS)
bank.o: bank.cpp
	$(CC) -c -o bank.o bank.cpp $(CFLAGS)
account.o: account.cpp
	$(CC) -c -o account.o account.cpp $(CFLAGS)
constants.o: constants.cpp
	$(CC) -c -o constants.o constants.cpp $(CFLAGS)
clean:
	rm -f server client proxy test-ssl sha-test *.o
