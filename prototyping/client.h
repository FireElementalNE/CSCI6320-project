#include <netdb.h>
#include <stdlib.h>
#include <iostream>
#include <arpa/inet.h>
#include <openssl/rsa.h>
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
	string server_pub_key;
public:
	RSA * create_rsa_pubkey(char * key);
	bool debug;
	bool init_connection();
	CryptoClient(int p1, string h1, bool d1);
	string send_recv_msg(string msg);
	void close_connection();
	void send_encr_msg(unsigned char * msg, int msg_len);
};