#include <iostream>
#include <unistd.h>
#include <cstring>
#include <fstream>
#include "server.h"
#include "gen_functions.h"
#include "constants.h"
using namespace std;
void print_usage(char * argv0);
int main(int argc, char ** argv) {
	if(argc < 2) {
		print_usage(argv[0]);
		exit(EXIT_SUCCESS);
	}
	string host = "localhost";
	char * port_safe = new char[MAX_PORT_LEN];
	memset(port_safe, '\0', MAX_PORT_LEN * sizeof(char));
	strncpy(port_safe, argv[1], MAX_PORT_LEN);
	int port = atoi(port_safe); 
	if(!check_port(port)) {
		cerr << "invalid port." << endl;
		print_usage(argv[0]);
		exit(EXIT_FAILURE);
	}
	bool debug = false;
	string priv_key = "keys/private_server.pem";
	string pub_key = "keys/public_server.pem";
	string accounts_dir = "./accounts_bank";
	string mac_keys = "./mac_keys_server.trusted";
	string mac_key = "./server.mac_key";
	int c;
	while ((c = getopt(argc, argv, "dht:k:K:a:m:M:")) != -1) {
		switch (c) {
			case 'h':
				print_usage(argv[0]);
				exit(EXIT_SUCCESS);
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
				accounts_dir = optarg;
				break;
			case 'm':
				mac_keys = optarg;
				break;
			case 'M':
				mac_key = optarg;
				break;
		}
	}
	CryptoServer server = CryptoServer(port, host, debug, pub_key, priv_key, accounts_dir, mac_key, mac_keys);
	int pid = fork();
	if(pid < 0) {
    		perror("fork");
    		exit(EXIT_FAILURE);
  	}
  	if(pid == 0) {
  		ofstream log_file("log_file.log");
	    // streambuf * cerrbuf = cerr.rdbuf(); //save old buf
	    cerr.rdbuf(log_file.rdbuf()); //redirect std::cin to in.txt!
	    // streambuf * coutbuf = cout.rdbuf(); //save old buf
	    cout.rdbuf(log_file.rdbuf()); //redirect std::cout to out.txt!
    	server.start_server();
  	}
  	else {
  		server.start_terminal(pid);
  	}
	
}
void print_usage(char * argv0) {
  cout << "usage: " << argv0 << " <port> [-d] [-h] [-H hostname] [-k private key] [-K public key] [-a accounts_dir] [-m mac_keys] [-M mac_key]" << endl;
  cout << "\t <port> the port to listen on (REQUIRED)" << endl;
  cout << "\t-h help (show this menu)" << endl;
  cout << "\t-d Debug flag" << endl;
  cout << "\t-H hostname to of sever to (DEFAULT: 'localhost')" << endl;
  cout << "\t-k private key file (DEFAULT: keys/private_server.pem)" << endl;
  cout << "\t-K public key file (DEFAULT: keys/public_server.pem)" << endl;
  cout << "\t-a accounts directory (DEFAULT: ./accounts)" << endl;
  cout << "\t-m mac_keys the  trusted mac keys file (DEFAULT: mac_keys_server.trusted)" << endl;
  cout << "\t-M mac_key the my mac key file (DEFAULT: server.mac_key)" << endl;
}
