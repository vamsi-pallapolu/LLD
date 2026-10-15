# Inheritance: Build a Specific Type from a General One

**Inheritance is an object-oriented mechanism by which a derived class acquires the members of a base class and may extend or specialize its behavior** [1].

In simple words, a new class can reuse the data and functions of an existing class and add features of its own.

## 1. A real-world analogy: a dog is an animal

Every animal has a name and can eat. A dog is an animal, so it also has a name and eats. A dog can additionally bark.

We can express this using two classes:

- `Animal` stores a name and provides `eat()`.
- `Dog` inherits both from `Animal` and adds `bark()`.

**A base class is a class from which another class derives.** Here, `Animal` is the base class: it provides the common data and behavior.

**A derived class is a class that inherits from another class.** Here, `Dog` is the derived class: it reuses what `Animal` provides and adds its own behavior.

The relationship is **"a dog is an animal."**

## 2. Bad vs. good: writing the same thing twice

### Without inheritance

These two classes repeat the same name storage and the same `eat()` function:

```cpp
#include <iostream>
#include <string>

class Animal {
public:
    explicit Animal(const std::string& name) : name_(name) {}

    void eat() const {
        std::cout << name_ << " is eating\n";
    }

private:
    std::string name_;
};

class Dog {
public:
    explicit Dog(const std::string& name) : name_(name) {}

    void eat() const {
        std::cout << name_ << " is eating\n";
    }

    void bark() const {
        std::cout << name_ << " says Woof!\n";
    }

private:
    std::string name_;
};

void feed(const Animal& animal) {
    animal.eat();
}

int main() {
    Animal parrot("Polly");
    Dog dog("Rex");

    feed(parrot);
    // feed(dog); // Error: a Dog is not an Animal to the compiler.
    dog.eat();
    dog.bark();
}
```

Output:

```text
Polly is eating
Rex is eating
Rex says Woof!
```

There are two problems:

1. **The code is duplicated.** The name, the constructor and `eat()` are written twice. If eating changes, both copies must be updated, and it is easy to forget one.
2. **C++ does not know a dog is an animal.** `feed()` accepts an `Animal`, but `feed(dog)` does not compile. The two classes merely look alike; to the compiler they are unrelated.

Repeated code alone is not enough reason to use inheritance. Here, there is also a real "is-a" relationship: a dog is a kind of animal [2].

### With inheritance

Write the shared parts once in `Animal`, then let `Dog` inherit them:

```cpp
#include <iostream>
#include <string>

class Animal {
public:
    explicit Animal(const std::string& name) : name_(name) {}

    std::string name() const {
        return name_;
    }

    void eat() const {
        std::cout << name_ << " is eating\n";
    }

private:
    std::string name_;
};

class Dog : public Animal {
public:
    explicit Dog(const std::string& name) : Animal(name) {}

    void bark() const {
        std::cout << name() << " says Woof!\n";
    }
};

void feed(const Animal& animal) {
    animal.eat();
}

int main() {
    Animal parrot("Polly");
    Dog dog("Rex");

    feed(parrot);
    feed(dog);  // Works: a Dog is an Animal.
    dog.bark();
}
```

Output:

```text
Polly is eating
Rex is eating
Rex says Woof!
```

`Dog` no longer stores its own name or defines its own `eat()`. It uses the ones inherited from `Animal`, and `feed(dog)` now compiles.

### Read the syntax

| Code | Plain meaning |
|------|---------------|
| `class Dog : public Animal` | `Dog` is a kind of `Animal`. It gets `Animal`'s members, and code expecting an `Animal&` can accept a `Dog` |
| `explicit Animal(const std::string& name)` | The constructor: it runs when an animal is created. `explicit` stops C++ from silently turning a string into an animal |
| `: name_(name)` | Give the stored name its starting value |
| `: Animal(name)` | In `Dog`'s constructor: build the animal part first, passing it the name |
| `const std::string&` | Use the caller's text without copying it and without changing it |
| `const` after a function, as in `eat() const` | This function only reads the object; it promises not to change its data |
| `private:` | Only `Animal`'s own functions can use `name_`. `Dog` cannot either |
| `const Animal& animal` | `feed()` works with the caller's existing animal, without copying it or changing it |

A **reference**, written with `&`, is another name for an existing object. `feed()` does not get its own copy; it uses the very animal you passed in.

### Walk through the program

1. `Animal parrot("Polly")` creates an ordinary animal named Polly.
2. `Dog dog("Rex")` starts creating a dog. Its constructor first runs `Animal(name)`, which builds the **animal part** of the dog and stores `"Rex"`.
3. The `Dog` constructor then finishes. The dog has no extra data, so there is nothing more to set up.
4. `feed(parrot)` calls `eat()` and prints `Polly is eating`.
5. `feed(dog)` passes the dog where an `Animal&` is expected. That is allowed because a `Dog` is an `Animal`. It prints `Rex is eating`.
6. `dog.bark()` runs `Dog`'s own function. `Dog` cannot read `name_` directly because it is private, so it calls the public `name()` function instead.

**A base-class subobject is the part of a derived object corresponding to one of its base classes.** In plain language, every `Dog` contains a complete `Animal` built inside it. That inner animal holds the name. It belongs only to this dog; Rex does not share it with Polly.

Inheritance works in one direction. A dog can use animal behavior, but an ordinary `Animal` object does not gain `bark()` from `Dog`.

## 3. The key intents

### Express an "is-a" relationship

A dog is an animal. Public inheritance tells the compiler so, which is why `feed(dog)` works. `feed()` only needs the `Animal` operations; it does not need to know the object is a dog.

**The Liskov Substitution Principle states that objects of a subtype should be usable in place of objects of their supertype without violating the properties expected by clients** [3]. In plain language, anywhere an animal works, a dog must work too, and keep every promise an animal makes. If `Animal` promises that `eat()` never fails, a `Dog` must not break that.

### Reuse common data and behavior

The name and `eat()` are written once in `Animal`. `Dog` reuses them, and another animal class, such as `Cat`, could reuse them too. If eating changes, there is one place to change it.

### Add specific behavior

`bark()` belongs to `Dog`. The base class does not need every action that every derived class might provide.

A function receiving `const Animal&` can call `eat()`, but it cannot call `bark()` through that reference, because not every animal can bark.

### Keep each class responsible for its own rules

`Animal` sets up and protects its own name, even inside a dog. `Dog` sets up only what it adds. Each class looks after its own data.

### Keep inheritance distinct from polymorphism

Here, every animal eats in exactly the same way: there is one `eat()` function. Letting a dog eat *differently* from other animals, while `feed()` stays unchanged, needs **virtual functions**. That is covered in [Polymorphism](05_Polymorphism.md) [4].

## 4. Common mistakes

### Using inheritance for a "has-a" relationship

A dog **is an** animal, so inheritance fits.

A dog **has a** collar. A dog is not a collar, so `Dog` should not inherit from `Collar` just to use it.

**Composition is the construction of a class using objects of other classes as members.** In simple words, one object contains another. A `Collar` member stored inside each dog would be composition.

### Assuming private members become accessible

Public inheritance does **not** let `Dog` touch `Animal`'s private `name_`. Writing `std::cout << name_;` inside `Dog` would not compile. The name still exists inside every dog; `Dog` just has to use `Animal`'s public `name()` function to read it.

| Member in `Animal` | Can `Dog`'s own functions use it? | Can outside code, such as `main()`, use it? |
|--------------------|-----------------------------------|---------------------------------------------|
| `public` | Yes | Yes |
| `protected` | Yes | No |
| `private` | No; it must use `Animal`'s public or protected functions | No |

It may be tempting to make `name_` protected so `Dog` can change it freely. Avoid that when the base class has rules about its data, because every derived class could then skip those rules.

### Forgetting to build the base part

If `Dog`'s constructor leaves out `: Animal(name)`, C++ tries to build the animal part with no arguments. `Animal` has no constructor that takes zero arguments, so the code does not compile. A derived constructor must say how to build its base part.

The order is always the same: the base part is built first, then the derived part. When the object goes away, the order is reversed: the derived part is cleaned up first, then the base part.

### Forgetting `public` in the inheritance declaration

For a class declared with the `class` keyword, inheritance is private if you leave the word out. With `class Dog : Animal`, outside code could no longer call `dog.eat()` or `feed(dog)`. Write `class Dog : public Animal`. (For a `struct`, the default is public.)

### Copying a dog into an animal

```cpp
Animal copy = dog;
// copy.bark(); // Error: copy is only an Animal.
```

**Object slicing is the loss of the derived-class portion when a derived object is copied into a base-class object by value** [5]. In plain language, only the animal part of Rex is copied. The result is a plain `Animal` named Rex, not a dog, and anything `Dog` added is gone.

Use a reference, as `feed(const Animal&)` does, when you want to work with the original dog.

### Assuming the base class gets the derived class's features

`Animal` does not inherit from `Dog`. You cannot call `bark()` on an ordinary `Animal` object.

### Assuming every inherited function is virtual

`eat()` is an ordinary, non-virtual function. If `Dog` defined its own `eat()`, calls through an `Animal&`, such as inside `feed()`, would still run `Animal`'s version. Virtual functions are explained in [Polymorphism](05_Polymorphism.md) [4].

### Deleting a dog through an animal pointer

This example uses local objects, which C++ cleans up automatically. If a program instead creates a `Dog` with `new` and deletes it through an `Animal*`, `Animal` must have a **virtual destructor** (a cleanup function marked `virtual`). Otherwise the behavior is **undefined**: C++ makes no promise about what happens [6]. The polymorphism note covers this.

## 5. Names you may encounter

You only need **single inheritance** for the example above. The other names describe larger arrangements of classes.

| Form | Meaning |
|------|---------|
| Single inheritance | A derived class has one direct base class |
| Multiple inheritance | A derived class has more than one direct base class |
| Multilevel inheritance | A class derives from a class that itself derives from another, such as `Puppy` from `Dog` from `Animal` |
| Hierarchical inheritance | Several classes derive from the same base, such as `Dog` and `Cat` from `Animal` |
| Hybrid inheritance | A hierarchy combines more than one of these patterns |

These names are common in textbooks and interviews. They are not C++ keywords, and only single and multiple inheritance are terms used by the C++ standard itself.

## 6. A short design checklist

1. Can you honestly say the derived type "is a" base type?
2. Would composition better describe a "has-a" relationship?
3. Which data and behavior genuinely belong to every kind, and so belong in the base class?
4. What does the derived class add?
5. Is the base data private, with the derived class using the base's functions?
6. Does every derived constructor build the base part, such as `: Animal(name)`?
7. Can code using the base type still work correctly with the derived type?
8. Will anything delete derived objects through a base pointer? If so, give the base a virtual destructor.

## 7. Self-test

**Which is the base class: `Animal` or `Dog`?**

<details>
<summary>Answer</summary>

`Animal` is the base class. `Dog` is the derived class because it inherits from `Animal`.

</details>

**Why does `feed(dog)` fail in the bad version but work in the good one?**

<details>
<summary>Answer</summary>

In the bad version, `Dog` and `Animal` are unrelated classes that merely look alike. In the good version, `Dog` publicly inherits from `Animal`, so a dog can be used wherever an `Animal&` is expected.

</details>

**Why can we call `eat()` on a dog when `Dog` does not define `eat()`?**

<details>
<summary>Answer</summary>

`Dog` publicly inherits `Animal`'s public `eat()` function.

</details>

**Why does `bark()` call `name()` instead of reading `name_`?**

<details>
<summary>Answer</summary>

`name_` is private in `Animal`, so even a derived class cannot use it directly. `name()` is public, so `Dog` can call it.

</details>

**What happens if `Dog`'s constructor leaves out `: Animal(name)`?**

<details>
<summary>Answer</summary>

It does not compile. C++ would try to build the animal part with no arguments, and `Animal` has no such constructor.

</details>

**After `Animal copy = dog;`, can you call `copy.bark()`?**

<details>
<summary>Answer</summary>

No. Only the animal part was copied, so `copy` is a plain `Animal`. This is object slicing.

</details>

**Should `Dog` inherit from `Collar`?**

<details>
<summary>Answer</summary>

No. A dog has a collar; it is not a collar. Use composition for that relationship.

</details>

**Does this program use virtual functions?**

<details>
<summary>Answer</summary>

No. `eat()` is one shared, non-virtual function. Inheritance does not automatically make functions virtual.

</details>

## 8. TL;DR

Inheritance lets a derived class reuse and extend a base class. `Dog` is an `Animal`: it inherits the name and `eat()`, adds `bark()`, and can be passed wherever an `Animal&` is expected. Build the base part in the derived constructor, keep base data private, and pass objects by reference to avoid slicing. Use public inheritance for a real "is-a" relationship and composition for "has-a".

## Sources and further reading

- [1] Wikipedia: [Inheritance (object-oriented programming)](https://en.wikipedia.org/wiki/Inheritance_(object-oriented_programming)).
- [2] C++ Core Guidelines: [C.120 — Use class hierarchies to represent concepts with inherent hierarchical structure (only)](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#rh-domain).
- [3] Wikipedia: [Liskov substitution principle](https://en.wikipedia.org/wiki/Liskov_substitution_principle).
- [4] cppreference: [Virtual functions](https://en.cppreference.com/w/cpp/language/virtual.html), explaining how virtual calls differ from ordinary calls.
- [5] C++ Core Guidelines: [C.145 — Access polymorphic objects through pointers and references](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#rh-poly), including an explanation of slicing.
- [6] C++ Core Guidelines: [C.35 — A base class destructor should be either public and virtual, or protected and non-virtual](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#rc-dtor-virtual).
- cppreference: [Derived classes](https://en.cppreference.com/w/cpp/language/derived_class), for inheritance access rules and base-class subobjects.
