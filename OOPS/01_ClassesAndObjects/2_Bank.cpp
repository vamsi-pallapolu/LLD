#include <iostream>

class BankAccount
{
private:
    double _accountNumber;
    std::string _ownerName;
    double _balance;

public:
    BankAccount(double accountNumer, const std::string &ownerName) : _accountNumber{accountNumer},
                                                                  _ownerName{ownerName},
                                                                  _balance{0} {}

    void deposit(double amount)
    {
        if (amount > 0)
        {
            _balance += amount;
        }
    }

    bool withdraw(double amount)
    {
        if ((_balance > 0 ) && (_balance - amount) >= 0)
        {
            _balance -= amount;
            return true;
        }
        else
        {
            std::cout << "Insufficient funds" << std::endl;
            return false;
        }
    }

    double getBalance() const { return _balance; }
};

int main()
{
    BankAccount account(1, "Vamsi");
    account.deposit(1000);
    std::cout << account.getBalance() << std::endl;

    return 0;
}
