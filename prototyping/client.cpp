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
#include "client.h"
#define BUF_LEN 4096
using namespace std;
CryptoClient::CryptoClient(int p1, string h1, bool d1) {
	port = p1;
	host = h1;
	debug = d1;
}
bool CryptoClient::init_connection() {
	if(debug) {
		cout << "DEBUG: connecting to " << host << " on port " << port << endl;
	}
	hostEntry = gethostbyname(host.c_str());
	if (!hostEntry) {
		cerr << "No such host name: " << host << endl;
		return false;
	}
	memset(&server_addr,0 , sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	memcpy(&server_addr.sin_addr, hostEntry->h_addr_list[0], hostEntry->h_length);
  	// create socket
	server = socket(PF_INET,SOCK_STREAM,0);
	if (server < 0) {
		perror("socket");
		return false;
	}
	int connect_rc = connect(server,(const struct sockaddr *)&server_addr,sizeof(server_addr));
 	// connect to server
	if (connect_rc < 0) {
		perror("connect");
		return false;
	}
	return true;
}
string CryptoClient::send_recv_msg(string msg) {
	char * buf = new char[BUF_LEN + 1];
	if(msg.size() > 0) {
	    // write the data to the server
		send(server, msg.c_str(), msg.length(), 0);
		if(debug) {
			cout << "DEBUG: sent '" << msg << "' to server." << endl;
		}
	    // read the response
		memset(buf,0,BUF_LEN);
		recv(server,buf,BUF_LEN,0);
	    // print the response
		if(debug) {
			cout << "DEBUG: got '" << buf << "' from server." << endl;
		}
		return (string)buf;
	}
	else {
		cerr << "ERROR: empty line." << endl;
		return "";
	}
}
void CryptoClient::close_connection() {
	close(server);
}