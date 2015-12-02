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
#include <vector>
#include <mutex>
#include "crypto.h"
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
        cerr << "could not open key file \'" << filename << "\' " << "." << endl;
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
        cerr << "could not open file " << f_name << "." << endl;
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
string make_mac(string filename) {
    string mac_key;
    ifstream file;
    file.open(filename);
    if(!file.is_open()) {
        cerr << "could not open file " <<  filename << "." << endl;
        exit(EXIT_FAILURE);
    }
    file >> mac_key;
    file.close();
    string rand_msg = make_random_msg();
    string final = rand_msg + ":" + get_mac(rand_msg, mac_key);
    return final;
}
bool check_mac(string filename, string mac_msg) {
    vector <string> mac_keys;
    ifstream file;
    file.open(filename);
    if(!file.is_open()) {
        cerr << "could not open file " <<  filename << "." << endl;
        exit(EXIT_FAILURE);
    }
    string tmp_key;
    while(file >> tmp_key) {
        mac_keys.push_back(tmp_key);
    }
    for(unsigned int i = 0; i < mac_keys.size(); i++) {
        regex mac_rgx(MAC_MSG_REGEX);
        smatch mac_rgx_match;
        regex_search(mac_msg, mac_rgx_match, mac_rgx);
        if(mac_rgx_match.size() != 3) {
            cerr << "your trusted mac file may be malformed." << endl;
            continue;
        }
        string msg = mac_rgx_match[1];
        string mac = mac_rgx_match[2];
        string tmp_msg = get_mac(msg, mac_keys[i]);
        if(tmp_msg == mac) {
            return true;
        }    
    }
    return false;
}
bool is_account_unlocked(string filename, string account_name) {
    vector <pair <string, int> > lib;
    ifstream in_file;
    ofstream out_file;
    static mutex MUTEX;
    lock_guard<mutex> lock(MUTEX);
    in_file.open(filename);
    if(!in_file.is_open() && file_exists(filename)) {
        cerr << "could not open file " << filename << "." << endl;
        return false;
    }
    if(!file_exists(filename)) {
        in_file.close();
    }
    else {
        string account;
        string lock_num_str;
        while(in_file >> account >> lock_num_str) {
            int lock_num = atoi(lock_num_str.c_str());
            if(account_name == account) {
                if(lock_num >= LOCKOUT_ATTEMPTS) {
                    return false;
                }
                else {
                    lock_num++;
                }
            }
            pair <string, int> tmp = make_pair(account, lock_num);
            lib.push_back(tmp);
        }
        in_file.close();
        remove(filename.c_str());
    }
    out_file.open(filename);
    if(!out_file.is_open()) {
        cerr << "could not open file " << filename << "." << endl;
        return false;
    }
    if(file_exists(filename)) {
        bool found_account = false;
        for(unsigned int i = 0; i < lib.size(); i++) {
            if(lib[i].first == account_name) {
                found_account = true;
            }
            out_file << lib[i].first << " " << lib[i].second << endl;
        }
        if(!found_account) {
            out_file << account_name << " " << 1 << endl;
        }
        out_file.close();
        return true;
    }
    else {
        out_file << account_name << " " << 1 << endl;
        out_file.close();
        return true;
    }
}