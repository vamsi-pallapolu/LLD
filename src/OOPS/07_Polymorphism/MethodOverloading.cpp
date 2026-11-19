#include <iostream>

class Calculator
{
public:
    int add(int number1, int number2)
    {
        return number1 + number2;
    }

    int add(int number1, int number2, int number3)
    {
        return number1 + number2 + number3;
    }

    int add(double number1, double number2)
    {
        return number1 + number2;
    }
};

int main()
{
    Calculator c;
    std::cout << c.add(1, 2) << std::endl;
    std::cout << c.add(3, 3, 3) << std::endl;
    std::cout << c.add(10.5, 1.5) << std::endl;
    return 0;
}