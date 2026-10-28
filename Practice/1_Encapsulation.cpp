#include <iostream>
#include <stdexcept>
#include <string>

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


// class Thermostat{
//     private:
//         static constexpr double MAX = 30.0;
//         static constexpr double MIN = 10.0;
//         double _target;
//     public:
//         Thermostat(double target):_target{target}{
//             if(target < MIN || target > MAX){
//                 throw std::invalid_argument("Target Out Of Range");
//             } // valid from birth
//         }

//         void warmer(){
//             if(_target < MAX) _target += 0.5; // Rule enforced here
//         }

//         void cooler(){
//             if(_target > MIN) _target -= 0.5; // Rule enforced here
//         }

//         double getTarget() const { return _target;}

// };



// Another exmple of encapsulation
// struct AdmissionTicket{
//     std::string _visitor{};
//     int priceCents{};

// };

class AdmissionTicket{
private:
    std::string _visitor{};
    int _priceCents{};
public:
    AdmissionTicket(const std::string& name, int priceCents):_visitor{name}, _priceCents{priceCents}{
        if(_priceCents < 0){
            throw std::invalid_argument("Price must not be negative");
        }
    }

    void setPriceCents(int priceCents){
        if(priceCents < 0)
            throw std::invalid_argument("Price must not be negative");
        _priceCents = priceCents;
    }

    std::string getVisitor() const{ return _visitor;}

    int getPriceCents() const {return _priceCents;}


};


int main(){
    // Thermostat t{22};
    // t.warmer();
    // std::cout << "Target:" << t.getTarget() << std::endl;

    // AdmissionTicket adult{"Maya", 1500};
    // adult.priceCents = -100; // nothing stop this
    // There is no check to stop the assignment

    AdmissionTicket adult("Maya", 1500);
    AdmissionTicket child("Leo", 800);

    std::cout << adult.getVisitor() << ": " << adult.getPriceCents() << std::endl;
    std::cout << child.getVisitor() << ": " << child.getPriceCents() << std::endl;

    try{
        adult.setPriceCents(-100);
    }catch(const std::invalid_argument& e){
        std::cout << "Exception raised:" << e.what() << std::endl;
    }


    return 0;
}

/*
===============================================================================
 GETTER NOTES: RETURN VALUES, REFERENCES (&), AND CONST
===============================================================================

1. RETURN A COPY — current implementation
---------------------------------------
    std::string getVisitor() const {
        return _visitor;
    }

    - The caller receives an independent copy of _visitor.
    - Changing that copy does not change the ticket's stored name.
    - The copy can outlive the ticket.
    - Only trailing const is needed for this version.

    Example:
        std::string name = adult.getVisitor();
        name = "Alex";  // The ticket still stores "Maya".

2. RETURN A CONST REFERENCE — avoid copying the string
----------------------------------------------------
    const std::string& getVisitor() const {
        return _visitor;
    }

    - The caller can read the stored string without copying it.
    - The caller cannot modify it through the returned reference.
    - This is a reference to the stored string, not an independent snapshot.
    - Do not use the reference after the ticket is destroyed.

    With this alternative getter:
        const std::string& name = adult.getVisitor();  // No copy.
        std::string copy = adult.getVisitor();        // Makes a copy.

3. WHY ARE THERE TWO CONSTS?
---------------------------
    const std::string& getVisitor() const
    ^                              ^
    Protects the returned string   Prevents changes inside the getter

    First const:
        The caller cannot modify the string THROUGH this reference.

    Trailing const:
        The getter cannot modify the object's ordinary (non-mutable)
        data members. It can also be called on a const ticket.

4. CAN I USE ONLY TRAILING CONST?
--------------------------------
    YES, when returning a copy:
        std::string getVisitor() const { return _visitor; }

    NO, when returning a mutable reference to this member:
        std::string& getVisitor() const { return _visitor; }  // Error!

    Inside the const getter, _visitor is treated as const. Returning it
    as std::string& would allow modification, so the compiler rejects it.
    Use const std::string& to return a read-only reference.

5. WHY AVOID A MUTABLE REFERENCE IN THIS GETTER?
----------------------------------------------
    std::string& getVisitor() {
        return _visitor;
    }

    This compiles, but lets callers change private data directly:
        adult.getVisitor() = "Alex";  // Changes the ticket's stored name.

    That bypasses any validation you might put in a setter.

6. WHAT ABOUT INT AND DOUBLE?
----------------------------
    int getPriceCents() const {
        return _priceCents;
    }

    Small types are cheap to copy, so prefer returning them by value.
    Returning int& would let callers bypass the price validation:
        adult.getPriceCents() = -100;  // Possible only with an int& getter.

===============================================================================
*/
