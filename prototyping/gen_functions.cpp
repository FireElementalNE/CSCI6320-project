#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <iostream>
#include <locale> 
#include <dirent.h>
#include <regex>
#include <sys/stat.h>
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
// https://techoverflow.net/blog/2013/08/21/how-to-check-if-file-exists-using-stat/
bool file_exists(const string& file) {
    struct stat buf;
    return (stat(file.c_str(), &buf) == 0);
}
vector <string> get_account_names(string account_dir) {
    vector <string> accounts;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(account_dir.c_str())) != NULL) {
        while((ent = readdir (dir)) != NULL) {
            char * tmp_name = new char[MAX_FILENAME_LEN];
            memset(tmp_name, '\0', MAX_FILENAME_LEN * sizeof(char));
            strncpy(tmp_name, ent->d_name, MAX_FILENAME_LEN);
            string f_name(tmp_name);
            regex file_name_regex(ACT_FNAME_REGEX);
            smatch file_name_match;
            regex_search(f_name, file_name_match, file_name_regex);
            if(file_name_match.size() == 2) {
                accounts.push_back(file_name_match[1]);    
            }
        }
        closedir(dir);
    }
    return accounts;
}
bool check_pin(string act, string pin, string a_dir) {
    int p = atoi(pin.c_str());
    string f_name = a_dir + "/" + act + ".card";
    if(!file_exists(f_name)) {
        cerr << "file " << f_name << " does not exist." << endl;
        return false;
    }
    ifstream file;
    file.open(f_name);
    if(!file.is_open()) {
        cerr << "could not open file " << f_name << endl;
        return false;
    }
    string p_str;
    file >> p_str;
    file.close();
    int real_pin = atoi(p_str.c_str());
    return real_pin == p; 
}
bool check_port(int port) {
    if(port < 1025 || port > 65535) {
        return false;
    }
    return true;
}