#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
using namespace std;
class CryptoServer {
private:
	int port;
	string host;
	struct sockaddr_in server_addr, client_addr;
	socklen_t clientlen;
	int reuse;
  	int server, client;
  	bool debug;
  	bool setup_connection();
  	void process_connection(int sock);
public:
	
	CryptoServer(int p1, string h1, bool d1);
	bool start_server();
};