#ifndef CLIENT_H
#define CLIENT_H
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
	string priv_key;
	string pub_key;
	int server;
	string server_pub_key;
	void get_public_key();
	// string decr_msg(unsigned char * msg);
public:
	bool debug;
	bool init_connection();
	CryptoClient(int p1, string h1, bool d1, string filename_pub, string filename_priv);
	string send_recv_msg(string msg);
	void close_connection();
	// void send_encr_msg(unsigned char * msg, int msg_len);
};
#endif /* CLIENT_H */