# Classes and Objects: Keep Related Things Together

**A class describes what an object stores and what it can do.** `AdmissionTicket` describes a ticket with a visitor name, a price, and functions for reading or changing those details.

**An object is one actual instance of a class.** Maya's ticket and Leo's ticket are two objects created from the same `AdmissionTicket` class. Each stores its own name and price.

C++ also calls values such as an `int` objects. This chapter focuses on objects created from classes.

All five notes use one example: **museum admission tickets**. Maya's adult ticket costs 1500 cents, Leo's child ticket costs 800 cents, and later Nora's senior ticket costs 1000 cents. Prices use whole cents: 1500 cents means 15 dollars. Storing these amounts as integers avoids rounding caused by decimal floating-point numbers. We assume the ticket category was chosen correctly at purchase; these examples do not check ages or take payments.

We develop the design in stages. Each complete program stands on its own; snippets labeled as additions belong in that chapter's program.

| Note | What the ticket example teaches |
|------|---------------------------------|
| Classes and objects | Create separate tickets from one class |
| [Encapsulation](02_Encapsulation.md) | Keep ticket prices valid through checked operations |
| [Abstraction](03_Abstraction.md) | Print a receipt using a ticket's public operations |
| [Inheritance](04_Inheritance.md) | Share admission rules across ticket kinds and add guided-tour check-in |
| [Polymorphism](05_Polymorphism.md) | Let one receipt function get each ticket kind's own price |


## Contents

1. [Think of a ticket template](#1-think-of-a-ticket-template)
2. [Why do we need classes?](#2-why-do-we-need-classes)
3. [A complete example with a class](#3-a-complete-example-with-a-class)
4. [One class, separate objects](#4-one-class-separate-objects)
5. [Copies and references](#5-copies-and-references)
6. [When does an object go away?](#6-when-does-an-object-go-away)
7. [The key intents](#7-the-key-intents)
8. [Common mistakes](#8-common-mistakes)
9. [Before writing a class](#9-before-writing-a-class)
10. [Check your understanding](#10-check-your-understanding)
11. [Short recap](#11-short-recap)

## 1. Think of a ticket template

A museum uses one ticket template with spaces for a visitor's name and a price. It can issue many tickets using that template.

The template is like a **class**. Each issued ticket is like an **object**.

Maya's and Leo's tickets have the same fields, but their values are separate. Changing Maya's ticket does not change Leo's.

## 2. Why do we need classes?

Suppose you track two tickets without a class:

```cpp
#include <iostream>
#include <string>

int main() {
    std::string adultVisitor = "Maya";
    int adultPriceCents = 1500;

    std::string childVisitor = "Leo";
    int childPriceCents = 800;

    childPriceCents = adultPriceCents; // Bug: copied the wrong price.
    adultPriceCents = -100;           // Bug: a price cannot be negative.

    std::cout << adultVisitor << ": " << adultPriceCents << " cents\n";
    std::cout << childVisitor << ": " << childPriceCents << " cents\n";
}
```

Output:

```text
Maya: -100 cents
Leo: 1500 cents
```

The variables are separate. Nothing groups each visitor with their price or checks the price when it changes.

A class groups each visitor name with its price and can provide a function that checks price changes. Our class below rejects negative prices.

That check cannot catch every mistake: 1500 is a valid price even if you meant to set 800. The later chapters give adult and child tickets separate functions that return their fixed prices.

## 3. A complete example with a class

This program creates two tickets and prints their details. It requires C++11 or newer.

Before reading it, know that `public:` lists functions callers may use, and `private:` lists fields callers cannot read or change directly. `std::string` stores text, `int` stores a whole number, and `std::cout` prints output.

`const std::string&` reads the supplied text through a reference without changing it; the constructor then stores its own copy. A reference is another name for an existing value. The trailing `const` on a getter means that function does not change the ticket's fields.

`throw std::invalid_argument(...)` reports an invalid price as an error. That error stops ticket creation or the requested price change. Section 3 explains how this works.

```cpp
#include <iostream>
#include <stdexcept>
#include <string>

class AdmissionTicket {
public:
    AdmissionTicket(const std::string& visitor, int priceCents)
        : visitor_(visitor), priceCents_(priceCents) {
        if (priceCents_ < 0) {
            throw std::invalid_argument("Price must not be negative"); // Reject this starting price.
        }
    }

    void setPriceCents(int priceCents) {
        if (priceCents < 0) {
            throw std::invalid_argument("Price must not be negative"); // Leave the old price unchanged.
        }
        priceCents_ = priceCents;
    }

    std::string visitor() const {
        return visitor_;
    }

    int priceCents() const {
        return priceCents_;
    }

private:
    std::string visitor_;
    int priceCents_;
};

int main() {
    AdmissionTicket adult("Maya", 1500);
    AdmissionTicket child("Leo", 800);

    std::cout << adult.visitor() << ": " << adult.priceCents() << " cents\n";
    std::cout << child.visitor() << ": " << child.priceCents() << " cents\n";
}
```

Output:

```text
Maya: 1500 cents
Leo: 800 cents
```

### 3.1 Read the class from the inside out

Each ticket stores two values:

| Name | Meaning |
|------|---------|
| `visitor_` | The visitor's name |
| `priceCents_` | The ticket's price in whole cents |

**Data members** are variables stored inside an object. Maya's ticket has its own `visitor_` and `priceCents_`; Leo's ticket has a separate pair.

These fields are called **non-static** because each object has its own values. A field declared `static` would instead be shared by the class. We do not use static fields here. The trailing `_` is a naming choice, not a C++ requirement.

**Member functions**, also called **methods**, are functions that belong to a class and work with its objects. `visitor()` returns the name, `priceCents()` returns the price, and `setPriceCents()` checks and changes the price.

A function that reads a stored value is often called a **getter**. A function that sets a stored value is often called a **setter**.

### 3.2 How a ticket is created

```cpp
AdmissionTicket adult("Maya", 1500);
```

Read this as: create an `AdmissionTicket` object named `adult`, for Maya, with a price of 1500 cents. At this stage, `adult` and `child` are variable names, not different C++ types.

**A constructor sets up a newly created object.** It has the same name as the class and no return type. Here, `AdmissionTicket(...)` receives the visitor name and starting price.

The part after the constructor's colon is its **initializer list**. `visitor_(visitor)` stores the supplied name, and `priceCents_(priceCents)` stores the supplied price before the code inside the braces runs.

If the price is negative, `throw std::invalid_argument(...)` reports an error and stops creation of the ticket. This kind of error is an **exception**. If the program does not handle it, the program stops. Zero is allowed for a free ticket.

### 3.3 What happens when we use the ticket?

1. `AdmissionTicket adult("Maya", 1500)` creates Maya's ticket.
2. `AdmissionTicket child("Leo", 800)` creates Leo's separate ticket.
3. `adult.visitor()` returns Maya's name.
4. `adult.priceCents()` returns 1500.
5. The same functions on `child` return Leo's name and 800.

The dot in `adult.priceCents()` means **call this function on this object**. It returns 1500; it does not print that number. The surrounding `std::cout` line prints the returned value. `\n` starts the next output line.

### 3.4 What do the other keywords mean?

| Code | Plain meaning |
|------|---------------|
| `public:` | Code outside the class can use these functions |
| `private:` | Outside code cannot directly access these stored values |
| `void setPriceCents(...)` | Change the price without returning a result |
| `int priceCents() const` | Return a whole number without changing the ticket |
| `std::string` | The C++ type used here to store text |
| `const std::string& visitor` | Receive the caller's text without copying or changing it; the constructor then copies it into `visitor_` |

Choosing which data outside code can change is covered in [Encapsulation](02_Encapsulation.md).

## 4. One class, separate objects

`adult` and `child` use the same functions but have separate stored values.

| Object | Visitor | Price |
|--------|---------|-------|
| `adult` | Maya | 1500 cents |
| `child` | Leo | 800 cents |

Adding another ticket takes one line:

```cpp
AdmissionTicket senior("Nora", 1000);
```

You do not need to write another `visitor()` or `priceCents()` function.

## 5. Copies and references

The snippets below use the class from section 3. Put object creation and printing lines inside `main()`. Put the `makeFree()` function definition before `main()`. Each experiment starts with a fresh ticket.

For this class, copying a ticket creates a separate object with the same starting values:

```cpp
AdmissionTicket original("Maya", 1500);
AdmissionTicket copy = original;
copy.setPriceCents(1000);

std::cout << original.priceCents() << ' ' << copy.priceCents();
```

This prints `1500 1000`. Changing the copy does not change the original.

A function parameter without `&` receives a copy of the ticket. This is called **passing by value**:

```cpp
void makeFree(AdmissionTicket ticket) {
    ticket.setPriceCents(0);
}
```

`ticket` is a copy. Calling `makeFree(original)` will not change `original`.

To work with the original ticket, replace that function with:

```cpp
void makeFree(AdmissionTicket& ticket) {
    ticket.setPriceCents(0);
}
```

A **reference**, written with `&`, is another name for an existing object. Now `makeFree(original)` changes the original ticket's price to zero.

Use `const AdmissionTicket&` when a function should read the existing ticket without changing it or making a copy.

## 6. When does an object go away?

A local object such as `adult` in `main()` is cleaned up when execution leaves the block where it was created.

**A destructor cleans up an object when it goes away.** Its name is the class name with `~` in front: `~AdmissionTicket()`.

C++ generates the destructor we need here, including cleanup of the stored string. In [Polymorphism](05_Polymorphism.md), we add `virtual` to the destructor so cleanup also works when a derived ticket is deleted through a base pointer.

### 6.1 References do not keep an object alive

A function receiving `const AdmissionTicket&` uses the caller's existing ticket. That parameter does not create or delete the ticket, and it does not extend the ticket's lifetime.

For example, `makeFree(original)` uses a reference while `original` still exists. If code saves that reference and uses it after `original` has gone away, the reference no longer points to a live object.

That is called a **dangling reference**. Using it has **undefined behavior**, meaning C++ promises no predictable result. A reference gives access to an object; it does not keep the object alive.

## 7. The key intents

### 7.1 Keep data and the actions on it together

A ticket's price and the checked operation that changes it live in one place.

### 7.2 Make a new type that reads like the problem

`AdmissionTicket adult(...)` and `adult.priceCents()` describe museum tickets directly.

### 7.3 Create many objects from one description

Write the rules once. Every ticket follows them, and each keeps its own values.

### 7.4 Start every object in a valid state

The constructor initializes the name and price, and rejects negative prices.

## 8. Common mistakes

### 8.1 Forgetting starting values

Give every stored number a starting value. Writing `int priceCents_;` alone does not guarantee zero. Our constructor sets it from the supplied price, so later reads use a known value.

### 8.2 Letting one class do too much

A ticket stores ticket details and enforces its rules. Saving files, sending receipts, and drawing screens can be handled separately.

### 8.3 Expecting a copy to change the original

Use `AdmissionTicket&` when a function must change an existing ticket.

### 8.4 Forgetting const on functions that only read

`priceCents() const` can be called through a `const AdmissionTicket&`. Without the ending `const`, C++ rejects that call because the function has not promised to leave the ticket unchanged.

### 8.5 Choosing class or struct by habit

Both can store data and have functions. Members of a `struct` are public by default; members of a `class` are private by default.

A `struct` can be useful for a freely editable draft of receipt text. A `class` is useful for a ticket whose price changes need checks.

## 9. Before writing a class

1. Name the thing you are describing.
2. List the values each object needs to store.
3. List the actions it needs to provide.
4. Give every stored value a sensible starting value.
5. Check rules when the object is created and when its data changes.
6. Use `const` on functions that only read.
7. Keep unrelated work outside the class.

## 10. Check your understanding

**What is the difference between a class and an object?**

<details>
<summary>Answer</summary>

`AdmissionTicket` is the description of a ticket. `adult` and `child` are two actual tickets made from that description, each with its own visitor and price.

</details>

**If you change Maya's price, does Leo's price change too?**

<details>
<summary>Answer</summary>

No. Each ticket has its own `priceCents_`.

</details>

**What does the constructor do?**

<details>
<summary>Answer</summary>

It initializes the visitor and price, and rejects a negative price.

</details>

**Why does `makeFree(AdmissionTicket ticket)` leave the original unchanged?**

<details>
<summary>Answer</summary>

It receives a copy. Use `AdmissionTicket&` to change the original.

</details>

## 11. Short recap

Write the ticket's data and functions once in `AdmissionTicket`. Create separate objects for Maya and Leo. The constructor gives each ticket its starting values; a destructor handles cleanup when the ticket goes away.

## Sources and further reading

- Wikipedia: [Classes](https://en.wikipedia.org/wiki/Class_(computer_programming))
- cppreference: [C++ classes](https://en.cppreference.com/w/cpp/language/classes)
- C++ Core Guidelines: [Organize related data together](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#rc-org)
- C++ Core Guidelines: [Set up objects completely](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#rc-complete)
