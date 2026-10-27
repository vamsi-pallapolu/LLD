# Polymorphism: One Request, Many Behaviors

**Polymorphism is the provision of a common interface to entities of different types, allowing the same operation to have type-specific behavior.**

In plain language, you write one request, such as "tell me your price", and send it to different kinds of objects. Each kind answers in its own way. The code making the request does not need to know which kind it is talking to.

C++ supports polymorphism in two ways: some choices are made while the program is compiled, and some while it runs. This note starts with the runtime kind, which uses **virtual functions**. It builds on the same museum tickets from [Inheritance](04_Inheritance.md): `AdultTicket` for Maya (1500 cents) and `ChildTicket` for Leo (800 cents), followed by `SeniorTicket` for Nora (1000 cents).

The inheritance note shared `visitor()` but defined prices only in the derived classes. This note adds `virtual priceCents()` to `AdmissionTicket`, making the base abstract, and adds a virtual destructor. We can now use the `printReceipt()` function from [Abstraction](03_Abstraction.md) unchanged with every ticket category. Each complete program below is a standalone version.

## 1. A real-world analogy: museum tickets

A museum sells adult tickets and child tickets. Both are admission tickets, and both have a visitor's name and a price. Their prices differ.

At the entrance, the attendant asks any ticket, "What is your price?" The attendant does not keep a list of categories and prices. The ticket itself knows the answer.

- **Inheritance:** an adult ticket and a child ticket are both kinds of admission ticket.
- **Polymorphism:** asking any ticket for its price gives the right price for that ticket.

In this example, assume the ticket category was chosen correctly at purchase. We are modeling prices, not checking ages or taking payments.

## 2. Bad vs. good: where should the price decision live?

### A design that spreads category checks

This complete program works for two categories:

```cpp
#include <iostream>
#include <string>

struct Ticket {
    std::string visitor;
    std::string category;
};

void printReceipt(const Ticket& ticket) {
    int priceCents;

    if (ticket.category == "adult") {
        priceCents = 1500;
    } else if (ticket.category == "child") {
        priceCents = 800;
    } else {
        std::cout << "Unknown ticket category\n";
        return;
    }

    std::cout << ticket.visitor << ": " << priceCents << " cents\n";
}

int main() {
    Ticket adult{"Maya", "adult"};
    Ticket child{"Leo", "child"};

    printReceipt(adult);
    printReceipt(child);
}
```

Output:

```text
Maya: 1500 cents
Leo: 800 cents
```

The receipt function must know every category and its price. Now imagine refunds and sales reports need prices too. Each of them would copy the same `if` chain, and adding a category would mean finding and updating every copy. A typo such as `"adlut"` is also accepted when a ticket is created, and only fails later.

One small `if` is not automatically a problem. If categories differ only by a number, a simple lookup table may be enough. Polymorphism becomes worthwhile when each kind has its own *behavior* and several parts of the program need it. This small pricing example keeps the mechanism easy to see.

### A design with a common base class

As in the inheritance note, `AdmissionTicket` is the **base class** (the general type), and `AdultTicket` and `ChildTicket` are **derived classes** (the specific types).

This complete program produces the same output. It requires C++11 or later.

```cpp
#include <iostream>
#include <string>

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

class AdultTicket : public AdmissionTicket {
public:
    explicit AdultTicket(const std::string& visitor)
        : AdmissionTicket(visitor) {}

    int priceCents() const override {
        return 1500;
    }
};

class ChildTicket : public AdmissionTicket {
public:
    explicit ChildTicket(const std::string& visitor)
        : AdmissionTicket(visitor) {}

    int priceCents() const override {
        return 800;
    }
};

void printReceipt(const AdmissionTicket& ticket) {
    std::cout << ticket.visitor() << ": "
              << ticket.priceCents() << " cents\n";
}

int main() {
    AdultTicket adult("Maya");
    ChildTicket child("Leo");

    printReceipt(adult);
    printReceipt(child);
}
```

Output:

```text
Maya: 1500 cents
Leo: 800 cents
```

Prices are stored as whole cents so the example avoids decimal rounding problems.

### Read the new syntax

`: public AdmissionTicket` means "is a kind of `AdmissionTicket`", as explained in [Inheritance](04_Inheritance.md). The other parts:

| Code | Plain meaning |
|------|---------------|
| `: AdmissionTicket(visitor)` | Build the base part of the ticket first, passing it the visitor's name |
| `private: std::string visitor_` | Only `AdmissionTicket` itself can touch the name; derived tickets read it through `visitor()` |
| `virtual int priceCents() const` | When called through an `AdmissionTicket&`, use the version belonging to the actual ticket |
| `= 0` | The base class gives no price of its own; every concrete ticket type must supply one. This is called a **pure virtual function** |
| `override` | Ask the compiler to confirm this function really replaces a virtual function from the base class |
| `virtual ~AdmissionTicket() = default` | Make cleanup work correctly even if a ticket is deleted through an `AdmissionTicket` pointer |
| `protected:` | Only this class and classes derived from it can use the constructor; outside code cannot create a plain `AdmissionTicket` |

**An abstract class is a class that cannot be instantiated directly.** In this example, you cannot create a base object because its pure virtual price operation has not been implemented by a concrete type. `AdmissionTicket` is abstract because `priceCents()` is pure virtual. `AdultTicket` and `ChildTicket` are **concrete** classes: they fill in the missing function, so you can create them.

### Follow the first call

1. `AdultTicket adult("Maya")` creates an adult ticket. Its base part stores `"Maya"`.
2. `printReceipt(adult)` receives the ticket as `const AdmissionTicket&`. It refers to the existing adult ticket; nothing is copied.
3. `ticket.visitor()` is an ordinary function shared by all tickets. It returns `"Maya"`.
4. `ticket.priceCents()` is virtual. C++ checks what kind of ticket this really is, finds an `AdultTicket`, and runs `AdultTicket::priceCents()`, which returns 1500.
5. Passing `child` to the same function runs `ChildTicket::priceCents()` instead.

**Dynamic dispatch is the selection, at runtime, of a virtual function's implementation according to the object's dynamic type.** In plain language, C++ waits until the program runs and then picks the version of the function that belongs to the actual object.

Two terms help here:

- The **static type** is the type written in the code: `printReceipt` declares its parameter as `const AdmissionTicket&`.
- The **dynamic type** is the type of the object that actually arrives: `AdultTicket` on the first call, `ChildTicket` on the second.

A virtual call follows the dynamic type. An ordinary call, such as `visitor()`, follows the static type.

### Add a new category

Place this class before `main()` in the good program:

```cpp
class SeniorTicket : public AdmissionTicket {
public:
    explicit SeniorTicket(const std::string& visitor)
        : AdmissionTicket(visitor) {}

    int priceCents() const override {
        return 1000;
    }
};
```

Then add these lines inside `main()`:

```cpp
SeniorTicket senior("Nora");
printReceipt(senior);
```

The extra output is `Nora: 1000 cents`. **`printReceipt()` did not change.** Only the code that creates tickets needs to know the new category exists.

## 3. The key intents

### Replace repeated type checks with one request

The bad design asks "which category is this?" and then decides. The good design simply asks "what is your price?" and trusts the answer.

**Replace Conditional with Polymorphism** is a well-known refactoring that turns an `if`/`switch` on an object's type into a virtual function on each type [1]. In plain language: instead of the caller choosing the behavior, each type carries its own.

### Add new kinds without editing working code

**The open–closed principle states that software entities should be open for extension but closed for modification** [2]. In plain language, you should be able to add new behavior by adding new code, not by rewriting code that already works.

Adding `SeniorTicket` is an example: we added a class and left `printReceipt()` alone. Every function written against `AdmissionTicket` gains senior tickets for free.

### Keep each kind's behavior with that kind

The adult price lives in `AdultTicket`. To change it, you look in one obvious place, not in every receipt, refund and report function.

### Let callers depend on the general type

`printReceipt()` only knows `AdmissionTicket`. That also makes testing easier: you can pass a pretend ticket with a known price and check the receipt.

### Every version must keep the same promise

Polymorphism only works if each version of `priceCents()` means the same thing. Suppose the base promises "a price in cents, zero or more, without changing the ticket". A derived ticket that returned dollars would compile but give wrong receipts. This rule is the **Liskov Substitution Principle**, covered in [Inheritance](04_Inheritance.md).

### How the four ideas fit together

| Concept | Purpose | In this example |
|---------|---------|-----------------|
| [Encapsulation](02_Encapsulation.md) | Bundle data and methods with controlled access to internal state | The visitor name stays private; the earlier editable-price version also checked price changes |
| [Abstraction](03_Abstraction.md) | Expose essential features while hiding unnecessary implementation details | Receipt code needs a name and a price, not how the price is worked out |
| [Inheritance](04_Inheritance.md) | Define a specific type from a general type | `AdultTicket` derives from `AdmissionTicket` |
| Polymorphism | Use one interface with type-specific behavior | `priceCents()` runs the actual ticket's version |

## 4. Compile-time and runtime polymorphism

**Compile-time polymorphism is polymorphism in which the applicable operation is determined during compilation.** In plain language, the compiler picks which function to call before the program ever runs. C++ supports it through function overloading, operator overloading and templates.

**Runtime polymorphism is polymorphism in which the applicable operation is determined during execution according to the object's dynamic type.** In plain language, the choice is made while the program runs, based on the real object. The ticket program uses this form.

| Form | What it does | Needs inheritance? |
|------|--------------|--------------------|
| Function overloading | Several functions share a name but take different parameters | No |
| Operator overloading | Define what `+`, `==` and so on mean for your own types | No |
| Templates | Write code once and let the compiler produce a version for each type | No |
| Virtual functions | Pick the actual object's version of a function while running | Yes |

### Overloading is different from overriding

These two words sound alike but are unrelated.

**Function overloading is the declaration of multiple functions with the same name but different parameter lists.** The compiler picks one by looking at the arguments. A different return type alone is not enough.

This separate, complete example prints a ticket label by number or by visitor name:

```cpp
#include <iostream>
#include <string>

void printLabel(int ticketNumber) {
    std::cout << "Ticket number: " << ticketNumber << '\n';
}

void printLabel(const std::string& visitor) {
    std::cout << "Visitor: " << visitor << '\n';
}

int main() {
    printLabel(42);
    printLabel(std::string("Maya"));
}
```

Output:

```text
Ticket number: 42
Visitor: Maya
```

`42` is an `int`, so the first version runs. `std::string("Maya")` is a `std::string`, so the second runs. No base class or virtual function is involved.

**Function overriding is the provision, in a derived class, of a new implementation of an inherited virtual function with the same signature.** In plain language, the derived class replaces the base class's version of a virtual function. The name, parameters and trailing `const` must match. `AdultTicket::priceCents() const` overrides `AdmissionTicket::priceCents() const` [3].

## 5. Common mistakes

### Forgetting `virtual` in the base class

If the base function is not virtual, a same-named function in a derived class does **not** replace it for calls through the base. `printReceipt()` would always call the base version. Inheritance alone does not make a function virtual.

### Leaving out `override`

Suppose you accidentally write `int priceCents() override` in `ChildTicket`, forgetting `const`. Because of `override`, the compiler reports an error: there is no matching base function to replace.

Without `override`, the compiler would quietly treat it as a brand-new function. Here `ChildTicket` would then be abstract, and creating one would fail with a confusing error. In a base with a default implementation, the wrong function would run silently. Always write `override` [4].

### Passing polymorphic objects by value

`printReceipt(const AdmissionTicket& ticket)` works with the real ticket. If a function instead took a base-class object by value, C++ would copy only the base part and lose the derived part.

**Object slicing is the loss of the derived-class portion when a derived object is copied into a base-class object by value.** In plain language, the copy is just a plain base object, so virtual calls on it run the base version, not the derived one.

In our example, `void printReceipt(AdmissionTicket ticket)` does not even compile, because you cannot create an abstract `AdmissionTicket`. Pass polymorphic objects by reference or pointer [5].

### Missing a virtual destructor

A **destructor** is the cleanup function that runs when an object goes away. If a program creates an `AdultTicket` with `new` and later deletes it through an `AdmissionTicket*`, the base destructor must be virtual. Otherwise the behavior is **undefined**: C++ makes no promise about what happens.

The same applies to `std::unique_ptr<AdmissionTicket>`, a smart pointer that deletes the ticket for you. Our example uses local objects, but the virtual destructor keeps the class safe for those other uses [6].

### Calling virtual functions from the base constructor

While the `AdmissionTicket` part is being built, the `AdultTicket` part does not exist yet. A virtual call made inside the base constructor or destructor will **not** reach the derived version. Calling the pure virtual `priceCents()` there is undefined behavior [3].

### Bringing the type checks back

If callers start asking "is this an `AdultTicket`?" before doing their work, the base class is probably missing an operation. Add that operation as a virtual function instead. Code that *creates* tickets still has to choose a category; that is expected.

### Using polymorphism where a value would do

If adult, child and senior tickets differ only by a number, a single `Ticket` class with a `priceCents_` member, or a lookup table, is simpler. Use virtual functions when the kinds actually *behave* differently.

## 6. A short design checklist

1. Name the request that every kind must answer, such as "give your price".
2. Write down what that request promises, so every version keeps the same meaning.
3. Check that the kinds really behave differently; otherwise store a value instead.
4. Make the request `virtual` in the base class, and use `override` in each derived class.
5. Give the base class a virtual destructor if objects may be deleted through a base pointer.
6. Pass objects by reference or pointer, never by base-class value.
7. Make sure callers use only the base class's operations, with no type checks.
8. Try adding a new kind: only the new class and the creation code should change.

## 7. Self-test

**What is the difference between inheritance and polymorphism?**

<details>
<summary>Answer</summary>

Inheritance defines a specific type from a general one. Polymorphism lets one request produce type-specific behavior. Here, inheritance connects the ticket types, and virtual calls pick each ticket's own price.

</details>

**Why does `printReceipt(child)` print 800, even though its parameter is an `AdmissionTicket&`?**

<details>
<summary>Answer</summary>

The reference refers to the real `ChildTicket`, and `priceCents()` is virtual. Dynamic dispatch follows the object's dynamic type and runs `ChildTicket::priceCents()`.

</details>

**What happens if `ChildTicket` declares `int priceCents() override` without `const`?**

<details>
<summary>Answer</summary>

Compilation fails. The signature does not match the base's `const` function, and `override` requires a real match. Without `override`, the mistake would be easier to miss.

</details>

**Is `visitor()` polymorphic just because it is inherited?**

<details>
<summary>Answer</summary>

No. It is an ordinary, non-virtual function. Every ticket shares the same version, chosen from the static type.

</details>

**What must change to support senior tickets?**

<details>
<summary>Answer</summary>

Add a `SeniorTicket` class that overrides `priceCents()`, and update the code that creates tickets. `printReceipt()` stays the same.

</details>

**When would a lookup table be better than virtual functions?**

<details>
<summary>Answer</summary>

When the kinds differ only by data, such as a price number, and not by behavior. A table or a single class with a price member is simpler then.

</details>

**How does overloading differ from overriding?**

<details>
<summary>Answer</summary>

Overloading means several same-named functions with different parameters; the compiler picks one from the arguments. Overriding means a derived class replaces a base virtual function; the program picks the version at runtime from the actual object.

</details>

## 8. TL;DR

Polymorphism lets one request produce type-specific behavior, so callers stop checking types. C++ chooses at compile time through overloading and templates, and at runtime through virtual functions. Make the base operation `virtual`, mark derived versions `override`, pass objects by reference or pointer, and give the base a virtual destructor. Every version must keep the same promise.

## Sources and further reading

- [1] Martin Fowler, Refactoring catalog: [Replace Conditional with Polymorphism](https://refactoring.com/catalog/replaceConditionalWithPolymorphism.html).
- [2] Wikipedia: [Open–closed principle](https://en.wikipedia.org/wiki/Open%E2%80%93closed_principle).
- [3] cppreference: [Virtual functions](https://en.cppreference.com/w/cpp/language/virtual.html), including overriding and virtual calls during construction and destruction.
- [4] C++ Core Guidelines: [C.128 — Virtual functions should specify exactly one of `virtual`, `override`, or `final`](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#rh-override).
- [5] C++ Core Guidelines: [C.145 — Access polymorphic objects through pointers and references](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#rh-poly).
- [6] C++ Core Guidelines: [C.127 — A class with a virtual function should have a virtual or protected destructor](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#rh-dtor).
- Wikipedia: [Polymorphism (computer science)](https://en.wikipedia.org/wiki/Polymorphism_(computer_science)).
