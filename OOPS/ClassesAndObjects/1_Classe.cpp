#include <iostream>
#include <string>

class Car {
private:
    std::string _brand;
    std::string _model;
    std::string _color;
    int _speed = 0;

public:
    Car(const std::string &brand, const std::string &model, const std::string &color)
        : _brand{brand}, _model{model}, _color{color} {}

    void increment(int inc)  { _speed += inc; }
    void brake(int dec)      { _speed = _speed > dec ? _speed - dec : 0; }


    int         speed() const { return _speed; }
    const auto &brand() const { return _brand; }
    const auto &model() const { return _model; }
    // auto &color()  { return _color; } // mutates the object state
    // auto& color() const {return _color;} // Calling this method gurantes, it does not mutate the object state
    // const auto& color() { return _color;} // returns reference that can be assigned to some other string but not let mutate the string
    auto& color() { return _color;} // returns reference that can be assigned to some other string also let mutate the string

    void displayStatus() const {
        std::cout << "brand " << _color << _brand << " is running at " << _speed << "km/hr" << std::endl;
    }
};

int main(){
    Car ob("Toyato", "Hilux", "Red");
    ob.increment(10);
    ob.displayStatus();
    std::string color = ob.color(); //valid
    // ob.color().append("Blue"); // invalid
    ob.color().append("Blue"); // Valid when return type is not const
    ob.displayStatus();
    return 0;
}
