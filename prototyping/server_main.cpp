#include <iostream>
#include <unistd.h>
#include "server.h"
using namespace std;
void print_usage(char * argv0);
int main(int argc, char ** argv) {
	string host = "localhost";
	int port = 5556; 
	bool debug = false;
	string priv_key = "keys/private_server.pem";
	string pub_key = "keys/public_server.pem";
	string account_file = "accounts.txt";
	int c;
	while ((c = getopt(argc, argv, "dhp:t:k:K:a:")) != -1) {
		switch (c) {
			case 'h':
				print_usage(argv[0]);
				exit(EXIT_SUCCESS);
				break;
			case 'p':
				port = atoi(optarg);
				break;
			case 'd':
				debug = true;
				break;
			case 'H':
				host = optarg;
				break;
			case 'k':
				priv_key = optarg;
				break;
			case 'K':
				pub_key = optarg;
				break;
			case 'a':
				account_file = optarg;
				break;
		}
	}
	CryptoServer server = CryptoServer(port, host, debug, pub_key, priv_key, account_file);
	server.start_server();
}
void print_usage(char * argv0) {
  cout << "usage: " << argv0 << " [-d] [-h] [-p port] [-H hostname] [-k private key] [-K public key] [-a accounts]" << endl;
  cout << "\t-h help (show this menu)" << endl;
  cout << "\t-d Debug flag" << endl;
  cout << "\t-H hostname to of sever to (DEFAULT: 'localhost')" << endl;
  cout << "\t-p port to listen on to (DEFAULT: 5556)" << endl;
  cout << "\t-k private key file (DEFAULT: keys/private_server.pem)" << endl;
  cout << "\t-K public key file (DEFAULT: keys/public_server.pem)" << endl;
  cout << "\t-a accounts file (DEFAULT: account.txt)" << endl;
}
