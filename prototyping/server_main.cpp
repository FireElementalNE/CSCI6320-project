#include <iostream>
#include "server.h"
using namespace std;
void print_usage(char * argv0);
int main(int argc, char ** argv) {
	string host = "localhost";
	int port = 5556; 
	bool debug = false;
	int c;
	while ((c = getopt(argc, argv, "dhp:t:")) != -1) {
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
		}
	}
	CryptoServer server = CryptoServer(port, host, debug);
	server.start_server();
}
void print_usage(char * argv0) {
  cout << "usage: " << argv0 << " [-d] [-h] [-p port] [-H hostname]" << endl;
  cout << "\t-h help (show this menu)" << endl;
  cout << "\t-d Debug flag" << endl;
  cout << "\t-H hostname to of sever to (DEFAULT: 'localhost')" << endl;
  cout << "\t-p port to listen on to (DEFAULT: 5556)" << endl;
}