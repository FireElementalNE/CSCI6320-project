CC=g++
CFLAGS=-g -Wall
all: server proxy client test-ssl sha-test
sockets: server proxy client
crypto: test-ssl sha-test
server: server.cpp
	$(CC) -o server server.cpp $(CFLAGS)
client: client.cpp
	$(CC) -o client client.cpp $(CFLAGS)
proxy: proxy.cpp
	$(CC) -o proxy proxy.cpp $(CFLAGS)
test-ssl: test-ssl.cpp
	$(CC) -o test-ssl test-ssl.cpp -lssl -lcrypto $(CLFAGS)
sha-test: sha-test.cpp
	$(CC) -o sha-test sha-test.cpp -lssl -lcrypto $(CLFAGS)

clean:
	rm server client proxy test-ssl sha-test
