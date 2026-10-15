#include <iostream>

enum Color {RED, YELLOW, GREEN};
enum TrafficLight {RED_LIGHT, ORANGE_LIGHT, GREEN_LIGHT};

enum class Color2{RED, YELLOW, GREEN};
enum class TrafficLight2{RED_LIGHT, YELLOW_LIGHT, GREEN_LIGHT};

int main(){
    Color c = RED; // no need to qualify
    int x = c; // implicit conversion
    std::cout << "x:" << x << std::endl;

    if ( c == RED_LIGHT) {
        std::cout << "This shouldn't be allowed\n";
    }

    enum TrafficLight{
        RED
    };

    Color2 c2 = Color2::RED; // must qualify 
    // int y = c2; // compiler error - no implicit conversion

    if ( c2 == Color2::RED){
        std::cout << "Safe comparison\n";
    }
    return 0;
}