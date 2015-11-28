#include <iostream>
#include <string.h>
#include <iomanip>
#include <sstream>
using namespace std;
int ENC_LEN = 256;
char * str_to_unsigned_char_ptr(string s) {
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