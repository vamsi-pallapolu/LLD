# Classes and Objects: Keep Related Things Together

**A class is a user-defined type that encapsulates data members and member functions, defining the state and behavior of its instances.**

**An object is an instance of a class, with its own identity and state and the behavior defined by that class.** In C++, the term *object* also applies to instances of non-class types, such as an `int`; this note focuses on class objects.

For example, `WaterBottle` can describe water bottles in your program. Your gym bottle and desk bottle are two separate objects made using that class.

## 1. Think of a house plan

A house plan describes the rooms a house will have. You can build several houses from the same plan.

The plan is like a **class**. Each house is like an **object**.

The houses follow the same plan, but they are separate. Painting one house does not paint all the others.

Likewise, two water-bottle objects use the same class, but each keeps track of its own water.

## 2. Why do we need classes?

Suppose you want to track two bottles without a class:

```cpp
#include <iostream>

int main() {
    int gymCapacityMl = 750;
    int gymWaterMl = 0;

    int deskCapacityMl = 500;
    int deskWaterMl = 0;

    gymWaterMl = gymCapacityMl;
    deskWaterMl = gymCapacityMl; // Bug: should be deskCapacityMl.

    gymWaterMl -= 1000; // More water than the bottle contains.

    std::cout << "Gym: " << gymWaterMl << " ml\n";
    std::cout << "Desk: " << deskWaterMl << " ml\n";
}
```

This prints:

```text
Gym: -250 ml
Desk: 750 ml
```

A bottle cannot hold negative water, and the desk bottle should hold only 500 ml.

The variables are separate. Nothing in this code connects each bottle's water amount to its capacity or checks the rules when the amount changes.

A class lets us keep each bottle's data and the functions that use it together.

## 3. A complete example with a class

```cpp
#include <iostream>
#include <stdexcept>
#include <string>

class WaterBottle {
public:
    WaterBottle(std::string label, int capacityMl)
        : label_(label), capacityMl_(capacityMl) {
        if (capacityMl_ <= 0) {
            throw std::invalid_argument("Capacity must be positive");
        }
    }

    void fill() {
        waterMl_ = capacityMl_;
    }

    int drink(int requestedMl) {
        if (requestedMl <= 0) {
            return 0;
        }

        int amount = requestedMl;
        if (amount > waterMl_) {
            amount = waterMl_;
        }

        waterMl_ -= amount;
        return amount;
    }

    int waterMl() const {
        return waterMl_;
    }

    std::string label() const {
        return label_;
    }

private:
    std::string label_;
    int capacityMl_;
    int waterMl_ = 0;
};

int main() {
    WaterBottle gym("Gym bottle", 750);
    WaterBottle desk("Desk bottle", 500);

    gym.fill();
    desk.fill();

    int amount = gym.drink(1000);

    std::cout << gym.label() << ": drank " << amount << " ml, "
              << gym.waterMl() << " ml left\n";
    std::cout << desk.label() << ": " << desk.waterMl() << " ml left\n";
}
```

Output:

```text
Gym bottle: drank 750 ml, 0 ml left
Desk bottle: 500 ml left
```

### Read the class from the inside out

Each bottle stores three values:

| Name | Meaning |
|------|---------|
| `label_` | The bottle's name |
| `capacityMl_` | How much water it can hold |
| `waterMl_` | How much water it currently holds |

**Data members** are variables declared as members of a class to represent its data. These are non-static data members, so each object has its own values. The ending `_` is just a naming choice to help recognize them.

**Member functions**, also called **methods**, are functions declared as members of a class that define its operations. In this example, they provide the actions available on each bottle.

### How a bottle is created

```cpp
WaterBottle gym("Gym bottle", 750);
```

Read this as: create a `WaterBottle` named `gym`, with the label "Gym bottle" and a capacity of 750 ml.

**A constructor is a special member function invoked to initialize an object of its class.** In C++, it has the same name as the class and no return type. Here, the constructor is named `WaterBottle`.

The part after `:` gives the stored variables their starting values. `waterMl_ = 0` makes every new bottle start empty.

If the capacity is zero or negative, `throw std::invalid_argument(...)` reports an error and stops creation of that bottle. This kind of error is called an **exception**. If the program does not handle it, the program stops.

### What happens when we use the bottle?

1. `gym.fill()` sets the gym bottle's water to its capacity: 750 ml.
2. `desk.fill()` uses the desk bottle's capacity: 500 ml.
3. `gym.drink(1000)` asks for 1000 ml.
4. The gym bottle has only 750 ml, so `drink()` removes 750 and returns that amount.
5. The gym bottle is now empty. The desk bottle still has 500 ml.

The dot in `gym.fill()` means **call this function on this object**.

### What do the other keywords mean?

| Code | Plain meaning |
|------|---------------|
| `public:` | Code outside the class can use these functions |
| `private:` | Outside code cannot directly access these stored values |
| `void fill()` | `fill()` does its work without returning a result |
| `int drink(...)` | `drink()` returns a whole number |
| `int waterMl() const` | This function reads the amount without changing the bottle |
| `std::string` | The C++ type used here to store text |

Classes let us put these pieces together. Choosing which data outside code can change is covered in [Encapsulation](02_Encapsulation.md).

## 4. One class, separate objects

`gym` and `desk` use the same functions, but have separate stored values.

| Object | Capacity | Water after the example |
|--------|----------|-------------------------|
| `gym` | 750 ml | 0 ml |
| `desk` | 500 ml | 500 ml |

Adding another bottle takes one line:

```cpp
WaterBottle travel("Travel bottle", 1000);
```

You do not need to write another `fill()` or `drink()` function.

## 5. Copies and references

For this class, copying a bottle creates a separate bottle with the same starting values:

```cpp
WaterBottle original("Original", 500);
original.fill();

WaterBottle copy = original;
copy.drink(100);

std::cout << original.waterMl() << ' ' << copy.waterMl();
```

This prints `500 400`. Changing the copy does not change the original.

The same thing happens when a function receives a bottle like this:

```cpp
void sip(WaterBottle bottle) {
    bottle.drink(100);
}
```

`bottle` is a copy. Calling `sip(original)` will not change `original`.

To work with the original bottle, use `&`:

```cpp
void sip(WaterBottle& bottle) {
    bottle.drink(100);
}
```

This is called a **reference**: another name for an existing object. Now `sip(original)` changes the original bottle.

Use `const WaterBottle&` when the function should read the existing bottle without changing it or making a copy.

## 6. When does an object go away?

A local object such as `gym` in `main()` is cleaned up when execution leaves the block where it was created.

**A destructor is a special member function invoked when an object's lifetime ends to perform cleanup.** In C++, its name is the class name prefixed with `~`. For `WaterBottle`, its name would be `~WaterBottle()`.

We do not need to write one here. C++ provides it and handles cleanup of the bottle's stored values, including its string.

For now, creating objects as local variables is a simple way to let C++ handle their cleanup.

## 7. The key intents

### Keep data and the actions on it together

A bottle's water amount and the `drink()` rule that changes it live in one place. You do not have to search the program to find every piece of code that touches the water.

### Make a new type that reads like the problem

`WaterBottle gym(...)` and `gym.fill()` say what the program is about. Separate `int` variables do not.

### Create many objects from one description

Write the rules once in the class. Every bottle you create follows them, and each one keeps its own values.

### Start every object in a valid state

The constructor runs when each bottle is created, so a bottle with zero or negative capacity is rejected.

## 8. Common mistakes

### Forgetting starting values

Give stored numbers a starting value in the class or constructor. A declaration like `int waterMl_;` alone does not always start at zero.

### Letting one class do too much

A water bottle should track its water and the actions on it. Saving files, sending reminders, and drawing screens can be handled separately.

A class is easier to understand when it has one clear purpose.

### Expecting a copy to change the original

Use a reference, such as `WaterBottle&`, when a function must change an existing bottle.

### Forgetting const on functions that only read

`waterMl() const` can be called through a `const WaterBottle&`. Without the ending `const`, C++ will reject that call because the function has not promised to leave the bottle unchanged.

### Choosing class or struct by habit

Both can store data and have functions. Members of a `struct` are public unless you say otherwise; members of a `class` are private unless you say otherwise.

A `struct` is often convenient for simple values that other code can edit directly, such as a point's `x` and `y`. A `class` is often useful when changes need checks, as with the water amount.

## 9. Before writing a class

1. Name the thing you are describing.
2. List the values each object needs to store.
3. List the actions it needs to provide.
4. Give every stored value a sensible starting value.
5. Check any rules when the object is created and when its data changes.
6. Use `const` on functions that only read.
7. Keep unrelated work outside the class.

## 10. Check your understanding

**What is the difference between a class and an object?**

<details>
<summary>Answer</summary>

A class is a user-defined type that defines the data and operations of its instances. An object is an instance of that class, with its own identity and state. `WaterBottle` is a class; `gym` and `desk` are objects.

</details>

**If you fill the gym bottle, does the desk bottle fill too?**

<details>
<summary>Answer</summary>

No. Each bottle has its own water amount.

</details>

**What does the constructor do?**

<details>
<summary>Answer</summary>

A constructor initializes an object of its class. Our constructor stores the label and capacity, and rejects a capacity that is zero or negative.

</details>

**Why does sip(WaterBottle bottle) leave the original bottle unchanged?**

<details>
<summary>Answer</summary>

The function receives a copy. Use WaterBottle& if the function should change the original.

</details>

## 11. TL;DR

A class defines a user-defined type by grouping data members and member functions. Each object is an instance of that class with its own non-static data members. Constructors initialize objects, and destructors perform cleanup when their lifetimes end.

## Sources and further reading

- Wikipedia: [Classes](https://en.wikipedia.org/wiki/Class_(computer_programming))
- cppreference: [C++ classes](https://en.cppreference.com/w/cpp/language/classes)
- C++ Core Guidelines: [Organize related data together](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#rc-org)
- C++ Core Guidelines: [Set up objects completely](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#rc-complete)
