#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#define BUF_LEN 4096
#define MAXCONNECTIONS 5
using namespace std;

// https://github.com/zappala/socket-programming-examples-c

int main(int argc, char **argv) {
  struct sockaddr_in server_addr,client_addr;
  socklen_t clientlen = sizeof(client_addr);
  int reuse;
  int server, client;
  char * buf;
  int nread;
  string host = "localhost";
  int port = 5556; 
  bool debug = false;
  int c;
  while ((c = getopt(argc, argv, "dhp:H:")) != -1) {
    switch (c) {
      case 'h':
        cout << "usage: " << argv[0] << " [-d] [-h] [-p port] [-H hostname]" << endl;
        cout << "\t-h help (show this menu)" << endl;
        cout << "\t-d Debug flag" << endl;
        cout << "\t-H hostname to of sever to (DEFAULT: 'localhost')" << endl;
        cout << "\t-p port to listen on to (DEFAULT: 5556)" << endl;
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
    cout << "DEBUG: listning from " << host << " on port " << port << endl;
  }

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
  buf = new char[BUF_LEN + 1];
  // accept clients
  while ((client = accept(server,(struct sockaddr *)&client_addr,&clientlen)) > 0) {
    while (1) {
      memset(buf, 0, BUF_LEN);
      nread = recv(client, buf, BUF_LEN, 0);
      if(debug) {
        char str[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(client_addr.sin_addr), str, INET_ADDRSTRLEN);
        cout << "Got a connection from " << str << endl;
        cout << "DEBUG: got '" << buf << "' from client." << endl;
      }
      if (nread == 0)
        break;
      // send a response
      send(client, buf, BUF_LEN, 0);
      if(debug) {
      cout << "DEBUG: sent '" << buf << "' to client." << endl;
    }
   }
   close(client);
 }
 close(server);
}
