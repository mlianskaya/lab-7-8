#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class BankAccount
{
protected:
	int accountNumber;
	double balance;
public:
	BankAccount(int accountNamberValue, double balanceValue)
	{
		accountNumber = accountNamberValue;
		balance = balanceValue;
	}

	bool operator == (const BankAccount& other)
	{
		if (accountNumber == other.accountNumber && balance == other.balance)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void deposit(double amount) 
	{
		balance += amount;
	}

	void withdraw(double amount) 
	{
		if (balance >= amount) 
		{
			balance -= amount;
		}
		else 
		{
			cout << "Insufficient balance!" << endl;
		}
	}

	int getAccountNumber() const
	{
		return accountNumber;
	}

	int getBalance() const {
		return balance;
	}

	void setBalance(int newBalance) {
		balance = newBalance;
	}

	virtual void processTransaction(double amount) {}
	virtual void displayAccountDetails() 
	{
		cout << "__INFORMATION__" << endl;
		cout << "Account Number: " << accountNumber << endl;
		cout << "Balance: " << balance << endl;
	}
	
	void saveAccountInfo() 
	{
		ofstream outFile("accountInfo.txt", ios::app);
		outFile << accountNumber << " " << balance << endl;
		outFile.close();
	}

	void loadAccountInfo() 
	{
		ifstream inFile("accountInfo.txt");
		if (inFile.is_open()) 
		{
			int accountNum;
			double accBalance;

			while (inFile >> accountNum) 
			{
				inFile >> accBalance;

				if (accountNum == accountNumber) 
				{
					balance = accBalance;
					break;
				}
			}

			inFile.close();
		}
	}
	
	virtual ~BankAccount() {}
};

class Customer
{
private:
	string name;
	BankAccount* account;
public:
	Customer(string nameValue)
	{
		name = nameValue;
		account = nullptr;
	}

	void displayAccountDetails() 
	{
		account->displayAccountDetails();
	}

	BankAccount* getAccount() const
	{
		return account;
	}

	string getName() const
	{
		return name;
	}

	~Customer()
	{
		delete account;
	}
};

class Transaction
{
private:
	double amount;
public:
	Transaction(double amountValue)
	{
		amount = amountValue;
	}

	void deposit(BankAccount* account) 
	{
		account->deposit(amount);
	}

	void withdraw(BankAccount* account) 
	{
		account->withdraw(amount);
	}

	static void transferMoney(BankAccount& fromAccount, BankAccount& toAccount, double amount)
	{
		// Проверяем, достаточно ли денег на счете для перевода
		if (amount <= fromAccount.getBalance()) 
		{
			// Снимаем деньги со счета fromAccount
			fromAccount.withdraw(amount);

			// Зачисляем деньги на счет toAccount
			toAccount.deposit(amount);

			cout << "Transfer completed successfully!" << endl;
		}
		else 
		{
			cout << "Insufficient funds to transfer!" << endl;
		}
	}

	static void updateTransaction(BankAccount* account1, BankAccount* account2)
	{
		double balance1 = account1->getBalance();
		double balance2 = account2->getBalance();
		account1->processTransaction(balance2 - balance1);
		account2->processTransaction(balance1 - balance2);
	}
};

class SavingsAccount : public BankAccount
{
public:
	string type;
	string dateString;
	
	SavingsAccount(int accountNumber, double balance) : BankAccount(accountNumber, balance) {}
	bool isDateStringStartOfMonth(const string& date) 
	{
		// Проверяем, что длина строки даты равна 10 символам (формат "yyyy-mm-dd")
		if (date.length() != 10) {
			return false;
		}

		// Извлекаем год, месяц и день из строки даты
		int year = stoi(date.substr(0, 4));
		int month = stoi(date.substr(5, 2));
		int day = stoi(date.substr(8, 2));

		// Проверяем, что день равен 1 и месяц находится в диапазоне от 1 до 12
		return (day == 1 && month >= 1 && month <= 12);
	}

	void processTransaction(double amount) override
	{
		cout << "Enter Transaction Type (Deposit/Withdrawal): ";
		cin >> type;
		cout << "Enter Transaction Date: ";
		cin >> dateString;
		if (type == "Deposit")
		{
			balance += amount;
			if (isDateStringStartOfMonth(dateString))
			{
				balance += balance * 0.06;
			}
		}
		
		else if (type == "Withdrawal")
		{
			balance -= amount;
			if (balance < 0)
			{
				cout << "Insufficient funds!" << endl;
			}
		}
		else
		{
			cout << "Incorrect value!" << endl;
		}
		cout << "Transaction Completed Successfully!" << endl;
		cout << "Updated balance: " << balance << endl;
		cout << endl;
	}
	void displayAccountDetails() override 
	{
		cout << "__INFORMATION__" << endl;
		cout << "Account Number: " << accountNumber << endl;
		cout << "Account Type: Savings Account" << endl;
		cout << "Balance: " << balance << endl;
	}
};

class CheckingAccount : public BankAccount
{
public:
	string type;
	CheckingAccount(int accountNumber, double balance) : BankAccount(accountNumber, balance) {}

	void processTransaction(double amount) override
	{
		cout << "Enter Transaction Type (Deposit/Withdrawal): ";
		cin >> type;

		if (type == "Deposit")
		{
			balance += amount;
		}
		else if (type == "Withdrawal")
		{
			balance -= amount;
			if (balance < 0)
			{
				cout << "Insufficient funds!" << endl;
			}
		}
		else 
		{
			cout << "Incorrect value!" << endl;
		}
		cout << "Transaction Completed Successfully!" << endl;
		cout << "Updated balance: " << balance << endl;
		cout << endl;
	}

	void displayAccountDetails() override 
	{
		cout << "__INFORMATION__" << endl;
		cout << "Account Number: " << accountNumber << endl;
		cout << "Account Type: Checking Account" << endl;
		cout << "Balance: " << balance << endl;
	}
};



int main()
{
	string name, accountType, transactionType;
	int accountNumber;
	double initialBalance, amount;

	while (true) 
	{
		cout << "Welcome to the bank! Select the required service" << endl;
		cout << "---------- Bank System Menu ----------" << endl;
		cout << "1. Create Account" << endl;
		cout << "2. Process Transaction" << endl;
		cout << "3. Money Transfer" << endl;
		cout << "4. Display Account Data" << endl;
		cout << "5. Save Account Data to File" << endl;
		cout << "6. Load Account Data from File" << endl;
		cout << "7. Comparison of accounts" << endl;
		cout << "0. Exit" << endl;
		cout << "---------------------------------------" << endl;
		cout << "Enter choice: ";
		int choice;
		cin >> choice;

		switch (choice)
		{
			case 1:
			{
				cout << "Enter Customer Name: ";
				cin >> name;
				cout << "Enter Account Number: ";
				cin >> accountNumber;
				cout << "Enter Account Type (Savings/Checking): ";
				cin >> accountType;
				cout << "Enter Initial Balance: ";
				cin >> initialBalance;
				if (accountType == "Savings")
				{
					SavingsAccount account(accountNumber, initialBalance);
				}
				else if (accountType == "Checking")
				{
					CheckingAccount account(accountNumber, initialBalance);
				}
				cout << "Bank account created for customer: " << name << endl;
			}
			break;

			case 2:
			{
				cout << "Enter Account Type (Savings/Checking): ";
				cin >> accountType;

				if (accountType == "Savings")
				{
					cout << "Enter Account Number: ";
					cin >> accountNumber;
					cout << "Enter Initial Balance: ";
					cin >> initialBalance;

					SavingsAccount account(accountNumber, initialBalance);

					cout << "Enter Transaction Amount: ";
					cin >> amount;
					account.processTransaction(amount);	
				}
				else if (accountType == "Checking")
				{
					cout << "Enter Account Number: ";
					cin >> accountNumber;
					cout << "Enter Initial Balance: ";
					cin >> initialBalance;

					CheckingAccount account(accountNumber, initialBalance);

					cout << "Enter Transaction Amount: ";
					cin >> amount;
					account.processTransaction(amount);
				}
			}
			break;

			case 3:
			{
				cout << "Enter Account Number from which the transfer is made: ";
				cin >> accountNumber;
				cout << "Enter Initial Balance: ";
				cin >> initialBalance;
				BankAccount account1(accountNumber, initialBalance);

				cout << "Enter Account Number to which funds are credited: ";
				cin >> accountNumber;
				cout << "Enter Initial Balance: ";
				cin >> initialBalance;
				BankAccount account2(accountNumber, initialBalance);

				cout << "Enter Transfer Amount: ";
				cin >> amount;

				Transaction::transferMoney(account1, account2, amount);

				// Выводим балансы по счетам после перевода
				cout << "Account balance #1 after transfer: " << account1.getBalance() << endl;
				cout << "Account balance #2 after transfer: " << account2.getBalance() << endl;
			}
			break;

			case 4:
			{
				cout << "Enter Account Number: ";
				cin >> accountNumber;
				cout << "Enter Account Type (Savings/Checking): ";
				cin >> accountType;
				cout << "Enter Initial Balance: ";
				cin >> initialBalance;

				if (accountType == "Savings")
				{
					SavingsAccount account(accountNumber, initialBalance);
					account.displayAccountDetails();
				}
				else if (accountType == "Checking")
				{
					CheckingAccount account(accountNumber, initialBalance);
					account.displayAccountDetails();
				}
			}
			break;

			case 5:
			{
				cout << "Enter Account Number: ";
				cin >> accountNumber;
				cout << "Enter Account Type (Savings/Checking): ";
				cin >> accountType;
				cout << "Enter Initial Balance: ";
				cin >> initialBalance;
				BankAccount account(accountNumber, initialBalance);
				account.loadAccountInfo();
			}
			break;

			case 6: 
			{
				BankAccount account(accountNumber, initialBalance);
				account.saveAccountInfo();
				account.displayAccountDetails();
			}
			break;

			case 7:
			{
				cout << "Enter Account Number #1: ";
				cin >> accountNumber;
				cout << "Enter Initial Balance: ";
				cin >> initialBalance;
				BankAccount account1(accountNumber, initialBalance);

				cout << "Enter Account Number #2: ";
				cin >> accountNumber;
				cout << "Enter Initial Balance: ";
				cin >> initialBalance;
				BankAccount account2(accountNumber, initialBalance);

				bool comparison = account1 == account2;
				cout << "Bank Account Equality:\n" << (comparison ? "yes" : "no") << endl;
			}
			break;

			case 0:
				cout << "See you!" << endl;
				return 0;

			default:
				cout << "Invalid choice. Please try again." << endl;
				break;
		}
		
	}

}