#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include "server.h"
#define BUF_LEN 4096
#define MAXCONNECTIONS 5
CryptoServer::CryptoServer(int p1, string h1, bool d1) {
	port = p1;
	host = h1;
	debug = d1;
	reuse = 1;
	clientlen = sizeof(client_addr);
}
bool CryptoServer::setup_connection() {
	if(debug) {
    	cout << "DEBUG: listning from " << host << " on port " << port << endl;
	}
	memset(&server_addr, 0, sizeof(server_addr));
  	server_addr.sin_family = AF_INET;
  	server_addr.sin_port = htons(port);
  	server_addr.sin_addr.s_addr = INADDR_ANY;
  	server = socket(PF_INET,SOCK_STREAM,0);
  	if (!server) {
    	perror("socket");
    	return false;
  	}
  	int setsockopt_rc = setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
  	if(setsockopt_rc < 0) {
    	perror("setsockopt");
    	return false;
  	}
  	// call bind to associate the socket with our local address and
  	// port
  	int bind_rc = bind(server,(const struct sockaddr *)&server_addr,sizeof(server_addr));
  	if(bind_rc < 0) {
    	perror("bind");
    	return false;
  	}
  	// convert the socket to listen for incoming connections
  	int listen_rc = listen(server, MAXCONNECTIONS);
  	if(listen_rc < 0) {
    	perror("listen");
    	return false;
  	}
  	return true;
}
bool CryptoServer::start_server() {
	bool setup_flag = setup_connection();
	if(!setup_flag) {
		return false;
	}
	while (1) {
		client = accept(server,(struct sockaddr *)&client_addr, &clientlen);
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
      		close(server);
      		process_connection(client);
      		exit(EXIT_SUCCESS);
      	}
      	else {
      		close(client);
      	}
	}
}
void CryptoServer::process_connection(int sock) {
	char * buf = new char[BUF_LEN + 1];
  	while(1) {
    	memset(buf, 0, BUF_LEN);
    	int nread = recv(sock, buf, BUF_LEN, 0);
    	if(nread == 0) {
      		cout << "GOT a read length of 0, closing socket" << endl;
      		close(sock);
      		return;
    	}
    	else {
      		if(debug) {
        		cout << "DEBUG: got '" << buf << "' from client." << endl;
      		}
    	}
    	send(sock, buf, BUF_LEN, 0);
    	if(debug) {
      		cout << "DEBUG: sent '" << buf << "' to client." << endl;
    	}
    }
}