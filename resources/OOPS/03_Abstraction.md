# Abstraction: Show What Matters

**Abstraction is the process of representing the essential features of an entity while hiding implementation details that are unnecessary to its users.** It emphasizes what an entity does rather than how it does it [1].

We continue with the same museum admission tickets: Maya pays 1500 cents and Leo pays 800 cents. A receipt needs each visitor's name and price. It does not need to know how a ticket stores those details.

## 1. Start with the museum receipt

At the museum counter, the receipt printer asks a ticket for:

- The visitor's name: `visitor()`.
- The price in cents: `priceCents()`.

Those operations describe **what the printer needs**. The ticket handles **how to supply it**.

**Hiding details does not mean the code must be secret.** It means you do not need to understand those details to use the ticket.

### You already use abstraction in code

```cpp
printReceipt(adult);
```

Someone reading this line understands the request: print a receipt for Maya's ticket. They do not need to read its formatting steps at every call.

A well-named function is already an abstraction. You do not need special C++ keywords every time you use this idea.

## 2. Bad vs. good: a receipt that depends on ticket storage

### A receipt that reads internal fields

This complete program works:

```cpp
#include <iostream>
#include <string>

struct AdmissionTicket {
    std::string visitor;
    int priceCents;
};

void printReceipt(const AdmissionTicket& ticket) {
    std::cout << ticket.visitor << ": " << ticket.priceCents << " cents\n";
}

int main() {
    AdmissionTicket adult{"Maya", 1500};
    AdmissionTicket child{"Leo", 800};

    printReceipt(adult);
    printReceipt(child);
}
```

Output:

```text
Maya: 1500 cents
Leo: 800 cents
```

The receipt depends directly on the ticket's fields. If prices later come from category-specific calculations, code reading the old field must change. The same dependency could appear in refunds and sales reports.

Public data can be appropriate for simple records. Here, we want callers to depend on the meaning of a price operation while the ticket's implementation evolves.

### A receipt that asks for ticket details

Use the same checked class from [Encapsulation](02_Encapsulation.md), and let the receipt call its public operations:

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

void printReceipt(const AdmissionTicket& ticket) {
    std::cout << ticket.visitor() << ": "
              << ticket.priceCents() << " cents\n";
}

int main() {
    AdmissionTicket adult("Maya", 1500);
    AdmissionTicket child("Leo", 800);

    printReceipt(adult);
    printReceipt(child);
}
```

Output:

```text
Maya: 1500 cents
Leo: 800 cents
```

`printReceipt()` knows only the operations it needs. Today `priceCents()` reads a member; later each ticket category will provide its own implementation. The receipt function's body will stay the same.

### Read the interface as a promise

**An interface specifies the operations available to clients and the behavioral contract those operations must satisfy, independently of their implementation.** In plain language, it says what you can ask for and what each request means.

| Operation | Promise in this example |
|-----------|-------------------------|
| `visitor() const` | Return the visitor's name without changing the ticket |
| `priceCents() const` | Return a nonnegative price in whole cents without changing the ticket |
| `setPriceCents(int)` | In this editable-price version, accept a nonnegative price or throw before changing it |

The receipt uses only the first two operations. It does not need permission to change the price.

C++ has no separate `interface` keyword. A class's public operations form an interface; an abstract class is another tool for defining shared operations [3][4].

### Follow the first call

1. `printReceipt(adult)` receives Maya's existing ticket through `const AdmissionTicket&`.
2. The reference avoids copying; `const` prevents the function from changing the ticket through that reference.
3. `ticket.visitor()` supplies `"Maya"`.
4. `ticket.priceCents()` supplies 1500.
5. The receipt formats those results without touching the stored fields.

The reference is borrowed for the call. It neither creates nor deletes the ticket. In this program, both ticket objects live until `main()` ends, so they exist throughout each call.

### How the example develops in the next notes

[Inheritance](04_Inheritance.md) introduces `AdultTicket` and `ChildTicket`, each derived from `AdmissionTicket`. The base keeps the visitor name, while each category supplies its price. That version stops offering arbitrary price editing.

[Polymorphism](05_Polymorphism.md) then makes the shared price request virtual. Its base class will look like this. This is a preview declaration, not a replacement to paste into the program above:

```cpp
class AdmissionTicket {
public:
    virtual ~AdmissionTicket() = default;

    std::string visitor() const {
        return visitor_;
    }

    virtual int priceCents() const = 0;

protected:
    explicit AdmissionTicket(const std::string& visitor)
        : visitor_(visitor) {}

private:
    std::string visitor_;
};
```

| Code | Plain meaning |
|------|---------------|
| `virtual int priceCents() const` | Calls through the base reference can use the actual ticket kind's implementation |
| `= 0` | This is a **pure virtual function**; a concrete ticket type must supply its price implementation |
| `virtual ~AdmissionTicket() = default` | Allow correct cleanup when deleting a derived ticket through a base pointer |
| `protected:` | The constructor can be used by the base and its derived classes |

**An abstract class is a class that cannot be instantiated directly.** This preview is abstract because `priceCents()` is pure virtual [2]. The earlier class with a stored price is concrete, so it can be instantiated.

An abstract base can share data, as this one shares the visitor name. When complete separation of interface and implementation is needed, C++ commonly uses a pure abstract class with no instance data [3][4].

For now, focus on the promise: callers ask for a price in cents. The next two notes explain inheritance, `override`, and virtual calls using these same tickets.

## 3. The key intents

### Reduce what you need to understand

You can read the receipt function without learning the ticket's storage or pricing rules.

### Change details without breaking readers

A ticket can change how it supplies a price while preserving the `priceCents()` promise. That keeps callers such as the receipt function independent of those details.

### Let one piece of code work with several implementations

The later virtual interface lets the same receipt function accept adult, child, and senior tickets. Abstraction defines the common request; runtime polymorphism chooses the implementation.

### Make testing easier

With the later abstract base, a test ticket can return a known price so receipt formatting can be checked independently of real pricing rules.

### Distinguish abstraction from encapsulation

| Idea | Question | Ticket example |
|------|----------|----------------|
| **Abstraction** | What essential operations are exposed? | The receipt asks `priceCents()` without knowing how it gets the price |
| **Encapsulation** | How are data and operations bundled, and access controlled? | `priceCents_` is private; the setter checks changes |

These ideas work together, but private fields alone do not guarantee an interface that is easy to use.

## 4. Common mistakes

### Forcing operations that do not fit every ticket

Every admission ticket has a visitor and a price. A request for an adult-only benefit does not necessarily belong on every ticket. Keep category-specific features with the relevant category.

### Letting details leak into the shared operation

Avoid making receipt code pass a category whenever it asks for a price:

```cpp
// An alternative interface to avoid in this design:
ticket.priceCents("adult");
```

The ticket should know its own pricing behavior. Requiring the caller to supply a matching category spreads that knowledge and allows mismatches.

### Hiding failures along with the details

The current getter returns a stored value and has no pricing-service dependency. If a later implementation retrieves prices from a service, define how it reports failure. Returning zero on failure would wrongly look like a valid free ticket.

Callers do not need every implementation detail, but they need a clear success and failure contract.

### Keeping the name but breaking the promise

A `priceCents()` implementation that returns dollars or changes the visitor breaks the interface's meaning even if it compiles. Every implementation must keep the same promise.

### Keeping a reference after its object is gone

Our receipt function uses its reference only during the call. If a different design stores that reference for later, the ticket must remain alive until its last use. Using a reference to a destroyed object is undefined behavior.

### Building an interface too early

The ordinary class already provides abstraction. Use an abstract base when several implementations or test doubles justify it. If categories differ only by a stored price, a single class with a price member can remain the simpler design [5].

## 5. A short design checklist

1. Write down what callers need to ask for.
2. Name each operation after what it does.
3. Include shared operations that make sense for every implementation.
4. Keep implementation-specific settings and calculations inside the relevant implementation.
5. Define results, units, side effects, and failure behavior.
6. Add an abstract class when multiple implementations or test doubles justify it.
7. Check that changing implementation details does not require editing callers.

## 6. Check your understanding

**What is hidden from the receipt function?**

<details>
<summary>Answer</summary>

How the ticket stores or calculates its price. The receipt only uses `visitor()` and `priceCents()`.

</details>

**What problem does reading public fields create here?**

<details>
<summary>Answer</summary>

Callers depend on those fields continuing to exist. Replacing stored prices with calculated prices would require changing those callers.

</details>

**Does abstraction always require an abstract class?**

<details>
<summary>Answer</summary>

No. `printReceipt()` and the ordinary ticket class already expose useful operations while hiding details.

</details>

**Why can't we create an object of the preview base class directly?**

<details>
<summary>Answer</summary>

Its `priceCents()` function is pure virtual, making `AdmissionTicket` abstract. A concrete derived ticket must supply that implementation.

</details>

**What should change in `printReceipt()` when senior tickets arrive?**

<details>
<summary>Answer</summary>

Once the virtual interface is in place, nothing. A senior ticket provides the same name and price operations. The code that creates tickets chooses the new type.

</details>

**Why shouldn't the receipt pass `"adult"` into `priceCents()`?**

<details>
<summary>Answer</summary>

It would force the receipt to know the ticket's category and risk supplying the wrong one. The ticket should handle its own pricing details.

</details>

## 7. TL;DR

Abstraction exposes essential operations while hiding unnecessary details. Our receipt asks a ticket for a visitor and a price without depending on its storage. Ordinary functions and classes already provide abstraction; abstract classes can express shared operations across implementations.

## Sources and further reading

- [1] Wikipedia: [Abstraction (computer science)](https://en.wikipedia.org/wiki/Abstraction_(computer_science)).
- [2] cppreference: [Abstract class](https://en.cppreference.com/w/cpp/language/abstract_class).
- [3] C++ Core Guidelines: [C.121 — If a base class is used as an interface, make it a pure abstract class](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#rh-abstract).
- [4] C++ Core Guidelines: [C.122 — Use abstract classes as interfaces when complete separation of interface and implementation is needed](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#rh-separation).
- [5] C++ Core Guidelines: [I.25 — Prefer empty abstract classes as interfaces to class hierarchies](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#ri-abstract).
