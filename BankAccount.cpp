#include "BankAccount.h"
#include <iostream>
#include <vector>
#include <limits>
#include <iomanip>
#include <cstring>

//Private Helper//
void BankAccount::copyName(const char* name) {
    if (name) {
        accountHolderName = new char[std::strlen(name) + 1];
        std::strcpy(accountHolderName, name);
    } else {
        accountHolderName = new char[1];
        accountHolderName[0] = '\0';
    }
}

//Constructor and Destructor//
BankAccount::BankAccount() {
    accountNumber = "0000";
    balance = 0.0;
    copyName("Unknown");
}

BankAccount::BankAccount(std::string num, std::string name, double initialBalance) {
    accountNumber = num;
    balance = (initialBalance >= 0.0) ? initialBalance : 0.0;
    copyName(name.c_str());
}

BankAccount::BankAccount(const BankAccount& other) {
    accountNumber = other.accountNumber;
    balance = other.balance;
    copyName(other.accountHolderName);
    transactionHistory = other.transactionHistory; // <-- ADD THIS LINE
}

BankAccount& BankAccount::operator=(const BankAccount& other) {
    if (this != &other) {
        delete[] accountHolderName;
        accountNumber = other.accountNumber;
        balance = other.balance;
        copyName(other.accountHolderName);
        transactionHistory = other.transactionHistory;
    }
    return *this;
}


BankAccount::~BankAccount() {
    delete[] accountHolderName;
}

//Getters and Setters//
std::string BankAccount::getAccountNumber() const { return accountNumber; }
std::string BankAccount::getAccountHolderName() const { return accountHolderName ? std::string(accountHolderName) : ""; }
double BankAccount::getBalance() const { return balance; }

void BankAccount::setAccountHolderName(std::string name) {
    if (!name.empty()) {
        delete[] accountHolderName;
        copyName(name.c_str());
    }
}

//Legacy Wrappers//
void BankAccount::deposit(double amount) {
    if (amount > 0) {
        *this += amount;
        transactionHistory.push_back({"Deposit", amount, "Completed"});
    } else {
        std::cout << "Invalid deposit amount.\n";
    }
}

bool BankAccount::withdraw(double amount) {
    double originalBalance = balance;
    *this -= amount;

    if (balance < originalBalance) {
        transactionHistory.push_back({"Withdrawal", amount, "Completed"});
        return true;
    }
    return false;
}


//Operator Overloading//
BankAccount& BankAccount::operator+=(double amount) {
    if (amount > 0) {
        balance += amount;
        std::cout << "Successfully deposited $" << std::fixed << std::setprecision(2) << amount << "\n";
    } else {
        std::cout << "Invalid deposit amount. Amount must be positive.\n";
    }
    return *this;
}

BankAccount& BankAccount::operator-=(double amount) {
    if (amount <= 0) {
        std::cout << "Invalid withdrawal amount. Amount must be positive.\n";
        return *this;
    }
    if (amount > balance) {
        std::cout << "Transaction failed: Insufficient funds.\n";
        return *this;
    }
    balance -= amount;
    std::cout << "Successfully withdrew $" << std::fixed << std::setprecision(2) << amount << "\n";
    return *this;
}

bool BankAccount::operator==(const BankAccount& other) const {
    return this->accountNumber == other.accountNumber;
}

bool BankAccount::operator<(const BankAccount& other) const {
    return this->balance < other.balance;
}

bool BankAccount::operator>(const BankAccount& other) const {
    return this->balance > other.balance;
}

//Static Utility Functions//
void BankAccount::printAccount(const BankAccount& account) {
    std::cout << "Acc #: " << account.accountNumber
              << " | Holder: " << (account.accountHolderName ? account.accountHolderName : "Unknown")
              << " | Balance: $" << std::fixed << std::setprecision(2) << account.balance << "\n";
}

BankAccount BankAccount::createAccountFromInput() {
    std::string num, name;
    double initialBalance;

    std::cout << "Enter new account number: ";
    std::getline(std::cin, num);

    std::cout << "Enter account holder full name: ";
    std::getline(std::cin, name);

    std::cout << "Enter initial deposit balance: $";
    while (!(std::cin >> initialBalance)) {
        std::cout << "Invalid amount. Please enter a valid decimal number: $";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    return BankAccount(num, name, initialBalance);
}

//UI Helpers Global//
void clearInputStream() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int getValidatedChoice() {
    int choice;
    while (!(std::cin >> choice)) {
        std::cout << "Invalid input. Please enter a number: ";
        clearInputStream();
    }
    clearInputStream();
    return choice;
}

double getValidatedAmount() {
    double amount;
    while (!(std::cin >> amount)) {
        std::cout << "Invalid amount. Please enter a valid decimal number: ";
        clearInputStream();
    }
    clearInputStream();
    return amount;
}

// Checking Account //

CheckingAccount::CheckingAccount(
    std::string num,
    std::string name,
    double balance,
    double fee)
    : BankAccount(num, name, balance)
{
    transactionFee = fee;
}

bool CheckingAccount::withdraw(double amount)
{
    std::cout << "Transaction fee applied: $"
              << transactionFee << "\n";

    return BankAccount::withdraw(amount + transactionFee);
}

// Savings Account //

SavingsAccount::SavingsAccount(
    std::string num,
    std::string name,
    double balance,
    double rate)
    : BankAccount(num, name, balance)
{
    interestRate = rate;
}

void SavingsAccount::calculateInterest()
{
    double interest =
        getBalance() * interestRate;

    deposit(interest);

    std::cout << "Interest added: $"
              << interest
              << "\n";
}

BankAccount* findAccount(
    std::vector<BankAccount*>& accounts,
    const std::string& num)
{
    for (size_t i = 0; i < accounts.size(); ++i)
    {
        if (accounts[i]->getAccountNumber() == num)
        {
            return accounts[i];
        }
    }
    return nullptr;
}

//Loop//
int main() {
    std::vector<BankAccount*> bankSystem;
    int choice = 0;

    bankSystem.push_back(new CheckingAccount("1001", "Alice Smith", 500.00, 2.50));
    bankSystem.push_back(new SavingsAccount("1002", "Bob Jones", 1250.50, 0.05));

    std::cout << "\n--- Polymorphism Demo ---\n";

    BankAccount* checking = new CheckingAccount("3001", "Demo Checking", 500.00, 2.50);
    BankAccount* savings = new SavingsAccount("3002", "Demo Savings", 500.00, 0.05);

    checking->withdraw(100);
    savings->withdraw(100);

    // Testing histories on base/derived pointers to satisfy Part 3 challenges
    checking->printHistory();
    savings->printHistory();

    delete checking;
    delete savings;
    std::cout << "--- Welcome to the Bank Account Management System! ---\n";

    do {
        std::cout << "\n-----------------------------\n";
        std::cout << "1. Create New Account\n";
        std::cout << "2. Deposit Funds\n";
        std::cout << "3. Withdraw Funds\n";
        std::cout << "4. Modify Account Name\n";
        std::cout << "5. View Specific Account\n";
        std::cout << "6. List All Accounts\n";
        std::cout << "7. Compare Two Account Balances\n";
        std::cout << "8. View Account History Logs\n";
        std::cout << "9. Exit\n";
        std::cout << "Enter your selection (1-9): ";

        choice = getValidatedChoice();
        std::cout << "-----------------------------\n\n";

        if (choice == 1) {
            BankAccount newAcc = BankAccount::createAccountFromInput();
            if (findAccount(bankSystem, newAcc.getAccountNumber()) != nullptr) {
                std::cout << "Error: An account with number " << newAcc.getAccountNumber() << " already exists.\n";
            } else {
                bankSystem.push_back(
                    new CheckingAccount(
                        newAcc.getAccountNumber(),
                        newAcc.getAccountHolderName(),
                        newAcc.getBalance(),
                        2.50));
                std::cout << "Account created successfully using static utility!\n";
            }
        }
        else if (choice == 2) {
            std::string num;
            std::cout << "Enter account number: ";
            std::getline(std::cin, num);

            BankAccount* acc = findAccount(bankSystem, num);
            if (acc) {
                std::cout << "Enter deposit amount: $";
                double amt = getValidatedAmount();
                acc->deposit(amt);
            } else {
                std::cout << "Account not found.\n";
            }
        }
        else if (choice == 3) {
            std::string num;
            std::cout << "Enter account number: ";
            std::getline(std::cin, num);

            BankAccount* acc = findAccount(bankSystem, num);
            if (acc) {
                std::cout << "Enter withdrawal amount: $";
                double amt = getValidatedAmount();
                acc->withdraw(amt);
            } else {
                std::cout << "Account not found.\n";
            }
        }
        else if (choice == 4) {
            std::string num, newName;
            std::cout << "Enter account number: ";
            std::getline(std::cin, num);

            BankAccount* acc = findAccount(bankSystem, num);
            if (acc) {
                std::cout << "Enter new account holder name: ";
                std::getline(std::cin, newName);
                acc->setAccountHolderName(newName);
                std::cout << "Name updated successfully.\n";
            } else {
                std::cout << "Account not found.\n";
            }
        }
        else if (choice == 5) {
            std::string num;
            std::cout << "Enter account number: ";
            std::getline(std::cin, num);

            BankAccount* acc = findAccount(bankSystem, num);
            if (acc) {
                BankAccount::printAccount(*acc);
            } else {
                std::cout << "Account not found.\n";
            }
        }
        else if (choice == 6) {
            if (bankSystem.empty()) {
                std::cout << "No accounts in the system.\n";
            } else {
                std::cout << "--- Listing All Accounts ---\n";
                for (const auto& acc : bankSystem){
                    BankAccount::printAccount(*acc);
                }
            }
        }
        else if (choice == 7) {
            std::string num1, num2;
            std::cout << "Enter first account number: ";
            std::getline(std::cin, num1);
            std::cout << "Enter second account number: ";
            std::getline(std::cin, num2);

            BankAccount* acc1 = findAccount(bankSystem, num1);
            BankAccount* acc2 = findAccount(bankSystem, num2);

            if (acc1 && acc2) {
                if (*acc1 == *acc2) {
                    std::cout << "Error: You entered the exact same account.\n";
                } else if (*acc1 > *acc2) {
                    std::cout << acc1->getAccountHolderName() << " has a larger balance than " << acc2->getAccountHolderName() << ".\n";
                } else if (*acc1 < *acc2) {
                    std::cout << acc1->getAccountHolderName() << " has a smaller balance than " << acc2->getAccountHolderName() << ".\n";
                } else {
                    std::cout << "Both accounts have exactly the same balance.\n";
                }
            } else {
                std::cout << "One or both accounts were not found.\n";
            }
        }
        else if (choice == 8) {
            std::string num;
            std::cout << "Enter account number: ";
            std::getline(std::cin, num);

            BankAccount* acc = findAccount(bankSystem, num);
            if (acc) {
                acc->printHistory();
            } else {
                std::cout << "Account not found.\n";
            }
        }
    } while (choice != 9); // Correct loop validation checkpoint anchor

    std::cout << "Thank you for using the Bank Account Management System. Have a nice day!\n";
    for (auto acc : bankSystem) {
        delete acc;
    }
    return 0;
}

void BankAccount::printHistory() const {
    std::cout << "\n=== Transaction History for Acc #: " << accountNumber << " ===\n";
    if (transactionHistory.empty()) {
        std::cout << "No transaction logs available.\n";
        return;
    }
    std::cout << std::left << std::setw(15) << "Type"
              << std::setw(12) << "Amount"
              << std::setw(15) << "Status" << "\n";
    std::cout << "------------------------------------------\n";
    for (const auto& tx : transactionHistory) {
        std::cout << std::left << std::setw(15) << tx.type
                  << "$" << std::setw(11) << std::fixed << std::setprecision(2) << tx.amount
                  << std::setw(15) << tx.timestamp << "\n";
    }
}
