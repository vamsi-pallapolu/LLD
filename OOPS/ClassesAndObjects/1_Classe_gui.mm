// GUI wrapper for the Car class defined in Car.h (used by 1_Classe.cpp).
// The class stays untouched — the visualization lives entirely here.
//
// Build:
//   clang++ -std=c++17 -fobjc-arc -framework Cocoa \
//     OOPS/viz/visualizer.mm OOPS/ClassesAndObjects/1_Classe_gui.mm \
//     -o derived/1_Classe_gui
// Run: ./derived/1_Classe_gui

#include "Car.h"
#include "../viz/visualizer.h"

// Generic adapter: describe any Car as a Snapshot. Reusable for any Car instance.
static viz::Snapshot describe(Car &c) {
    return viz::Snapshot{}
        .title(c.brand() + " " + c.model())
        .bar("speed", c.speed(), 200, std::to_string(c.speed()) + " km/hr")
        .line("color: " + c.color())
        .action('w', "accelerate", [&]{ c.increment(20); })
        .action('s', "brake",      [&]{ c.brake(20); });
}

int main() {
    Car hilux{"Toyota", "Hilux", "Red"};
    Car corolla{"Toyota", "Corolla", "Blue"};

    viz::show({
        [&]{ return describe(hilux); },
        [&]{ return describe(corolla); },
    }, "Cars");
    return 0;
}
