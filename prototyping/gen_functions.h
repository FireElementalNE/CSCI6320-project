#ifndef GENFUNC_H
#define GENFUNC_H
#include <iostream>
#include <vector>
extern std::string raw_to_hex(unsigned char * bs, unsigned int n);
extern std::string read_keyfile(std::string filename);
extern char * str_to_char_ptr_safe(std::string s, int max_len);
extern std::string to_lower(std::string s);
extern bool file_exists(const std::string& file);
extern std::vector <std::string> get_account_names(std::string account_dir);
extern bool check_pin(std::string a_dir, std::string act, std::string pin);
extern bool check_port(int port);
#endif /* GENFUNC_H */
