#include <iostream>
#include<fstream>
#include<vector>
#include<map>
#include<cstdlib>
#include<string>
using namespace std;

#define MIN_BALANCE 500                                      //constant

class InsufficientFunds {
public:
	InsufficientFunds() {
		cout << "Insufficient Balance,Minimum Balance Limit Crossed ! " << endl;
		exit(0);
	}
}; //exception 1
class AccDoesntExt {
public:
	AccDoesntExt() {
		cout << "\n\tAccount Does Not Exist ! " << endl;
		exit(0);
	}
};//exception 2

class Account {
private:
	//data member

	long accountNumber;
	string firstName;
	string lastName;
	float balance;
	static long NextAccountNumber;
public:
	//member function

	Account();// { }                                       //default constructor
	Account(string fname, string lname, float balance);      //parameterised constructor

	//accessor ,no mutator because we cant edit once account is created

	long getAccNo() { return accountNumber; }
	string getFirstName() { return firstName; }
	string getLastName() { return lastName; }
	float getBalance() { return balance; }
	//operation perform for user
	void Deposit(float amount);
	void Withdraw(float amount);

	//static function for assign account number automatically and store it into file

	static void setLastAccountNumber(long accountNumber);
	static long getLastAccountNumber();

	//overloading operator for store data into file and retrive them

	friend ofstream& operator<<(ofstream& ofs, Account& acc);
	friend ifstream& operator>>(ifstream& ifs, Account& acc);
	friend ostream& operator<<(ostream& os, Account& acc);
};

//intialisation of static member for account number automatically assign
long Account::NextAccountNumber = 0;

//class for banking official

class Bank {
private:
	map<long, Account> accounts; //all the data will stored in a conatainer for banking official for performing operation
public:
	Bank();//default constructor
	//performing operation for banking official
	Account OpenAccount(string fname, string lname, float balance);
	Account BalanceEnquiry(long accountNumber);
	Account Deposit(long accountNumber, float amount);
	Account Withdraw(long accountNumber, float amount);
	void CloseAccount(long accountNumber);
	void ShowAllAccounts();
	~Bank();
};
int main() {
	Bank b;
	Account acc;
	int choice;
	string fname, lname;
	long accountNumber;
	float balance;
	float amount;
	cout << "***Banking System Software Developed By SK ASIF IQBAL***" << endl;
	do {
		cout << "\n\tSelect One Option Below:\n";
		cout << "\n\t1 Open An Account ";
		cout << "\n\t2 Balance Enquiry ";
		cout << "\n\t3 Deposit ";
		cout << "\n\t4 Withdrawl ";
		cout << "\n\t5 Close An Account ";
		cout << "\n\t6 Show All Accounts Details ";
		cout << "\n\t7 Quit \n";
		cout << "\nEnter Your Choice:  ";
		cin >> choice;
		switch (choice) {
		case 1:
			cout << "\nEnter First Name : ";
			getline(cin >> ws, fname);
			cout << "Enter Last Name : ";
			getline(cin >> ws, lname);
			cout << "Enter Intial Balance: ";
			cin >> balance;
			acc = b.OpenAccount(fname, lname, balance);
			cout << endl << "Congratulations Account Is Created !" << endl;
			cout << endl << acc;
			break;
		case 2:
			cout << "\nEnter Account Number: ";
			cin >> accountNumber;
			acc = b.BalanceEnquiry(accountNumber);
			cout << endl << "Your Account Details Is :" << endl;
			cout << endl << acc;
			break;
		case 3:
			cout << "\nEnter Account Number: ";
			cin >> accountNumber;
			cout << "Enter Balance: ";
			cin >> amount;
			acc = b.Deposit(accountNumber, amount);
			cout << endl << "Amount Is Deposited !" << endl;
			cout << endl << acc;
			break;
		case 4:
			cout << "\nEnter Account Number: ";
			cin >> accountNumber;
			cout << "Enter Balance: ";
			cin >> amount;
			acc = b.Withdraw(accountNumber, amount);
			cout << endl << "Amount Withdrawn ! " << endl;
			cout << endl << acc;
			break;
		case 5:
			cout << "\nEnter Account Number: ";
			cin >> accountNumber;
			b.CloseAccount(accountNumber);
			cout << endl << "Account Is Closed !" << endl;

		case 6:
			b.ShowAllAccounts();
			break;
		case 7:break;
		default:
			cout << "\n Choice Is Not Correct !\n ";
			//exit(0);
		}
	} while (choice != 7);
	return 0;
}
//function defination
//account class function defination
Account::Account() {//default constructor

	accountNumber = 0;
	balance = 0;
}
Account::Account(string fname, string lname, float balance) {//parameterised constructor
	NextAccountNumber++;
	accountNumber = NextAccountNumber;
	firstName = fname;
	lastName = lname;
	this->balance = balance;
}
void Account::Deposit(float amount) {
	balance += amount;
}
void Account::Withdraw(float amount) {
	if (balance - amount < MIN_BALANCE)
		throw InsufficientFunds();
	balance -= amount;
}
void Account::setLastAccountNumber(long accountNumber) {
	NextAccountNumber = accountNumber;
}
long Account::getLastAccountNumber() {
	return NextAccountNumber;
}
ofstream& operator<<(ofstream& ofs, Account& acc) {
	ofs << acc.accountNumber << endl;
	ofs << acc.firstName << endl;
	ofs << acc.lastName << endl;
	ofs << acc.balance << endl;
	return ofs;
}
ifstream& operator>>(ifstream& ifs, Account& acc) {
	ifs >> acc.accountNumber;
	getline(ifs >> ws, acc.firstName);
	getline(ifs >> ws, acc.lastName);
	ifs >> acc.balance;
	return ifs;
}
ostream& operator<<(ostream& os, Account& acc) {
	os << "First Name: " << acc.getFirstName() << endl;
	os << "last Name: " << acc.getLastName() << endl;
	os << "Account Number: " << acc.getAccNo() << endl;
	os << "Balance: " << acc.getBalance() << endl;
	return os;
}

//bank class function defination

Bank::Bank() {//default constructor

	Account account;
	ifstream infile;
	infile.open("Bank.data");
	if (!infile) {
		cout << "Error! Database Does Not Found,May Be It's Your First Operation !" << endl;
		return;
	}
	while (!infile.eof()) {//eof()  , is a bool function which is true if the EOF character has been read, false otherwise.
		infile >> account;
		accounts.insert(pair<long, Account>(account.getAccNo(), account)); //storing data into map containner for access og bank employee
	}
	Account::setLastAccountNumber(account.getAccNo());
	infile.close();
}
Account Bank::OpenAccount(string fname, string lname, float balance) {
	ofstream outfile;
	Account account(fname, lname, balance);
	accounts.insert(pair<long, Account>(account.getAccNo(), account));//store into map
	outfile.open("Bank.data", ios::trunc);//opening database for store 

	map<long, Account>::iterator itr;
	for (itr = accounts.begin(); itr != accounts.end(); itr++) {
		outfile << itr->second;
	}
	outfile.close();
	return account;
}
Account Bank::BalanceEnquiry(long accountNumber) {
	map<long, Account>::iterator itr = accounts.find(accountNumber);
	if (itr == accounts.end())
		throw AccDoesntExt();

	return itr->second;


}
Account Bank::Deposit(long accountNumber, float amount) {
	map<long, Account>::iterator itr = accounts.find(accountNumber);
	if (itr == accounts.end())
		throw AccDoesntExt();
	itr->second.Deposit(amount);
	return itr->second;


}
Account Bank::Withdraw(long accountNumber, float amount) {
	map<long, Account>::iterator itr = accounts.find(accountNumber);
	if (itr == accounts.end())
		throw AccDoesntExt();
	itr->second.Withdraw(amount);
	return itr->second;

}
void Bank::CloseAccount(long accountNumber) {
	map<long, Account>::iterator itr = accounts.find(accountNumber);
	if (itr == accounts.end())
		throw AccDoesntExt();
	cout << "Account Deleted " << itr->second;
	accounts.erase(accountNumber);//erase a data from map
}
void Bank::ShowAllAccounts() {
	cout << endl << "Present Accounts Are:\n" << endl;
	map<long, Account>::iterator itr;
	for (itr = accounts.begin(); itr != accounts.end(); itr++) {
		cout << itr->second << endl;
	}
}
Bank::~Bank() {
	ofstream outfile;

	outfile.open("Bank.data", ios::trunc);//opening database for store all updating work for access next

	map<long, Account>::iterator itr;
	for (itr = accounts.begin(); itr != accounts.end(); itr++) {
		outfile << itr->second;
	}
	outfile.close();
