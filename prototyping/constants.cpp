#include "constants.h"
const int BUF_LEN = 4096;
const int MAXCONNECTIONS = 5;
const int ENC_LEN = 256;
const int MAX_ACT_SIZE = 16;
const int MAX_PIN_SIZE = 8;
const int MAX_AMT_LEN = 16;
const int RAND_PAD_LEN = 4;
const int MAX_PORT_LEN = 5;
const int MAX_FILENAME_LEN = 255; // max len for ext4
const int SOCKET_TIMEOUT = 45; 
const int MAC_MSG_LEN = 64;
const std::string MAC_MSG_REGEX = "^(\\w+):(\\w+)$";
const std::string ACT_FNAME_REGEX = "^(\\w+)\\.act$";
const std::string OK_REGEX_STR = "^OK.$";
const std::string FAIL_REGEX_STR = "^FAILURE.";
const std::string SUCCESS_REGEX_STR = "^SUCCESS.$";
const std::string EXIT_REGEX_STR = "^EXIT.$";
const std::string ACT_REGEX_STR = "^(\\w{1,16}):(\\d{1,8})$";
const std::string TRANS_REGEX_STR_NON_BALANCE_TRANSFER = "^([WD]):(\\d{1,16})$";
const std::string TRANS_REGEX_STR_BALANCE = "^B:$";
const std::string TRANS_REGEX_STR_TRANSFER = "^T:(\\d{1,16}):(\\w{1,16})$";
const std::string LOGIN_REGEX_STR = "^login (\\w{1,16})$";
const std::string WITHDRAW_REGEX_STR = "^withdraw (\\d{1,16})$";
const std::string DEPOSIT_REGEX_STR = "^deposit (\\d{1,16})$";
const std::string TRANSFER_REGEX_STR = "^transfer (\\d{1,16}) (\\w{1,16})$";
const std::string BALANCE_REGEX_STR = "^balance$";
const std::string EXIT_REGEX_STR_CMD = "^exit$";
