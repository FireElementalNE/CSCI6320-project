#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#define BUF_LEN 4096
using namespace std;
// https://github.com/zappala/socket-programming-examples-c
class CryptoClient {
private: 
	int port;
	string host;
	bool connected;
	struct hostent * hostEntry;
	struct sockaddr_in server_addr;
	int server;
public:
	bool debug;
	bool init_connection();
	CryptoClient(int p1, string h1, bool d1);
	string send_recv_msg(string msg);
	void close_connection();
};