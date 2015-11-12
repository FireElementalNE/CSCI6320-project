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

// Modified from:
// https://github.com/zappala/socket-programming-examples-c

void print_debug(bool debug, string msg);
int main(int argc, char **argv) {
	string host = "localhost";
	int port = 5555; 
	bool debug = false;
	int c;
	while ((c = getopt(argc, argv, "dhp:H:")) != -1) {
		switch (c) {
		case 'h':
			cout << "usage: " << argv[0] << " [-d] [-h] [-p port] [-H hostname]" << endl;
			cout << "\t-h help (show this menu)" << endl;
			cout << "\t-d Debug flag" << endl;
			cout << "\t-H hostname to connect to (DEFAULT: 'localhost')" << endl;
			cout << "\t-p port to connect to (DEFAULT: 5555)" << endl;
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
	if(debug) {
		cout << "DEBUG: connecting to " << host << " on port " << port << endl;
	}

	struct hostent *hostEntry;
	hostEntry = gethostbyname(host.c_str());
	if (!hostEntry) {
		cout << "No such host name: " << host << endl;
		exit(EXIT_FAILURE);
	}

	// setup socket address structure
	struct sockaddr_in server_addr;
	// set the memory to 0
	memset(&server_addr,0 , sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	
	memcpy(&server_addr.sin_addr, hostEntry->h_addr_list[0], hostEntry->h_length);

  	// create socket
	int server = socket(PF_INET,SOCK_STREAM,0);
	if (server < 0) {
		perror("socket");
		exit(-1);
	}

 	// connect to server
	if (connect(server,(const struct sockaddr *)&server_addr,sizeof(server_addr)) < 0) {
		perror("connect");
		exit(EXIT_FAILURE);
	}

  	// allocate buffer
	char * buf = new char[BUF_LEN + 1];

  	// read a line from standard input
	string line;
	while (getline(cin,line)) {

	    // write the data to the server
		send(server, line.c_str(), line.length(), 0);
		if(debug) {
			cout << "DEBUG: sent '" << line << "' to server." << endl;
		}

	    // read the response
		memset(buf,0,BUF_LEN);
		recv(server,buf,BUF_LEN,0);
		
	    // print the response
		if(debug) {
			cout << "DEBUG: got '" << buf << "' from server." << endl;
		}
	}
  	// Close socket
	close(server);
}
void print_debug(bool debug, string msg) {
	if(debug) {
		cout << "\tDEBUG: " << msg << endl;
	}
}