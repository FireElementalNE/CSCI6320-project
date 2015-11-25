CC=g++
CFLAGS=-g -Wall
all: server proxy client test-ssl sha-test
sockets: server proxy client
crypto: test-ssl sha-test
client: client.o client_main.cpp
	$(CC) -o client client_main.cpp client.o -lssl -lcrypto $(CFLAGS)
server: server.o server_main.cpp
	$(CC) -o server server_main.cpp server.o -lssl -lcrypto $(CFLAGS)
proxy: proxy.cpp
	$(CC) -o proxy proxy.cpp $(CFLAGS)
test-ssl: test-ssl.cpp
	$(CC) -o test-ssl test-ssl.cpp -lssl -lcrypto $(CLFAGS)
sha-test: sha-test.cpp
	$(CC) -o sha-test sha-test.cpp -lssl -lcrypto $(CLFAGS)

clean:
	rm -f server client proxy test-ssl sha-test *.o
