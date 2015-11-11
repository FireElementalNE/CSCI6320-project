#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

#define MAXCONNECTIONS 5
using namespace std;

// https://github.com/zappala/socket-programming-examples-c

int main(int argc, char **argv) {
  struct sockaddr_in server_addr,client_addr;
  socklen_t clientlen = sizeof(client_addr);
  int port, reuse;
  int server, client;
  char * buf;
  int buflen;
  int nread;
  if(argc != 2) {
    cerr << "ERROR: port not provided." << endl;
    exit(EXIT_FAILURE);
  }
  port = atoi(argv[1]);

  // setup socket address structure
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = INADDR_ANY;
  // create socket
  server = socket(PF_INET,SOCK_STREAM,0);
  if (!server) {
    perror("socket");
    exit(-1);
  }
  // set socket to immediately reuse port when the application closes
  reuse = 1;
  if(setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }
  // call bind to associate the socket with our local address and
  // port
  if(bind(server,(const struct sockaddr *)&server_addr,sizeof(server_addr)) < 0) {
    perror("bind");
    exit(EXIT_FAILURE);
  }
  // convert the socket to listen for incoming connections
  if(listen(server, MAXCONNECTIONS) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }
  // allocate buffer
  buflen = 1024;
  buf = new char[buflen+1];
  // accept clients
  while ((client = accept(server,(struct sockaddr *)&client_addr,&clientlen)) > 0) {
    // loop to handle all requests
    while (1) {
      // read a request
      memset(buf, 0, buflen);
      nread = recv(client, buf, buflen, 0);
      if (nread == 0)
	break;
      cout << "Got a connection from " << client_addr.sin_addr.s_addr << endl;
      // send a response
      send(client, buf, nread, 0);
    }
    close(client);
  }
    
  close(server);
}
