#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <iostream>
#include <locale> 
#include "constants.h"
using namespace std;
char * str_to_char_ptr_safe(string s, int max_len) {
    if(s.size() > (unsigned int)max_len) {
        return NULL;
    }
    char * out = new char[s.size()];
    memcpy(out, s.c_str(), s.size());
    return out;
}
string raw_to_hex(unsigned char * bs, unsigned int n) {
    string ret_val = "";
    for (unsigned int i = 0; i < n; i++) {
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
string to_lower(string s){
    string out = "";
    for(unsigned int i = 0; i < s.size(); i++) {
        out += tolower(s[i]);
    }
    return out;
}