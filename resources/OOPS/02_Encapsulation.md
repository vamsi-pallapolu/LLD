# Encapsulation: Let an Object Protect Its Data

**Encapsulation is the bundling of data and the methods that operate on that data into a single unit, such as a class, with controlled access to the unit's internal state.**

**Data hiding** is the restriction of direct access to an object's internal data. In C++, access specifiers such as `private` support data hiding as part of encapsulation.

We continue with the museum's `AdmissionTicket` from [Classes and Objects](01_ClassesAndObjects.md): Maya's ticket starts at 1500 cents and Leo's at 800 cents. This note focuses on the rule that a ticket price must never be negative. Zero is valid for a free ticket.

## 1. Think of changing a ticket at the museum counter

A staff member asks the ticket system to change a price. The system checks the new value before accepting it.

In code:

- The stored price is `private`: outside code cannot change it directly.
- `setPriceCents()` is `public`: outside code can request a change.
- The constructor and setter both reject negative prices.

The object handles the checks, so every place that uses it does not have to repeat them.

## 2. The problem with directly changing data

```cpp
#include <iostream>
#include <string>

struct AdmissionTicket {
    std::string visitor;
    int priceCents;
};

int main() {
    AdmissionTicket adult{"Maya", 1500};
    adult.priceCents = -100;

    std::cout << adult.visitor << ": " << adult.priceCents << " cents\n";
}
```

Output:

```text
Maya: -100 cents
```

There is no check to stop the assignment. If many parts of the program change `priceCents` directly, every one must remember the rule. Forget it once, and the ticket stores an invalid price.

## 3. Put the checks inside the class

This is the same class as in the first note. The new `main()` demonstrates a valid change and a rejected change.

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

    adult.setPriceCents(0);
    std::cout << adult.visitor() << ": " << adult.priceCents() << " cents\n";

    try {
        child.setPriceCents(-100);
    } catch (const std::invalid_argument& error) {
        std::cout << error.what() << '\n';
    }

    std::cout << child.visitor() << ": " << child.priceCents() << " cents\n";
}
```

Output:

```text
Maya: 1500 cents
Leo: 800 cents
Maya: 0 cents
Price must not be negative
Leo: 800 cents
```

### Follow the example

1. Each constructor checks the starting price.
2. `adult.setPriceCents(0)` makes Maya's ticket free. Zero satisfies the rule.
3. `child.setPriceCents(-100)` throws an exception before changing Leo's price.
4. The `catch` block handles that exception and prints its message, so execution continues.
5. Leo's price is still 800 cents.

A negative starting price is rejected too. For example, `AdmissionTicket invalid("Maya", -100)` would throw during construction, so no completed ticket object would be created.

### Read the C++ syntax

| Code | Plain meaning |
|------|---------------|
| `private:` | Outside code cannot directly access the listed data |
| `public:` | Outside code can call the listed functions |
| `: visitor_(visitor), priceCents_(priceCents)` | Initialize the ticket's stored values |
| `priceCents < 0` | Check whether the requested price is negative |
| `void setPriceCents(int priceCents)` | Request a price change without returning a result |
| `int priceCents() const` | Read the price without changing the ticket |
| `throw std::invalid_argument(...)` | Report an invalid request as an exception |
| `try` / `catch` | Attempt an operation and handle the indicated exception |

An unhandled exception stops the program. The ending `_` in `priceCents_` is just a naming choice for data stored inside the object.

### What can outside code do?

It can request a checked change through `setPriceCents()`, or read values through `visitor()` and `priceCents()`.

This direct change is intentionally invalid and is rejected by the compiler:

```cpp
child.priceCents_ = -100; // Error: priceCents_ is private.
```

The checks belong inside the ticket. Code using it only needs to request the change and handle a possible rejection.

## 4. Private alone is not enough

Suppose we replace the checked setter with this unchecked version:

```cpp
void setPriceCents(int priceCents) {
    priceCents_ = priceCents;
}
```

Now `child.setPriceCents(-100)` accepts a bad value even though the data is still private.

A function that sets a value is often called a **setter**. Setters are useful when callers need to choose a value, but they must enforce the rules. The checked version in the complete program does that before changing the stored price.

**A class invariant is a condition that must hold for every valid object of a class.** Here, it is `priceCents_ >= 0`. The constructor establishes it, and public operations preserve it.

Every way of changing the data must follow the rules. Private access by itself cannot guarantee this.

## 5. Reading data without giving away control

The example's `priceCents()` function returns a copy of the stored number:

```cpp
int chosen = child.priceCents();
chosen = -100;
```

Changing `chosen` does not change `child`. They are separate values.

Be careful about replacing the getter with this version:

```cpp
int& priceCents() {
    return priceCents_;
}
```

The `&` means a **reference**: another name for the actual stored number. Outside code could then write `child.priceCents() = -100` and bypass the checks.

For a small value such as this price, returning a copy is simple and keeps control inside the class. For larger data, `const&` allows reading existing data without permission to change it through that reference.

## 6. The key intents

### Fewer checks to repeat

Callers use `setPriceCents()` and handle rejection. They do not have to duplicate its validation to protect the ticket.

### Easier changes

Suppose the ticket later calculates its price instead of storing it. If `priceCents()` still returns a nonnegative price in cents, code that reads it can keep working. We make that change in the inheritance and polymorphism notes.

The setter belongs to this editable-price version. A future fixed-category design can remove price editing; callers that use the setter would then need to change.

### A clearer place to look for mistakes

If the stored price becomes negative, inspect the class's constructor and functions that can change it. The allowed ways to change the value are gathered in one place.

## 7. Ask for an action instead of repeating its steps

Without a checked operation, outside code might repeat:

```text
Check whether the requested ticket price is negative.
If it is, report an error.
Otherwise, store the new price.
```

With the class, it calls:

```cpp
child.setPriceCents(800);
```

The class handles validation. Reading is still useful: a receipt needs `priceCents()` to display the price. Encapsulation keeps the rules for changing it inside the ticket.

## 8. How is this different from abstraction?

| Idea | What it helps with | Ticket example |
|------|--------------------|----------------|
| **Encapsulation** | Bundling data and methods with controlled access to state | `priceCents_` is private and changes go through a checked setter |
| **Abstraction** | Exposing essential operations while hiding implementation details | A receipt calls `priceCents()` without knowing how the price is stored or calculated |

They work together. Encapsulation establishes the boundary around data and its operations; abstraction exposes essential operations without requiring knowledge of their implementation.

See [Abstraction](03_Abstraction.md) for the next topic.

## 9. Common mistakes

- **Checking changes but not starting values:** the constructor must reject a negative price too.
- **Adding an unchecked setter:** private data does not help if a public function accepts any value.
- **Returning a mutable reference:** callers can bypass validation.
- **Forgetting the boundary:** zero should succeed, while a negative price should fail without changing the old value.
- **Adding unnecessary restrictions:** freely editable draft receipt text may be fine in a `struct`; ticket price rules need checked operations.

## 10. Before writing the class

1. List the data the object stores.
2. Write its rules in plain words, such as "the price is zero or more cents."
3. Keep data private when outside changes need to be controlled.
4. Check starting values in the constructor.
5. Provide the actions other code needs.
6. Make every state-changing operation preserve the rules.
7. Let callers read what they need without giving them an unchecked way to change it.

## 11. Check your understanding

**Why is a public price variable a problem here?**

<details>
<summary>Answer</summary>

Any code can assign a negative price without checking the rule.

</details>

**Does making the price private automatically fix everything?**

<details>
<summary>Answer</summary>

No. An unchecked public setter or a getter returning `int&` can still let invalid values through.

</details>

**What happens when `child.setPriceCents(-100)` is called?**

<details>
<summary>Answer</summary>

The checked setter throws before assignment. Leo's price stays at 800 cents. The caller must handle the exception if execution should continue.

</details>

**Why return `int` from `priceCents()` instead of `int&`?**

<details>
<summary>Answer</summary>

Returning `int` gives the caller a copy. Returning `int&` lets the caller change the stored price directly.

</details>

**Is a struct always a bad choice?**

<details>
<summary>Answer</summary>

No. It can suit freely editable data. Use checked operations when changes must follow rules; a struct can also have private members and functions if explicitly declared that way.

</details>

## 12. TL;DR

Encapsulation bundles data and operations and controls access to state. Data hiding restricts direct access. Our ticket establishes a nonnegative price in its constructor and preserves that rule through its setter; the getter returns a copy so callers cannot bypass the check.

## Sources and further reading

- Wikipedia: [Encapsulation](https://en.wikipedia.org/wiki/Encapsulation_(computer_programming))
- Martin Fowler: [Keeping actions with the data they use](https://martinfowler.com/bliki/TellDontAsk.html)
- C++ Core Guidelines: [Limit direct access to members](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#rc-private)
- C++ Core Guidelines: [Set up objects completely](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#rc-complete)
