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
#define MAXCONNECTIONS 5
#define BUF_LEN 4096
#define TRUE 1
#define FALSE 0
using namespace std;
void process_connection(int sock_client, string server_host, int server_port, bool debug);
void print_usage(char * argv0);
int main(int argc, char ** argv) {
	struct sockaddr_in proxy_addr, client_addr;
	socklen_t clientlen = sizeof(proxy_addr);
	int reuse = 1;
	int proxy, client;
	string proxy_host = "localhost";
	string server_host = "localhost";
	int proxy_port = 5555; 
	int server_port = 5556;

	bool debug = false;
	int c;
	while ((c = getopt(argc, argv, "dhp:H:t:T:P:")) != -1) {
		switch (c) {
		case 'h':
			print_usage(argv[0]);
			exit(EXIT_SUCCESS);
		case 'p':
			proxy_port = atoi(optarg);
			break;
		case 'P':
			server_port = atoi(optarg);
			break;
		case 'd':
			debug = true;
			break;
		case 't':
			proxy_host = optarg;
			break;
		case 'T':
			server_host = optarg;
			break;
		}
	}
	if(debug) {
		cout << "DEBUG: Proxy host  -> " << proxy_host << endl;
		cout << "DEBUG: Proxy port  -> " << proxy_port << endl;
		cout << "DEBUG: Server host -> " << server_host << endl;
		cout << "DEBUG: Server port -> " << server_port << endl;
	}
  	// setup socket address structure
	memset(&proxy_addr, 0, sizeof(proxy_addr));
	proxy_addr.sin_family = AF_INET;
	proxy_addr.sin_port = htons(proxy_port);
	proxy_addr.sin_addr.s_addr = INADDR_ANY;
  	// create socket
	proxy = socket(PF_INET,SOCK_STREAM,0);
	if (!proxy) {
		perror("socket");
		exit(EXIT_FAILURE);
	}
  	// set socket to immediately reuse port when the application closes
  	int setsockopt_rc = setsockopt(proxy, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
  	int bind_rc = bind(proxy,(const struct sockaddr *)&proxy_addr,sizeof(proxy_addr));
  	int listen_rc = listen(proxy, MAXCONNECTIONS);
	if(setsockopt_rc < 0) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
  	// call bind to associate the socket with our local address and port
	if(bind_rc < 0) {
		perror("bind");
		exit(EXIT_FAILURE);
	}
  	// convert the socket to listen for incoming connections
	if(listen_rc < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
  	// accept clients
	// loop to handle all requests
	while (1) {
  		// read a request
  		client = accept(proxy,(struct sockaddr *)&client_addr,&clientlen);
		if(client < 0) {
			perror("accept");
			exit(EXIT_FAILURE);
		}
		if(debug) {
			char str[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, &(client_addr.sin_addr), str, INET_ADDRSTRLEN);
			cout << "Got a connection from " << str << endl;
		}
		int pid = fork();
	    if(pid < 0) {
	      perror("fork");
	      exit(EXIT_FAILURE);
	    }
	    if(pid == 0) {
	      close(proxy);
	      process_connection(client, server_host, server_port, debug);
	      exit(EXIT_SUCCESS);
	    }
	}
  	cout << "EXITING PROXY SERVER" << endl;
  	close(proxy);
}
void process_connection(int sock_client, string server_host, int server_port, bool debug) {
	char * buf = new char[BUF_LEN+1];
	struct sockaddr_in server_addr;
	struct hostent * server_hostent;
	server_hostent = gethostbyname(server_host.c_str());
	if (!server_hostent) {
		cerr << "No such host name: " << server_host << endl;
		exit(EXIT_FAILURE);
	}
	memset(&server_addr, 0 , sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(server_port);
	memcpy(&server_addr.sin_addr, server_hostent->h_addr_list[0], server_hostent->h_length);
	int server = socket(PF_INET,SOCK_STREAM,0);
	if (server < 0) {
		perror("socket");
		exit(EXIT_FAILURE);
	}
 	// connect to server
 	int connect_rc = connect(server,(const struct sockaddr *)&server_addr,sizeof(server_addr));
	if ( connect_rc < 0) {
		perror("connect");
		exit(EXIT_FAILURE);
	}
	while(1) {
		memset(buf, 0, BUF_LEN);
		int nread = recv(sock_client, buf, BUF_LEN, 0);
		if (nread == 0) {
			cout << "GOT a read length of 0, closing socket" << endl;
			break;
		}
		else {
			if(debug) {
				cout << "DEBUG: got '" << buf << "' from client." << endl;
			}
		}
		// send a response
		send(server, buf, nread, 0);
		if(debug) {
			cout << "DEBUG: sent '" << buf << "' to server." << endl;
		}
		// read the response
		memset(buf,0,BUF_LEN);
		recv(server,buf, BUF_LEN,0);
		if(debug) {
			cout << "DEBUG: got '" << buf << "' from server." << endl;
		}
		send(sock_client, buf, BUF_LEN, 0);
		if(debug) {
			cout << "DEBUG: sent '" << buf << "' to client." << endl;
		}
	}
	close(server);
}
void print_usage(char * argv0) {
	cout << "usage: " << argv0 << " [-d] [-h] [-p port] [-H hostname]" << endl;
	cout << "\t-h help (show this menu)" << endl;
	cout << "\t-d Debug flag" << endl;
	cout << "\t-t hostname of proxy (DEFAULT: 'localhost')" << endl;
	cout << "\t-T hostname of server (DEFAULT: 'localhost')" << endl;
	cout << "\t-p port for proxy (DEFAULT: 5556)" << endl;
	cout << "\t-P port for server (DEFAULT: 5555)" << endl;
}