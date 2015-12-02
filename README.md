CSCI6320-project
==================

Project description can be found here.

tested and works in ubuntu 14.04 LTS 32-bit
Dependencies
-------------
* openssl headers ([libssl-dev](http://packages.ubuntu.com/trusty/libssl-dev))
* g++ 4.9 or newer (more on that later, detail on package [here](https://launchpad.net/ubuntu/+source/gcc-4.9))  

This project is coded against the [c++11 standard](https://en.wikipedia.org/wiki/C%2B%2B11) and uses c++ regular  
expressions which were not fully implemented into g++ until version 4.9.  

To use this standard a ppa must be added and g++-4.9 must be installed  
(this is in the setup.bash file, simply run the file).

Compilation
-------------
if all is well all that is needed is to run 'make' in the 'src/' directory

Usages
------------
'''
$ ./client -h
invalid port.
usage: ./client <port> [-d] [-h] [-T hostname] [-k private key] [-K public key] [-a accounts] [-m mak_keys] [-M mac_keys]
         port port to connect to (REQUIRED)
        -h help (show this menu)
        -d Debug flag
        -T hostname to connect to (DEFAULT: 'localhost')
        -a account directory for atm (DEFAULT: ./accounts_atm/
        -M mac_key the mac key file (DEFAULT: client.mac_key)
        -m mac_keys mac_keys the  trusted mac keys file (DEFAULT: mac_keys_client.trusted)
'''  
'''
$ ./server -h
invalid port.
usage: ./server <port> [-d] [-h] [-H hostname] [-k private key] [-K public key] [-a accounts_dir] [-m mac_keys] [-M mac_key]
        <port> the port to listen on (REQUIRED)
       -h help (show this menu)
       -d Debug flag
       -H hostname to of sever to (DEFAULT: 'localhost')
       -k private key file (DEFAULT: keys/private_server.pem)
       -K public key file (DEFAULT: keys/public_server.pem)
       -a accounts directory (DEFAULT: ./accounts_bank)
       -m mac_keys the  trusted mac keys file (DEFAULT: mac_keys_server.trusted)
       -M mac_key the my mac key file (DEFAULT: server.mac_key)
'''  
'''
usage: ./proxy <c_port> <s_port> [-d] [-h] [-p port] [-t hostname] [-T hostname]
         <c_port> client port (REQUIRED)
         <s_port> server port (REQUIRED)
        -h help (show this menu)
        -d Debug flag
        -t hostname of proxy (DEFAULT: 'localhost')
        -T hostname of server (DEFAULT: 'localhost')
'''
