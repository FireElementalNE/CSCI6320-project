#include "constants.h"
#include <regex>
#include <iostream>
const int BUF_LEN = 4096;
const int MAXCONNECTIONS = 5;
const int ENC_LEN = 256;
const int MAX_ACT_SIZE = 16;
const int MAX_PIN_SIZE = 8;
const int MAX_AMT_LEN = 16;
const int RAND_PAD_LEN = 4;
// TODO: use random numbers with server msgs
const std::string OK_REGEX_STR = "^OK.$";
const std::string FAIL_REGEX_STR = "^FAILURE.";
const std::string SUCCESS_REGEX_STR = "^SUCCESS.$";
const std::string EXIT_REGEX_STR = "^EXIT.$";
// END TODO
const std::string ACT_REGEX_STR = "^(\\d{1,16}):(\\d{1,8})$";
const std::string TRANS_REGEX_STR_NON_BALANCE_TRANSFER = "^([WD]):(\\d{1,16})$";
const std::string TRANS_REGEX_STR_BALANCE = "^B:$";
const std::string TRANS_REGEX_STR_TRANSFER = "^T:(\\d{1,16}):(\\d{1,16})$";
const std::string LOGIN_REGEX_STR = "^login (\\d{1,16})$";
const std::string WITHDRAW_REGEX_STR = "^withdraw (\\d{1,16})$";
const std::string DEPOSIT_REGEX_STR = "^deposit (\\d{1,16})$";
const std::string TRANSFER_REGEX_STR = "^transfer (\\d{1,16}) (\\d{1,16})$";
const std::string BALANCE_REGEX_STR = "^balance$";
const std::string EXIT_REGEX_STR_CMD = "^exit$";
