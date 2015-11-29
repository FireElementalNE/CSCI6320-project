#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <iomanip>
#include <sstream>
#include <fstream>
using namespace std;
#define ENC_LEN 256 // length of encr msg
char * str_to_char_ptr(string s) {
	const char * tmp = new char[s.size()];
   	tmp = s.c_str();
    char * out = new char[s.size()];
    strncpy(out, tmp, s.size());
    return out;
}
string raw_to_hex(unsigned char * bs, unsigned int n) {
    string ret_val = "";
    for (int i = 0; i < n; i++) {
        stringstream ss;
        ss << hex << static_cast<int>(bs[i]);
        ret_val += ss.str();
    }
    return ret_val;
}
string read_keyfile(string filename) {
    ifstream file;
    file.open(filename.c_str());
    if(!file.is_open()) {
        cerr << "could not open key file \'" << filename << "\' " << endl;
        exit(EXIT_FAILURE);
    }
    string str((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();
    return str;
}
