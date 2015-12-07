Protocol
==============
1. Client send plaintext string 'PUBKEY' to bank
2. Bank send public key (PUB-B) to client
3. Client sends public key (PUB-C) to bank.  
AFTER THIS POINT ALL MESSAGES ARE ENCRYPTED USING [OPENSSL RSA](https://www.openssl.org/docs/manmaster/crypto/rsa.html)
4. Bank Send E(PUB-C, 'MAC.') to client.
5. Client takes its MAC key and hashes a random message using [HMAC](https://www.openssl.org/docs/manmaster/crypto/hmac.html)  
and [EVP_sha256](https://www.openssl.org/docs/manmaster/crypto/EVP_DigestInit.html) (MAC-C) it then sends a the message concatenated with the  
result of HMAC. (sends E(PUB-B, MSG + ':' + HASH-C))
6. The bank checks the MAC by taking the message and using its stored version  
of the client's MAC key and running HMAC/EVP_sha256 on it. If the HMAC/ECP_sha256  
message that the bank received matches the HMAC/EVP_sha256, proceed.
7. Steps 5 and 6 are repeated with the bank authenticating with the client.  
sends E(PUB-C, MSG + ':' + HASH-B)  
WE NOW HAVE A SECURE/TRUSTED CONNECTION
8. The client waits for the user to login.
9. When the user logs in, client sends E(PUB-B, USERNAME + ':' + PIN)
10. Iff the login credentials are correct, bank sends E(PUB-C, 'OK.')
USER CAN NOW USE BANK
11. Client presents user with command shell in which to do transactions.
12. Client sends transaction to Bank
13. Iff transaction is successful bank send OK to client along with changes.
14. User logs out of terminal.
15. bank/client close connection

NOTES
-------
* all connection closures do not notify the other party
* if client fails steps #6 or #9 bank closes connection.
* if bank fails step #7 client closes connection.
* if any decryption fails (message tampering) bank/client closes connection.
* if there are three login failures to client, client locks account.
* if there are three login failures to bank, bank locks account.
* if a client is inactive at the terminal for more than 15 seconds bank  
locks account.
* if client fails #13, simply ignore the transaction,  
 bring them back to main menu with a failure message.
