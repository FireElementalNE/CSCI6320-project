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

using namespace std;

// Modified from:
// https://github.com/zappala/socket-programming-examples-c

int
main(int argc, char **argv) {
  
  string host = "localhost";
  if(argc != 2) {
    cerr << "ERROR: port not provided." << endl;
    exit(EXIT_FAILURE);
  }
  int port = atoi(argv[1]);
  // use DNS to get IP address
  struct hostent *hostEntry;
  hostEntry = gethostbyname(host.c_str());
  if (!hostEntry) {
    cout << "No such host name: " << host << endl;
    exit(EXIT_FAILURE);
  }
  // setup socket address structure
  struct sockaddr_in server_addr;
  memset(&server_addr,0,sizeof(server_addr));
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
  int buflen = 1024;
  char* buf = new char[buflen+1];

  // read a line from standard input
  string line;
  while (getline(cin,line)) {

    // write the data to the server
    send(server, line.c_str(), line.length(), 0);

    // read the response
    memset(buf,0,buflen);
    recv(server,buf,buflen,0);
        
    // print the response
    cout << buf << endl;
  }

  // Close socket
  close(server);
}
