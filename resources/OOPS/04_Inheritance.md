# Inheritance: Build a Specific Type from a General One

**Inheritance is an object-oriented mechanism by which a derived class acquires the members of a base class and may extend or specialize its behavior** [1].

In simple words, a new class can reuse the data and functions of an existing class and add features of its own.

We continue with the museum tickets from the earlier notes. Maya's adult ticket costs 1500 cents; Leo's child ticket costs 800 cents. Previously, both were objects of one editable-price class. Here, we introduce distinct `AdultTicket` and `ChildTicket` types. Their shared `AdmissionTicket` base stores the visitor, while each category supplies its own fixed price. This version removes `setPriceCents()` and the stored price.

Each complete program is a separate version of the example. This chapter first teaches inheritance with ordinary functions; the next adds virtual dispatch.

## 1. A real-world analogy: an adult ticket is an admission ticket

Every museum admission ticket has a visitor's name. An adult ticket is an admission ticket, so it also has a visitor's name. Its category additionally provides an adult price.

We can express this using classes:

- `AdmissionTicket` stores the visitor and provides `visitor()`.
- `AdultTicket` inherits the shared part and adds `priceCents()`, returning 1500.
- `ChildTicket` inherits the same shared part and adds `priceCents()`, returning 800.

**A base class is a class from which another class derives.** Here, `AdmissionTicket` is the base class: it provides common data and behavior.

**A derived class is a class that inherits from another class.** Here, `AdultTicket` and `ChildTicket` are derived classes: they reuse what the base provides and add their own operations.

The relationship is **"an adult ticket is an admission ticket."**

## 2. Bad vs. good: writing the same thing twice

### Without inheritance

These unrelated classes repeat the visitor storage and getter:

```cpp
#include <iostream>
#include <string>

class AdmissionTicket {
public:
    explicit AdmissionTicket(const std::string& visitor) : visitor_(visitor) {}

    std::string visitor() const {
        return visitor_;
    }

private:
    std::string visitor_;
};

class AdultTicket {
public:
    explicit AdultTicket(const std::string& visitor) : visitor_(visitor) {}

    std::string visitor() const {
        return visitor_;
    }

    int priceCents() const {
        return 1500;
    }

private:
    std::string visitor_;
};

class ChildTicket {
public:
    explicit ChildTicket(const std::string& visitor) : visitor_(visitor) {}

    std::string visitor() const {
        return visitor_;
    }

    int priceCents() const {
        return 800;
    }

private:
    std::string visitor_;
};

void printVisitor(const AdmissionTicket& ticket) {
    std::cout << ticket.visitor() << '\n';
}

int main() {
    AdultTicket adult("Maya");
    ChildTicket child("Leo");

    // printVisitor(adult); // Error: AdultTicket is unrelated to AdmissionTicket.
    // printVisitor(child); // Error: ChildTicket is unrelated too.
    std::cout << adult.visitor() << '\n';
    std::cout << child.visitor() << '\n';
    std::cout << adult.priceCents() << ' ' << child.priceCents() << " cents\n";
}
```

Output:

```text
Maya
Leo
1500 800 cents
```

There are two problems:

1. **The code is duplicated.** Visitor storage and access are repeated across the classes. A change to that shared behavior needs several edits.
2. **C++ does not know these are kinds of admission ticket.** `printVisitor()` takes an `AdmissionTicket&`, but neither specific type can be passed to it. Similar-looking classes are still unrelated types.

Repeated code alone is not enough reason to use inheritance. There must also be a useful "is-a" relationship [2]. This small hierarchy teaches the mechanism; fixed prices alone could still be represented with the earlier single class.

### With inheritance

Write the shared parts once in `AdmissionTicket`, then let both categories inherit them:

```cpp
#include <iostream>
#include <string>

class AdmissionTicket {
public:
    explicit AdmissionTicket(const std::string& visitor) : visitor_(visitor) {}

    std::string visitor() const {
        return visitor_;
    }

private:
    std::string visitor_;
};

class AdultTicket : public AdmissionTicket {
public:
    explicit AdultTicket(const std::string& visitor) : AdmissionTicket(visitor) {}

    int priceCents() const {
        return 1500;
    }
};

class ChildTicket : public AdmissionTicket {
public:
    explicit ChildTicket(const std::string& visitor) : AdmissionTicket(visitor) {}

    int priceCents() const {
        return 800;
    }
};

void printVisitor(const AdmissionTicket& ticket) {
    std::cout << ticket.visitor() << '\n';
}

int main() {
    AdultTicket adult("Maya");
    ChildTicket child("Leo");

    printVisitor(adult);
    printVisitor(child);
    std::cout << adult.priceCents() << ' ' << child.priceCents() << " cents\n";
}
```

Output:

```text
Maya
Leo
1500 800 cents
```

Neither derived class repeats `visitor_` or `visitor()`. Both can now be passed to `printVisitor()`.

### Read the syntax

| Code | Plain meaning |
|------|---------------|
| `class AdultTicket : public AdmissionTicket` | An adult ticket is a kind of admission ticket; code expecting a base reference can accept it |
| `explicit AdmissionTicket(const std::string& visitor)` | Construct the base with a name; `explicit` prevents automatic conversion from a string to a ticket |
| `: visitor_(visitor)` | Give the stored visitor its starting value |
| `: AdmissionTicket(visitor)` | In a derived constructor, build the base part first with the visitor's name |
| `const std::string&` | Read the caller's text without copying the argument or changing it |
| `visitor() const` | Read the object without changing its data |
| `private:` | Only the base's own functions can directly use `visitor_`; derived classes cannot |
| `const AdmissionTicket& ticket` | Use the caller's existing ticket without copying or changing it |

A **reference**, written with `&`, is another name for an existing object. `printVisitor()` uses the ticket you passed in.

### Walk through the program

1. `AdultTicket adult("Maya")` starts creating an adult ticket.
2. Its constructor first calls `AdmissionTicket(visitor)`, which stores `"Maya"` in the base part.
3. The adult constructor finishes. This derived class has no additional data to initialize.
4. `ChildTicket child("Leo")` builds a separate base part storing `"Leo"`.
5. `printVisitor(adult)` reads Maya's name through the inherited `visitor()` function.
6. `printVisitor(child)` reads Leo's name through the same function.
7. Direct calls to `adult.priceCents()` and `child.priceCents()` use the functions declared in their respective types.

**A base-class subobject is the part of a derived object corresponding to one of its base classes.** Each adult or child ticket contains its own `AdmissionTicket` part. Maya's and Leo's names remain separate.

Inheritance works in one direction. A derived ticket can use base behavior, but the base does not gain the derived class's operations.

## 3. The key intents

### Express an "is-a" relationship

An adult ticket is an admission ticket. Public inheritance tells the compiler so, which is why `printVisitor(adult)` works. The function only needs the base's operations.

**The Liskov Substitution Principle states that objects of a subtype should be usable in place of objects of their supertype without violating the properties expected by clients** [3]. In plain language, every derived ticket must keep the promises made by the general ticket. If `visitor()` promises the ticket holder's name, a derived ticket must preserve that meaning.

### Reuse common data and behavior

The visitor name and getter are written once. Both categories reuse them, and a later `SeniorTicket` can too.

### Add specific behavior

In this version, `priceCents()` is declared only in the derived classes. Code working directly with an `AdultTicket` can call its price function.

A function receiving `const AdmissionTicket&` can call `visitor()`, but cannot call `priceCents()` yet: the base has no such operation. Matching function names in derived classes do not automatically create a shared base operation.

### Keep each class responsible for its own rules

`AdmissionTicket` stores and protects the visitor name. Each category provides its own price. The derived classes do not need direct access to the name member.

### Keep inheritance distinct from polymorphism

There is one inherited, non-virtual `visitor()` function. Both derived price functions are also non-virtual in this chapter.

The earlier `printReceipt(const AdmissionTicket&)` needs both name and price, so it cannot be used with this intermediate base yet. [Polymorphism](05_Polymorphism.md) adds a virtual `priceCents()` to the base, allowing the same receipt function to ask any category for its price [4].

## 4. Common mistakes

### Using inheritance for a "has-a" relationship

An adult ticket **is an** admission ticket, so that relationship can use inheritance.

A ticket **has a** visitor name; it is not a string. `AdmissionTicket` stores a `std::string` member instead of inheriting from `std::string`.

**Composition is the construction of a class using objects of other classes as members.** In plain language, one object contains another. The ticket's string member is an example.

### Assuming private members become accessible

Public inheritance does not let `AdultTicket` access private `visitor_` directly. Code in a derived class can call `visitor()` instead.

| Member in `AdmissionTicket` | Can derived member functions use it? | Can outside code use it? |
|-----------------------------|--------------------------------------|--------------------------|
| `public` | Yes | Yes |
| `protected` | Yes | No |
| `private` | No; use base public or protected functions | No |

Making data protected lets derived classes bypass the base's rules. Prefer private data and operations that preserve those rules.

### Forgetting to build the base part

If `AdultTicket` leaves out `: AdmissionTicket(visitor)`, C++ tries to construct the base without arguments. There is no zero-argument base constructor here, so compilation fails.

The base is built first, then the derived part. Cleanup happens in reverse order: derived part first, then base part.

### Forgetting `public` in the inheritance declaration

With the `class` keyword, inheritance is private by default. Writing `class AdultTicket : AdmissionTicket` prevents outside code from using the inherited getter or passing the adult ticket as a base reference. Write `: public AdmissionTicket`. For a `struct`, inheritance defaults to public.

### Copying an adult ticket into a base ticket

Using the inheritance program's classes and `adult` object:

```cpp
AdmissionTicket copy = adult;
std::cout << copy.visitor(); // Maya
// copy.priceCents(); // Error: the base has no priceCents() operation here.
```

**Object slicing is the loss of the derived-class portion when a derived object is copied into a base-class object by value** [5]. The copy is a plain `AdmissionTicket` containing Maya's name. Its type no longer provides the adult price function.

Use a reference, as `printVisitor(const AdmissionTicket&)` does, to work with the original object.

### Assuming the base gains derived features

`AdmissionTicket` does not inherit from `AdultTicket`. Adding a price function to the adult class does not add it to the base.

### Assuming every inherited function is virtual

`visitor()` is non-virtual. If `AdultTicket` declared its own same-named function, a call through an `AdmissionTicket&` would still use the base version. Virtual functions make different behavior possible through that base reference [4].

### Deleting a derived ticket through a base pointer

This program uses local objects, cleaned up automatically. If you create an `AdultTicket` with `new` and delete it through an `AdmissionTicket*`, the base needs a **virtual destructor**, a cleanup function marked `virtual`. Without it, behavior is **undefined**: C++ makes no promise about what happens [6]. The next note adds this destructor.

## 5. Names you may encounter

Each derived ticket above uses single inheritance. Together, the two categories also form hierarchical inheritance.

| Form | Meaning |
|------|---------|
| Single inheritance | A derived class has one direct base, as `AdultTicket` does |
| Multiple inheritance | A class has more than one direct base |
| Multilevel inheritance | A class derives from another derived class, for example `DiscountedAdultTicket` from `AdultTicket` from `AdmissionTicket` |
| Hierarchical inheritance | Several classes derive from the same base, as adult and child tickets do |
| Hybrid inheritance | A hierarchy combines more than one pattern |

These are descriptive terms used in textbooks and interviews, not C++ keywords. Only single and multiple inheritance are terms used by the C++ standard itself. The multilevel example describes a possible shape, not a recommendation to create a subclass for every discount.

## 6. A short design checklist

1. Can you honestly say the derived type "is a" base type?
2. Would composition better describe a "has-a" relationship?
3. Which data and behavior belong to every kind?
4. What does each derived class add?
5. Is base data private, with derived classes using its operations?
6. Does each derived constructor build the base part?
7. Can code using the base still work correctly with derived objects?
8. If deletion through a base pointer is needed, does the base have a virtual destructor?

## 7. Self-test

**Which is the base class: `AdmissionTicket` or `AdultTicket`?**

<details>
<summary>Answer</summary>

`AdmissionTicket` is the base. `AdultTicket` derives from it.

</details>

**Why does `printVisitor(adult)` fail in the bad version but work in the good one?**

<details>
<summary>Answer</summary>

The bad version has unrelated classes. Public inheritance in the good version allows an adult ticket to be used through an admission-ticket reference.

</details>

**Why can we call `adult.visitor()` when `AdultTicket` does not define it?**

<details>
<summary>Answer</summary>

It publicly inherits the base's `visitor()` function.

</details>

**Can a derived ticket read `visitor_` directly?**

<details>
<summary>Answer</summary>

No. It is private in the base. The derived class can call the public `visitor()` getter.

</details>

**What happens if the adult constructor omits `: AdmissionTicket(visitor)`?**

<details>
<summary>Answer</summary>

Compilation fails because the base has no zero-argument constructor.

</details>

**After `AdmissionTicket copy = adult;`, can you call `copy.priceCents()`?**

<details>
<summary>Answer</summary>

No. The copy is only a base ticket, which has no price operation in this chapter. This is object slicing.

</details>

**Should `AdmissionTicket` inherit from `std::string` to store the name?**

<details>
<summary>Answer</summary>

No. A ticket has a name; it is not a string. Store the string as a member using composition.

</details>

**Does this program use virtual functions?**

<details>
<summary>Answer</summary>

No. Inherited functions are not automatically virtual. The next note makes the price request virtual so the receipt can use it through the base type.

</details>

## 8. TL;DR

Inheritance lets a derived class reuse and extend a base. Adult and child tickets inherit the visitor from `AdmissionTicket` and supply their own price operations. Build the base part in each derived constructor, keep its data private, and use references to avoid slicing. Use public inheritance for "is-a" and composition for "has-a".

## Sources and further reading

- [1] Wikipedia: [Inheritance (object-oriented programming)](https://en.wikipedia.org/wiki/Inheritance_(object-oriented_programming)).
- [2] C++ Core Guidelines: [C.120 — Use class hierarchies to represent concepts with inherent hierarchical structure (only)](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#rh-domain).
- [3] Wikipedia: [Liskov substitution principle](https://en.wikipedia.org/wiki/Liskov_substitution_principle).
- [4] cppreference: [Virtual functions](https://en.cppreference.com/w/cpp/language/virtual.html), explaining how virtual calls differ from ordinary calls.
- [5] C++ Core Guidelines: [C.145 — Access polymorphic objects through pointers and references](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#rh-poly), including an explanation of slicing.
- [6] C++ Core Guidelines: [C.35 — A base class destructor should be either public and virtual, or protected and non-virtual](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#rc-dtor-virtual).
- cppreference: [Derived classes](https://en.cppreference.com/w/cpp/language/derived_class), for inheritance access rules and base-class subobjects.
