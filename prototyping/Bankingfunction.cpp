#include <iostream>
#include <string>
#include <fstream>
using namespace std;

int main()
{ 
	int password;
	int accountnumber;
	    
	    
	for(int i=0;i<3;i++)
	{
	    cout <<"\nEnter Account Number:\n";
        cin>>accountnumber;
        cout <<"\n";
	    
	    if (accountnumber==565656)//may have to build a parent class of account numbers
        {
	    
	        break;
	    
        }
        else
            cout<<"Account number not recognized, please try again!\n";
        
        if (i == 2) return 0;//if failed three times, end program
        
	}//end account number loop

    for (int i=0;i<3;i++) 
    {
        
        cout <<"Enter PIN:\n"; 
	    cin>>password;
	    cout <<"\n";

	        //loop that only allows three attempts at the passord
	        if (password==7777) //obviously we need a better password
	        {
	        
		        cout<<"Correct!!!\n"; 
		        break; 
	        
	        } 
	        else
	        	if(i != 2)
	        	{
		        	cout<<"Incorrect PIN, Please try again!\n";
	        	}
	        	else
	        	{
	        		cout<<"You have entered the PIN incorrectly three times\n";
	        		cout<<"\nYour account has been locked by the bank for your safety";
	        	}
	        
	        if (i == 2) return 0; // if failed 3 times, end program 
	} // end password loop



 
 /////////////////////////////////////////////////////////////////////
 //						ATM Functionality							//
 /////////////////////////////////////////////////////////////////////
 

	    double balance=10000; //starting balance (may need to include this in a file somewhere associated with users on the bank server)
	    double withdrawal, deposit; //need doubles to perform functions for withdraw and deposit
	    int option;//need a number to select the options in the menu 
	    bool finished = false; // if user selects option 4, 'quit', finished = true, otherwise finished = false
 
 
    	cout<<"\n"; 
	    cout<<"*** Automated Teller Machine ***"<<endl; 
	while (!finished) 
	{ // repeat until user selects quit 
	cout<<"\nChoose a Transaction:\n"; 
	cout<<"\n"; 
	cout<<"[1] Inquire Balance \n" 
	<<"[2] Withdrawal \n" 
	<<"[3] Deposit \n" 
	<<"[4] Quit \n" 
	<<"\n" 
	<<"Enter Option:"; 
	cin>>option; 

	    switch(option) 
    	{    
    	case 1: 
	    	cout<<"\n  BALANCE INQUIRY  \n"; 
	    	cout.setf(ios::fixed); 
	    	cout.setf(ios::showpoint); 
	    	cout.precision(2); 
	    	cout<<"\n Your current balance is $"<<balance<<endl; 
	    	break; 
    	case 2: 
	    	cout<<"\n  WITHDRAWAL  \n"; 
	    	cout<<"Enter amount: $"; 
	    	cin>>withdrawal; 

        	//to check if there is enough money and to prevent overdrafting
        	if(balance>=0 && withdrawal<=balance)
        	{
	        	balance = balance - withdrawal; 
            	cout.setf(ios::fixed); 
	        	cout.setf(ios::showpoint); 
	        	cout.precision(2); 
	        	cout<<"You withdrew $"<<withdrawal<<endl; 
	        	cout<<"Your remaining balance is $"<<balance<<endl;
        	}
        	else
        	{
            	cout<<"You do not have enough money!!!";
        	}
        	continue; 
    	case 3:
	    	cout<<"\n  DEPOSIT  \n"; 
	    	cout<<"Enter amount: $"; 
	    	cin>>deposit;
	    	//prevent depositing a negative number
        	if(deposit>0 && deposit<99999)
        	{
            	balance = balance + deposit; 
            	cout.setf(ios::fixed); 
	        	cout.setf(ios::showpoint); 
	        	cout.precision(2); 

	        	cout<<"You deposited $"<<deposit<<endl; 
	        	cout<<"Your new balance is $"<<balance<<endl;
        	}
        	else
        	{
            	cout<<"\nYou cannot deposit this amount, try again!\n";
        	}
        
	        continue;
		case 4: finished = true; // user has selected quit and changes the bool value
	    	cout<<"\n*** YOUR TRANSACTION HAS FINISHED ***\n";
	    	cout<<"       *** Have a nice day! ***\n";
        	break;  
    	default: 
	    	cout<<"\n That is an invalid option \n";
	    	cout<<"Select an option from 1-4 only\n";
    	} 

	} 
return 0; 
}


//Works cited: initial functionality derived from (https://www.gidforums.com/t-17106.html)