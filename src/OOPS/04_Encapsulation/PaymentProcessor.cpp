#include <iostream>
#include <string>

class PaymentProcessor
{
private:
    std::string _cardNumber;
    double _amount;

    std::string masknumber(const std::string &number)
    {
        return "****-****-" + number.substr(number.length() - 4);
    }

public:
    PaymentProcessor(const std::string &number, double amount) : _cardNumber{masknumber(number)}, _amount{amount}
    {
    }

    void processPayment()
    {
        std::cout << "Processing Payment: " << _amount << " for card: " << _cardNumber
                  << std::endl;
    }
};

int main()
{
    PaymentProcessor pp("123456789", 300);
    pp.processPayment();
    return 0;
}