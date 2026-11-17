#include <iostream>
#include <string>
#include <stdexcept>

class BankAccount
{
private:
    std::string _accountHolder;
    double _balance{0};

public:
    // constructor
    BankAccount(const std::string &holder, double balance) : _accountHolder{holder}, _balance{balance}
    {
    }

    const std::string &getAccountHolder() const
    {
        return _accountHolder;
    }

    double getBalance() const
    {
        return _balance;
    }

    void deposit(double amount)
    {
        if (amount < 0) // invariant: Always keep the object in valid state
        {
            throw std::invalid_argument("Amount cannot be negative");
        }
        _balance += amount;
    }

    void withdraw(double amount)
    {
        if (amount <= 0)
        {
            throw std::invalid_argument("Withdrawal amount must be positive");
        }
        if (amount > _balance)
        {
            std::cout << "Insufficient funds" << std::endl;
        }
    }
};

int main()
{

    return 0;
}