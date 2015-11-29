#ifndef GENFUNC_H
#define GENFUNC_H
#include <iostream>
extern char * str_to_char_ptr(std::string s);
extern std::string raw_to_hex(unsigned char * bs, unsigned int n);
extern std::string read_keyfile(std::string filename);
#endif /* GENFUNC_H */
