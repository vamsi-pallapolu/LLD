# Inheritance: Build a Specific Type from a General One

**Inheritance is an object-oriented mechanism by which a derived class acquires the members of a base class and may extend or specialize its behavior.**

In simple words, a new class can reuse features of an existing class and add features of its own.

## 1. A real-world analogy: a dog is an animal

All animals eat. A dog is an animal, so it also eats. A dog can additionally bark.

We can express this using two classes:

- `Animal` provides `eat()`.
- `Dog` inherits `eat()` from `Animal` and adds `bark()`.

**A base class is a class from which another class derives.** Here, `Animal` is the base class: it provides the common behavior.

**A derived class is a class that inherits from another class.** Here, `Dog` is the derived class: it reuses that behavior and adds its own.

The relationship is **“a dog is an animal.”**

## 2. Bad vs. good: writing the same behavior twice

### Without inheritance

These classes each contain the same `eat()` function:

```cpp
#include <iostream>

class Animal {
public:
    void eat() const {
        std::cout << "Eating food\n";
    }
};

class Dog {
public:
    void eat() const {
        std::cout << "Eating food\n";
    }

    void bark() const {
        std::cout << "Woof!\n";
    }
};

int main() {
    Dog dog;
    dog.eat();
    dog.bark();
}
```

Output:

```text
Eating food
Woof!
```

This works, but `eat()` is written twice. If its shared behavior changes, both copies need updating. C++ also treats these as unrelated classes, even though the real-world concepts are related.

Repeated code alone is not enough reason to use inheritance. Here, inheritance also represents a meaningful relationship: a dog is a kind of animal [1].

### With inheritance

Write `eat()` once in `Animal`, then let `Dog` inherit it:

```cpp
#include <iostream>

class Animal {
public:
    void eat() const {
        std::cout << "Eating food\n";
    }
};

class Dog : public Animal {
public:
    void bark() const {
        std::cout << "Woof!\n";
    }
};

int main() {
    Dog dog;
    dog.eat();  // Inherited from Animal.
    dog.bark(); // Defined in Dog.
}
```

Output:

```text
Eating food
Woof!
```

`Dog` has no separate definition of `eat()`. It uses the function inherited from `Animal`.

### Read the important line

```cpp
class Dog : public Animal
```

Read this as: **“Dog publicly inherits from Animal.”**

| Code | Meaning |
|------|---------|
| `class Animal` | Define the base class |
| `class Dog : public Animal` | Define `Dog` as a derived class of `Animal` |
| `public:` inside a class | Allow outside code to use the following members |
| `void` | The function returns no value |
| `const` after a function | The function cannot change ordinary object data through `this` |
| `Dog dog;` | Create a `Dog` object named `dog` |
| `dog.eat()` | Call the function inherited from `Animal` |
| `dog.bark()` | Call the function defined in `Dog` |

### Walk through the program

1. `Animal` defines the common action, `eat()`.
2. `Dog` inherits from `Animal`, so its objects can use that action.
3. `Dog` adds another action, `bark()`.
4. `Dog dog;` creates one dog object.
5. `dog.eat()` runs `Animal`'s function and prints `Eating food`.
6. `dog.bark()` runs `Dog`'s function and prints `Woof!`.

You do not need to create a separate `Animal` object for the dog to use `eat()`.

Inheritance works in one direction. A dog can use animal behavior, but an ordinary `Animal` object does not gain `bark()` from `Dog`.

## 3. The key intents

### Express an “is-a” relationship

A dog is an animal. Public inheritance lets code use a dog where an animal reference or pointer is expected.

For example, add this function before `main()` in the good program:

```cpp
void feed(const Animal& animal) {
    animal.eat();
}
```

Then add this line inside `main()`, after creating `dog`:

```cpp
feed(dog); // Prints: Eating food
```

A **reference** is an alias for an existing object. Here, `const Animal&` lets `feed()` use the existing dog's animal part without copying it or changing it through the reference.

`feed()` only needs the `Animal` operations. It does not need to know that the object is a dog.

A derived type should preserve the promises of its base type. If code can feed an animal, using a dog should still work correctly. This is the idea behind the **Liskov Substitution Principle**: objects of a subtype should be usable in place of objects of their supertype without violating the properties expected by clients.

### Reuse common behavior

`eat()` is defined once in `Animal`. `Dog` reuses it. Another suitable animal class could reuse it too.

If the shared eating behavior changes, its implementation has one place to change.

### Add specific behavior

`bark()` belongs to `Dog`. The base class does not need every action that every derived class might provide.

A function receiving `const Animal&` can call `eat()`, but it cannot call `bark()` through that reference because `Animal` does not declare it.

### Keep inheritance distinct from polymorphism

This example shares an existing function. It does not choose between different implementations of `eat()` at runtime.

That additional behavior requires virtual functions in this kind of class hierarchy. See [Polymorphism](05_Polymorphism.md) when you are ready for it [2].

## 4. Common mistakes

### Using inheritance for a “has-a” relationship

A dog **is an** animal, so inheritance fits.

A dog **has a** collar. A dog is not a collar, so `Dog` should not inherit from `Collar` just to use it.

**Composition is the construction of a class using objects of other classes as members.** In simple words, one object contains another. A collar stored inside a dog object would be composition.

### Assuming private members become public

Public inheritance does not give `Dog` direct access to private members of `Animal`.

| Base member access | What it means for `Dog` under public inheritance |
|--------------------|-------------------------------------------------|
| `public` | `Dog` and ordinary outside code can use it |
| `protected` | `Dog` can use it subject to protected-access rules; ordinary outside code cannot |
| `private` | `Dog` cannot access it directly; it must use operations exposed by `Animal` |

Our example uses public functions so the main relationship stays easy to see.

### Forgetting `public` in the inheritance declaration

For a declaration beginning with `class`, inheritance is private by default. Write `class Dog : public Animal` when outside code should be able to treat a dog as an animal.

With private inheritance, `dog.eat()` and `feed(dog)` would not be allowed from ordinary outside code in this example.

### Assuming the base class gets the derived class's features

`Animal` does not inherit from `Dog`. You cannot call `bark()` on an ordinary `Animal` object.

### Assuming every inherited function is virtual

`eat()` is non-virtual. If a derived class later defines a same-named function, calls through an `Animal&` still use the base version. Virtual functions and overriding are explained in the next note [2].

### Overlooking object lifetime

The base part is constructed before the derived part. Destruction happens in the reverse order. The compiler supplies the constructors and destructors needed by these simple classes.

The example uses a local `Dog` object, which is cleaned up automatically. If a later design deletes derived objects through base pointers, the base normally needs a public virtual destructor; the polymorphism note covers that rule [3].

## 5. Names you may encounter

You only need **single inheritance** for the example above. The other names describe larger arrangements of classes.

| Form | Standard meaning |
|------|------------------|
| Single inheritance | A derived class has one direct base class |
| Multiple inheritance | A derived class has more than one direct base class |
| Multilevel inheritance | A class derives from a class that itself derives from another class |
| Hierarchical inheritance | Multiple classes derive from the same base class |
| Hybrid inheritance | A hierarchy combines more than one of these patterns |

These are descriptions of class relationships, not separate C++ keywords.

## 6. A short design checklist

1. Can you honestly say the derived type “is a” base type?
2. Which behavior belongs in the base class?
3. What does the derived class add?
4. Can code using the base type still work correctly with the derived type?
5. Would composition better describe a “has-a” relationship?

## 7. Self-test

**Which is the base class: `Animal` or `Dog`?**

<details>
<summary>Answer</summary>

`Animal` is the base class. `Dog` is the derived class because it inherits from `Animal`.

</details>

**Why can we call `dog.eat()` when `Dog` does not define `eat()`?**

<details>
<summary>Answer</summary>

`Dog` publicly inherits `Animal`'s public `eat()` function.

</details>

**Can an ordinary `Animal` object call `bark()`?**

<details>
<summary>Answer</summary>

No. `bark()` is defined only in `Dog`. The base class does not acquire the derived class's added members.

</details>

**Why can `feed(const Animal& animal)` accept a dog?**

<details>
<summary>Answer</summary>

Public inheritance allows the reference to bind to the animal part of the existing dog object. No separate animal object is copied.

</details>

**Should `Dog` inherit from `Collar`?**

<details>
<summary>Answer</summary>

No. A dog has a collar; it is not a collar. Use composition for that relationship.

</details>

**Does this program use virtual-function dispatch?**

<details>
<summary>Answer</summary>

No. `eat()` is a shared non-virtual function. Inheritance does not automatically make functions virtual.

</details>

## 8. TL;DR

Inheritance lets a derived class reuse and extend a base class. `Dog` is an `Animal`: it inherits `eat()` and adds `bark()`. Use public inheritance for a valid “is-a” relationship and composition for a “has-a” relationship.

## Sources and further reading

- [1] C++ Core Guidelines: [C.120 — Use class hierarchies for inherently hierarchical concepts](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#rh-domain).
- [2] cppreference: [Virtual functions](https://en.cppreference.com/w/cpp/language/virtual.html), explaining how virtual dispatch differs from non-virtual calls.
- [3] C++ Core Guidelines: [C.35 — A base destructor should be public and virtual, or protected and non-virtual](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#rc-dtor-virtual), for designing base-class destruction interfaces.
