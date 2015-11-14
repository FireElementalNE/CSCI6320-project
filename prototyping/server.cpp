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
void print_usage(char * argv0);
void process_connection(int sock, bool debug);
int main(int argc, char **argv) {
  struct sockaddr_in server_addr,client_addr;
  socklen_t clientlen = sizeof(client_addr);
  int reuse = 1;
  int server, client;
  string host = "localhost";
  int port = 5556; 
  bool debug = false;
  int c;
  while ((c = getopt(argc, argv, "dhp:H:")) != -1) {
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
    exit(EXIT_FAILURE);
  }
  // set socket to immediately reuse port when the application closes
  int setsockopt_rc = setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
  if(setsockopt_rc < 0) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }
  // call bind to associate the socket with our local address and
  // port
  int bind_rc = bind(server,(const struct sockaddr *)&server_addr,sizeof(server_addr));
  if(bind_rc < 0) {
    perror("bind");
    exit(EXIT_FAILURE);
  }
  // convert the socket to listen for incoming connections
  int listen_rc = listen(server, MAXCONNECTIONS);
  if(listen_rc < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }
  // accept clients
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
      process_connection(client, debug);
      exit(EXIT_SUCCESS);
    }
  }
  cout << "EXITING PROXY SERVER" << endl;
  close(server);
}
void print_usage(char * argv0) {
  cout << "usage: " << argv0 << " [-d] [-h] [-p port] [-H hostname]" << endl;
  cout << "\t-h help (show this menu)" << endl;
  cout << "\t-d Debug flag" << endl;
  cout << "\t-H hostname to of sever to (DEFAULT: 'localhost')" << endl;
  cout << "\t-p port to listen on to (DEFAULT: 5556)" << endl;
}
void process_connection(int sock, bool debug) {
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