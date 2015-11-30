Protocol
==============

1. Bank send public key (PUB-BANK) to client
2. Client sends public key (PUB-CLI) to bank (encrypted with PUB-BANK)
3. Bank sends OK to client and requests MAC
4. Client send MAC to bank (encrypted with PUB-BANK)
5. if MAC valid bank sends request for account information:  
  Account information consists of:
  * account number
  * PIN 
7. Bank sends OK/NOT OK
     -- if valid bank locks account from other login attempts
8. if OK, client sends transaction
9. Bank returns OK/NOT OK
10. if OK change account

NOTES
-------
* if client fails #4, bank terminates connection, locks account  
  and blocks all incoming connections from that ip for
  20 seconds.

* if client fails #5 bank allows 2 more login attempts  
 then completely locks out the account.

* if client fails #8, simply ignore the transaction,  
 bring them back to main menu with a failure msg.
