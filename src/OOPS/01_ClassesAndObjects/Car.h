#pragma once
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
    auto &color() const { return _color; }

    void displayStatus() const {
        std::cout << "brand " << _brand << " is running at " << _speed << "km/hr";
    }
};
