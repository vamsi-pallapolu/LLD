# Classes and Objects: Keep Related Things Together

**A class is a user-defined type that encapsulates data members and member functions, defining the state and behavior of its instances.**

**An object is an instance of a class, with its own identity and state and the behavior defined by that class.** In C++, the term *object* also applies to instances of non-class types, such as an `int`; this note focuses on class objects.

All five notes use one example: **museum admission tickets**. Maya's adult ticket costs 1500 cents, Leo's child ticket costs 800 cents, and later Nora's senior ticket costs 1000 cents. Prices use whole cents to avoid decimal rounding problems. We assume the ticket category was chosen correctly at purchase; these examples do not check ages or take payments.

We develop the design in stages. Each complete program stands on its own; snippets labeled as additions belong in that chapter's program.

| Note | What the ticket example teaches |
|------|---------------------------------|
| Classes and objects | Create separate tickets from one class |
| [Encapsulation](02_Encapsulation.md) | Keep ticket prices valid through checked operations |
| [Abstraction](03_Abstraction.md) | Print a receipt using a ticket's public operations |
| [Inheritance](04_Inheritance.md) | Model adult and child tickets as kinds of admission ticket |
| [Polymorphism](05_Polymorphism.md) | Ask different ticket kinds for their prices through one interface |

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

A class keeps each ticket's data and the functions that use it together. It can reject negative prices, although it cannot detect every mistake: 1500 is a valid number even if you intended 800. Later notes give each ticket category its own price behavior.

## 3. A complete example with a class

```cpp
#include <iostream>
#include <stdexcept>
#include <string>

class AdmissionTicket {
public:
    AdmissionTicket(const std::string& visitor, int priceCents)
        : visitor_(visitor), priceCents_(priceCents) {
        if (priceCents_ < 0) {
            throw std::invalid_argument("Price must not be negative");
        }
    }

    void setPriceCents(int priceCents) {
        if (priceCents < 0) {
            throw std::invalid_argument("Price must not be negative");
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

### Read the class from the inside out

Each ticket stores two values:

| Name | Meaning |
|------|---------|
| `visitor_` | The visitor's name |
| `priceCents_` | The ticket's price in whole cents |

**Data members** are variables declared as members of a class to represent its data. These are non-static data members, so each object has its own values. The ending `_` is a naming choice to help recognize them.

**Member functions**, also called **methods**, are functions declared as members of a class that define its operations. Here, `visitor()` and `priceCents()` read ticket details, and `setPriceCents()` changes a price after checking it.

### How a ticket is created

```cpp
AdmissionTicket adult("Maya", 1500);
```

Read this as: create an `AdmissionTicket` object named `adult`, for Maya, with a price of 1500 cents. At this stage, `adult` and `child` are variable names, not different C++ types.

**A constructor is a special member function invoked to initialize an object of its class.** It has the same name as the class and no return type. Here, it is named `AdmissionTicket`.

The part after `:` gives the stored variables their starting values. Both members are initialized before the constructor body runs.

If the price is negative, `throw std::invalid_argument(...)` reports an error and stops creation of the ticket. This kind of error is an **exception**. If the program does not handle it, the program stops. Zero is allowed for a free ticket.

### What happens when we use the ticket?

1. `AdmissionTicket adult("Maya", 1500)` creates Maya's ticket.
2. `AdmissionTicket child("Leo", 800)` creates Leo's separate ticket.
3. `adult.visitor()` returns Maya's name.
4. `adult.priceCents()` returns 1500.
5. The same functions on `child` return Leo's name and 800.

The dot in `adult.priceCents()` means **call this function on this object**.

### What do the other keywords mean?

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

The following snippets are separate experiments using the class above.

For this class, copying a ticket creates a separate object with the same starting values:

```cpp
AdmissionTicket original("Maya", 1500);
AdmissionTicket copy = original;
copy.setPriceCents(1000);

std::cout << original.priceCents() << ' ' << copy.priceCents();
```

This prints `1500 1000`. Changing the copy does not change the original.

The same thing happens when a function receives a ticket by value:

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

**A destructor is a special member function invoked when an object's lifetime ends to perform cleanup.** Its name is the class name prefixed with `~`: here, `~AdmissionTicket()`.

We do not need to write one in this version. C++ provides it and handles cleanup of the ticket's stored values, including its string. The polymorphism note will add a virtual destructor when introducing a different use of the class.

## 7. The key intents

### Keep data and the actions on it together

A ticket's price and the checked operation that changes it live in one place.

### Make a new type that reads like the problem

`AdmissionTicket adult(...)` and `adult.priceCents()` describe museum tickets directly.

### Create many objects from one description

Write the rules once. Every ticket follows them, and each keeps its own values.

### Start every object in a valid state

The constructor initializes the name and price, and rejects negative prices.

## 8. Common mistakes

### Forgetting starting values

Initialize stored numbers in the class or constructor. A declaration like `int priceCents_;` alone does not always start at zero. Our constructor explicitly initializes it.

### Letting one class do too much

A ticket stores ticket details and enforces its rules. Saving files, sending receipts, and drawing screens can be handled separately.

### Expecting a copy to change the original

Use `AdmissionTicket&` when a function must change an existing ticket.

### Forgetting const on functions that only read

`priceCents() const` can be called through a `const AdmissionTicket&`. Without the ending `const`, C++ rejects that call because the function has not promised to leave the ticket unchanged.

### Choosing class or struct by habit

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

`AdmissionTicket` is a class defining ticket data and operations. `adult` and `child` are separate objects of that class, each with its own identity and state.

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

## 11. TL;DR

A class groups data members and member functions into a user-defined type. Each object has its own non-static data members. Constructors initialize objects; destructors perform cleanup when their lifetimes end. Here, `AdmissionTicket` defines the type, and Maya's and Leo's tickets are separate objects.

## Sources and further reading

- Wikipedia: [Classes](https://en.wikipedia.org/wiki/Class_(computer_programming))
- cppreference: [C++ classes](https://en.cppreference.com/w/cpp/language/classes)
- C++ Core Guidelines: [Organize related data together](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#rc-org)
- C++ Core Guidelines: [Set up objects completely](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#rc-complete)
