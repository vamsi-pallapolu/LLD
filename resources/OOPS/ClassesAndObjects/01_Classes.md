# Classes & Objects

Source: `OOPS/ClassesAndObjects/Car.h`, `OOPS/ClassesAndObjects/1_Classe.cpp`

## What is a class?

A **class** in C++ is a user-defined type that bundles data (member variables) and behavior (member functions) into a single unit. An **object** is an instance of a class — a concrete value in memory with its own copy of the non-static members. `class` and `struct` differ only in defaults: members and base classes default to `private` in a `class`, and to `public` in a `struct`. Everything else — methods, constructors, access specifiers — works identically for both.

## Header layout & `#pragma once`

`Car.h` is a header file that declares (and here, defines) the `Car` class. `#pragma once` tells the compiler to include the file at most once per translation unit — the modern replacement for include guards (`#ifndef CAR_H / #define CAR_H / #endif`).

```cpp
#pragma once
#include <iostream>
#include <string>

class Car {
    // ...
};
```

## Access specifiers: `private` vs `public`

Members under `private:` are only accessible from inside the class; members under `public:` form the class's external API. The convention in this file is to prefix private data members with an underscore (`_brand`, `_speed`) to distinguish them from parameters and public getters.

```cpp
class Car {
private:
    std::string _brand;
    std::string _model;
    std::string _color;
    int _speed = 0;   // in-class member initializer — default value if the ctor doesn't set it

public:
    // constructors, methods, getters ...
};
```

## Constructors & member initializer lists

A **constructor** runs when an object is created and is responsible for initializing its members. The syntax `: _brand{brand}, _model{model}, _color{color}` after the parameter list is a **member initializer list** — it initializes members directly, before the constructor body runs. This is preferred over assignment inside the body because:
- `const` members and references *must* be initialized here (they can't be assigned to).
- For non-trivial types it avoids a default-construct-then-assign pair.

The braces `{brand}` are **brace initialization** (a.k.a. uniform / list initialization). It disallows narrowing conversions (e.g. `int x{3.14};` fails to compile), unlike `=` or `()` initialization.

```cpp
Car(const std::string &brand, const std::string &model, const std::string &color)
    : _brand{brand}, _model{model}, _color{color} {}
```

Parameters are passed as `const std::string &` — a **const reference** — so the string isn't copied and can't be mutated by the constructor.

## Creating an object

```cpp
Car ob("Toyato", "Hilux", "Red");   // stack-allocated; destructor runs at end of scope
ob.increment(10);
ob.displayStatus();
```

Alternative syntaxes for the same thing:
```cpp
Car ob{"Toyato", "Hilux", "Red"};   // brace init — preferred in modern C++
auto ob = Car{"Toyato", "Hilux", "Red"};
```

Heap-allocated with `new` would be `Car *p = new Car("Toyato", "Hilux", "Red");` — but then *you* are responsible for `delete p` (or use `std::unique_ptr<Car>` / `std::make_unique<Car>(...)`).

## Member functions

Methods are declared inside the class. Any method that does not modify the object's state should be marked `const` — the trailing `const` after the parameter list.

```cpp
void increment(int inc) { _speed += inc; }              // mutates state
void brake(int dec)     { _speed = _speed > dec ? _speed - dec : 0; }

int speed() const { return _speed; }                    // pure getter — const
```

A `const` method can be called on a `const Car`; a non-const one cannot. Forgetting `const` on a getter is a common bug that only surfaces when someone tries to pass your object as `const Car&`.

`brake` uses a ternary to enforce the invariant that `_speed` never drops below zero: `_speed > dec ? _speed - dec : 0`. This is the class-level equivalent of the guard in `BankAccount::withdraw` — the mutator is responsible for keeping the object in a valid state, rather than trusting callers to check first. Note that `_speed` is `int`, so without the guard `_speed -= dec` would silently produce a negative speed.

## Getters returning `const auto &`

```cpp
const auto &brand() const { return _brand; }
```

- `auto` lets the compiler deduce the return type (`std::string` here).
- `const &` returns a reference to the internal string without copying and prevents the caller from mutating it.
- The trailing `const` marks the method as non-mutating.

If you returned by value (`std::string brand() const`), each call would copy the string. Note the `const` on the return type is technically redundant here — because the method itself is `const`, `this` is `const Car *` and `auto &` already deduces to `const std::string &`. The redundancy becomes protection if you later drop the trailing `const` on the method: without the return-type `const`, a caller could then write `ob.brand() = "X";` and mutate the private field.

## `displayStatus` — a `const` output method

```cpp
void displayStatus() const {
    std::cout << "brand " << _brand << " is running at " << _speed << "km/hr";
}
```

Reads state, doesn't modify it → `const`. `std::cout` is the standard output stream from `<iostream>`; `<<` is the stream-insertion operator.

## Common patterns for a class definition

| Pattern | When to use |
| --- | --- |
| `private:` for data, `public:` for methods | Default encapsulation |
| Initializer list `: x{a}, y{b}` | Always for non-trivial member init |
| `const std::string &` parameters | Non-owning, non-mutating string input |
| `const` on read-only methods | Enables use through `const` references |
| `const auto &` return for owned data | Zero-copy read access to internal state |
| `#pragma once` at top of headers | Prevent double inclusion |

## Gotchas

- **Missing `const` on getters** silently breaks usage from `const` contexts. Add it by default on any method that doesn't mutate.
- **Returning a reference to a local** (`const std::string &f() { std::string s = ...; return s; }`) is undefined behavior — the local dies at return. Only return references to members or to objects with a longer lifetime.
- **`class` vs `struct`** differ only in default member access and default base-class access (both `private` for `class`, both `public` for `struct`). Everything else is identical — including the ability to have methods, constructors, etc.
- **In-class member initializers** (`int _speed = 0;`) run *before* the constructor body, but the initializer list overrides them if it also mentions that member.
- **Braces vs parens for construction**: `std::vector<int> v(10, 1);` gives a vector of ten `1`s; `std::vector<int> v{10, 1};` gives a vector of `{10, 1}` (two elements). Brace init prefers `std::initializer_list` constructors when one exists.
