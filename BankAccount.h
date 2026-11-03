#ifndef BANKACCOUNT_H
#define BANKACCOUNT_H

#include <string>
#include <vector>

// Part 2 & 3: Transaction Structure //
struct Transaction {
    std::string type;
    double amount;
    std::string timestamp; // Simple text representation (e.g., "Manual Log") //
};

class BankAccount {
private:
    std::string accountNumber;
    char* accountHolderName;
    double balance;

    // Part 2 & 3: Composition Relationship //
    std::vector<Transaction> transactionHistory;

    void copyName(const char* name);

public:
    // Constructors and Destructor //
    BankAccount();
    BankAccount(std::string num, std::string name, double initialBalance);
    BankAccount(const BankAccount& other);
    BankAccount& operator=(const BankAccount& other);

    virtual ~BankAccount();

    // Getters and Setters //
    std::string getAccountNumber() const;
    std::string getAccountHolderName() const;
    double getBalance() const;
    void setAccountHolderName(std::string name);

    // Legacy Methods //
    void deposit(double amount);
    virtual bool withdraw(double amount);

    void displayAccountInfo() const;

    // Part 2 & 3: Print History Method //
    void printHistory() const;

    // Operators
    BankAccount& operator+=(double amount);
    BankAccount& operator-=(double amount);

    bool operator==(const BankAccount& other) const;
    bool operator<(const BankAccount& other) const;
    bool operator>(const BankAccount& other) const;

    // Static Utility Functions //
    static void printAccount(const BankAccount& account);
    static BankAccount createAccountFromInput();
};

// Checking Account //

class CheckingAccount : public BankAccount {
private:
    double transactionFee;

public:
    CheckingAccount(std::string num, std::string name, double balance, double fee);
    bool withdraw(double amount) override;
};

// Savings Account //

class SavingsAccount : public BankAccount {
private:
    double interestRate;

public:
    SavingsAccount(std::string num, std::string name, double balance, double rate);
    void calculateInterest();
};

#endif
