#include <iostream>
#include <string>

class Car
{
private:
    // Attributes
    std::string _brand;
    std::string _model;
    int _speed;

public:

    // Constructor
    Car(const std::string &brand, const std::string &model) : _brand{brand}, _model {model}{}

    // Method to accelerate
    void increment(int increment){
        _speed += increment;
    }

    // Method to display info
    void displayStatus(){
        std::cout << "brand " << _brand << " is running at " << _speed << "km/hr";
    }
};

int main(){
    Car ob("Toyato", "Hilux");
    ob.increment(10);
    ob.displayStatus();
    return 0;
}