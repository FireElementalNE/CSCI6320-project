#ifndef SERVER_H
#define SERVER_H
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <openssl/rsa.h>
#include "bank.h"
using namespace std;
class CryptoServer {
private:
	RSA * rsa;
	int port;
	string host;
	struct sockaddr_in server_addr, client_addr;
	socklen_t clientlen;
	int reuse;
	int server, client;
	bool debug;
	string clinet_pub_key;
	string pub_key;
	string priv_key;
	bool is_approved;
	bool setup_connection();
	void process_connection(int sock);
	void send_public_key(int sock);
	string setup_keys();
	Bank bank;
	string accounts_dir;
public:
	string mac_keys_filename;
	string mac_filename;
	void send_response(string response);
	CryptoServer() {}
	CryptoServer(int p1, string h1, bool d1, string filename_pub, string filename_priv, string a_dir, string mcf, string mf);
	bool start_server();
	void start_terminal(int child_pid);
};
#endif /* SERVER_H */