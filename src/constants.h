#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <iostream>
extern const int BUF_LEN;
extern const int MAXCONNECTIONS;
extern const int ENC_LEN;
extern const int MAX_ACT_SIZE;
extern const int MAX_PIN_SIZE;
extern const int MIN_PIN_SIZE;
extern const int MAX_AMT_LEN;
extern const int RAND_PAD_LEN;
extern const int MAX_FILENAME_LEN;
extern const int MAX_PORT_LEN;
extern const int SOCKET_TIMEOUT;
extern const int MAC_MSG_LEN;
extern const std::string CLIENT_LOCKOUT_FILE;
extern const std::string SERVER_LOCKOUT_FILE;
extern const int LOCKOUT_ATTEMPTS;
extern const std::string TERM_DEPOSIT_REGEX;
extern const std::string TERM_BALANCE_REGEX;
extern const std::string MAC_MSG_REGEX;
extern const std::string ACT_FNAME_REGEX;
extern const std::string OK_REGEX_STR;
extern const std::string FAIL_REGEX_STR;
extern const std::string SUCCESS_REGEX_STR;
extern const std::string EXIT_REGEX_STR;
extern const std::string ACT_REGEX_STR;
extern const std::string TRANS_REGEX_STR_NON_BALANCE_TRANSFER;
extern const std::string TRANS_REGEX_STR_BALANCE;
extern const std::string TRANS_REGEX_STR_TRANSFER;
extern const std::string LOGIN_REGEX_STR;
extern const std::string WITHDRAW_REGEX_STR;
extern const std::string DEPOSIT_REGEX_STR;
extern const std::string TRANSFER_REGEX_STR;
extern const std::string BALANCE_REGEX_STR;
extern const std::string EXIT_REGEX_STR_CMD;
#endif /* CONSTANTS_H */
