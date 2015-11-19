Getting started
===============

1. Install necessary software:  
  ```
  sudo apt-get update
  sudo apt-get upgrade
  sudo apt-get install git libssl-dev
  ```

2. setup git
  ```
  git config --global user.name <Github username>
  git config --global user.email <Gitbub email>
  ```
  **NOTE: these are the email and username you provided  
  when you setup a Github account**

3. Install your favorite editor (some suggestions below):
  * emacs (good tutorial [here](http://www.jesshamrick.com/2012/09/10/absolute-beginners-guide-to-emacs/))  
    ```
    sudo apt-get install emacs
    ```
  * vim (good tutorial [here](http://vim.wikia.com/wiki/Tutorial))  
    ```
    sudo apt-get install vim
    ```
  * sublime text
    * this is not in the ubuntu repos but can be downloaded [here](http://www.sublimetext.com/).
    * **NOTE: unless you have XP in Vim or Emacs I would suggest sublime text  
 as it is the most intuative.**

4) clone this repository:
  ```
  cd ~/Desktop
  git clone https://github.com/FireElementalNE/CSCI6320-project.git
  ```

5 Build the project:  
  ```
  cd ~/Desktop/CSCI6320-project/prototyping
  make
  ```
5. test it out in 3 separate terminal windows:
  1. server window:  
    ```shell
    ./client -d
    ```
  2. proxy window:  
    ```shell
    ./proxy -d
    ```
  3. client window:  
    ```shell
    ./client -d
    ```

if you type something on the client you should see it propagate:  
client -> proxy -> server -> proxy -> client  

Feel free to email me with questions
