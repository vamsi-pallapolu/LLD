#include <iostream>
#include <stdexcept>

// The Problem
// struct Thermostat{
//     double targetCelsius{};
// };

// int main(){
//     Thermostat stat;
//     stat.targetCelsius = -500; // nothing stop this
//     stat.targetCelsius = 65; // nor this
//     return 0;
// }


class Thermostat{
    private:
        static constexpr double MAX = 30.0;
        static constexpr double MIN = 10.0;
        double _target;
    public:
        Thermostat(double target):_target{target}{
            if(target < MIN || target > MAX){
                throw std::invalid_argument("Target Out Of Range");
            } // valid from birth
        }

        void warmer(){
            if(_target < MAX) _target += 0.5; // Rule enforced here
        }

        void cooler(){
            if(_target > MIN) _target -= 0.5; // Rule enforced here
        }

        double getTarget() const { return _target;}

};


int main(){
    Thermostat t{22};
    t.warmer();
    std::cout << "Target:" << t.getTarget() << std::endl;
    return 0;
}