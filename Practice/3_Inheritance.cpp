/*
    Inheritance concepts
*/

// Without Inheritance
#include <iostream>
#include <string>

class Animal
{
private:
    std::string _name{};
    std::string _species{};

public:
    Animal(const std::string &name) : _name{name}
    {
    }

    ~Animal() = default;

    void eat() const
    {
        std::cout << _name << " eats food." << std::endl;
    }
};

class Dog
{
private:
    std::string _name{};

public:
    Dog(const std::string &name) : _name{name} {}

    ~Dog() = default;

    void eat() const
    {
        std::cout << _name << " eats food." << std::endl;
    }

    void barks() const
    {
        std::cout << _name << " barks." << std::endl;
    }
};

// Problem
// We have to write eat twice,
// if shred behavior changes, both copies needs updating

// With Inheritance

class Cat : public Animal
{
private:
    std::string _name{};

public:
    Cat(const std::string &name) : Animal{name}, _name{name}
    {
    }

    void crawl() const
    {
        std::cout << _name << "  crawls" << std::endl;
    }

    void eat() const
    {
        std::cout << _name << " eats mice." << std::endl;
    }
};

int main()
{
    std::string dog_name = "Bob";
    Dog dog(dog_name);
    dog.eat();
    dog.barks();

    // With inheritance
    Cat cat("Little");
    cat.eat();
    cat.crawl();

    Animal* animal = new Cat("Sten");
    animal->eat(); // still call eat() from base because it is not virtual

    return 0;
}