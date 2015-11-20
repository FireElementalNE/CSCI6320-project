#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/sha.h>
#include <iomanip>
#include <sstream>
#define MAX_SIZE 256
using namespace std; 
// https://memset.wordpress.com/2010/10/06/using-sha1-function/
// http://stackoverflow.com/questions/11765522/how-do-i-write-this-in-cout-c
string print_hex(unsigned char *bs, unsigned int n) {
    string ret_val = "";
    for (int i = 0; i < n; i++) {
        stringstream ss;
        ss << hex << static_cast<int>(bs[i]);
        ret_val += ss.str();
    }
    return ret_val;
}
int main(int argc, char ** argv) {
    if ( argc != 2 ) {
        cerr << "Usage: " << argv[0] << " <string>" << endl;
        exit(EXIT_FAILURE);
    }
    char * tmp = new char[MAX_SIZE];
    unsigned char temp[SHA512_DIGEST_LENGTH];
 	strncpy(tmp, argv[1], MAX_SIZE);
    memset(temp, 0x0, SHA512_DIGEST_LENGTH);
    SHA512((unsigned char *)tmp, strlen(tmp), temp);
    cout << "SHA512 of " << tmp << " is " << print_hex(temp, sizeof(temp)) << endl;
    return 0;
}
