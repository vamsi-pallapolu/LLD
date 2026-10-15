# `explicit`: No Silent Conversions

**The `explicit` specifier declares that a constructor or conversion function cannot be used for implicit conversions or copy-initialization** [1].

In plain language, `explicit` stops C++ from **silently turning a value into an object of your class**. The object can still be created, but only when you ask for it by name.

## 1. A real-world analogy: a currency exchange counter

Imagine a shop that only accepts euros. You hand over dollars. A helpful cashier could quietly convert them at some rate and carry on, and you might never notice what happened or what rate was used.

A careful shop says instead: "Please exchange your money at the counter first." The conversion still happens, but **you** choose it and you can see it.

- A constructor **without** `explicit` is the helpful cashier: it converts silently.
- A constructor **with** `explicit` is the careful shop: you must write the conversion yourself.

## 2. Bad vs. good

### Without `explicit`: C++ converts behind your back

A constructor that can be called with **one argument** also acts as a conversion rule. If C++ needs a `Seconds` but finds an `int`, it quietly calls the constructor.

```cpp
#include <iostream>

class Seconds {
public:
    Seconds(int value) : value_(value) {}

    int value() const {
        return value_;
    }

private:
    int value_;
};

void wait(Seconds duration) {
    std::cout << "Waiting " << duration.value() << " seconds\n";
}

int main() {
    wait(5);           // Compiles: C++ secretly runs Seconds(5).
    Seconds s = 10;    // Compiles: same secret conversion.
    wait(s);
}
```

Output:

```text
Waiting 5 seconds
Waiting 10 seconds
```

Step by step, this is what C++ does with `wait(5)`:

1. `wait` needs a `Seconds`.
2. You passed `5`, which is an `int`.
3. `Seconds` has a constructor that accepts an `int`.
4. C++ silently creates `Seconds(5)` and passes it in.

The whole point of a `Seconds` type is to make units clear. Yet `wait(5)` gives a reader no clue: 5 seconds? 5 minutes? 5 milliseconds? The silent conversion undoes the safety the type was meant to add.

### With `explicit`: you must ask for the conversion

Add one word to the constructor:

```cpp
#include <iostream>

class Seconds {
public:
    explicit Seconds(int value) : value_(value) {}

    int value() const {
        return value_;
    }

private:
    int value_;
};

void wait(Seconds duration) {
    std::cout << "Waiting " << duration.value() << " seconds\n";
}

int main() {
    wait(Seconds(5));  // OK: the conversion is written out.
    Seconds s(10);     // OK: direct construction.
    wait(s);

    // wait(5);        // Error: no silent conversion from int.
    // Seconds t = 10; // Error: "=" form needs an implicit conversion.
}
```

Output:

```text
Waiting 5 seconds
Waiting 10 seconds
```

### What changes

| Code | Without `explicit` | With `explicit` |
|------|--------------------|-----------------|
| `wait(5);` | Compiles silently | **Error** |
| `Seconds s = 10;` | Compiles silently | **Error** |
| `wait(Seconds(5));` | Compiles | Compiles |
| `Seconds s(10);` | Compiles | Compiles |
| `Seconds s{10};` | Compiles | Compiles |

**`explicit` never stops you from creating an object.** It only stops C++ from creating one *for* you without being asked.

### Two forms of initialization

These terms explain the `=` row in the table:

- **Direct-initialization** is initialization in which the constructor arguments are given directly, as in `Seconds s(10);` or `Seconds s{10};`. You name the type and its arguments, so `explicit` constructors are allowed.
- **Copy-initialization** is initialization from a value of another form, as in `Seconds s = 10;` or passing `5` to a `Seconds` parameter. C++ must *convert* the value, so `explicit` constructors are not considered [2].

## 3. A sneakier bug: a temporary object

This one prints the right text and is still wrong:

```cpp
#include <iostream>
#include <string>

class Animal {
public:
    Animal(const std::string& name) : name_(name) {}   // no explicit

    void eat() const {
        std::cout << name_ << " is eating\n";
    }

private:
    std::string name_;
};

void feed(const Animal& animal) {
    animal.eat();
}

int main() {
    std::string name = "Rex";
    feed(name);   // Compiles!
}
```

Output:

```text
Rex is eating
```

`feed()` expects an animal, but receives a piece of text. C++ silently builds a **temporary** `Animal` named Rex, feeds it, and throws it away when the line ends. **No real animal was fed.** If the program had a real `Animal rex("Rex")` somewhere, it would still be hungry.

With `explicit Animal(const std::string& name)`, the line `feed(name)` no longer compiles, and the mistake is caught immediately.

## 4. The key intents

### Prevent surprising conversions

An implicit conversion is invisible at the call site. `explicit` makes every conversion visible in the code, so readers see exactly which objects are created.

### Keep strong types strong

Types such as `Seconds`, `Meters` or `UserId` exist to stop mix-ups between plain numbers. Without `explicit`, any `int` slips in, and the type protects nothing.

### Catch mistakes at compile time

Passing the wrong kind of value becomes a compile error instead of a silent temporary object or a wrong unit at runtime.

### Make the author's intent clear

Leaving `explicit` off becomes a deliberate statement: "converting to this type is natural and safe." For example, `std::string s = "hello";` is meant to work.

## 5. Where `explicit` applies

### Single-argument constructors: the main case

Any constructor that **can** be called with one argument, including one where the other parameters have defaults:

```cpp
class Buffer {
public:
    explicit Buffer(int size, char fill = ' ');   // Callable with one argument.
};
```

Without `explicit`, `Buffer b = 64;` would silently create a 64-character buffer.

### Constructors with several arguments and `{}`

Since C++11, braces can supply several arguments at once. Without `explicit`, this compiles:

```cpp
struct Point {
    Point(int x, int y) : x(x), y(y) {}
    int x, y;
};

void draw(Point p);

draw({3, 4});   // Silently builds Point(3, 4).
```

This form is often harmless and readable, so many codebases allow it. Mark a multi-argument constructor `explicit` only if `{3, 4}` could be misunderstood.

### Conversion operators

A class can also convert *into* another type. The common case is checking whether something is valid:

```cpp
#include <iostream>

class Connection {
public:
    explicit operator bool() const {
        return connected_;
    }

private:
    bool connected_ = true;
};

int main() {
    Connection conn;

    if (conn) {                 // OK: conditions may use explicit bool.
        std::cout << "Connected\n";
    }

    // int n = conn;            // Error: no silent conversion to bool, then int.
    // bool b = conn;           // Error: "=" form needs an implicit conversion.
    bool b = static_cast<bool>(conn);   // OK: written out.
    std::cout << b << '\n';
}
```

Output:

```text
Connected
1
```

`explicit operator bool` lets you write `if (conn)` while blocking accidents such as `int n = conn;` or `conn + 1` [1].

### Where you do **not** need it

- **Constructors with no arguments,** such as `Seconds()`.
- **Copy and move constructors,** such as `Seconds(const Seconds&)`. Marking them `explicit` causes confusing errors when returning or passing objects.

## 6. Common mistakes

### Thinking `explicit` blocks object creation

It does not. `Seconds s(5);`, `Seconds s{5};` and `Seconds(5)` all still work. Only the silent forms are blocked.

### Forgetting constructors with default arguments

`Buffer(int size, char fill = ' ')` looks like a two-argument constructor, but it can be called with one. It needs `explicit` just as much.

### Expecting a string literal to convert all the way

Without `explicit`, `feed(name)` compiles when `name` is a `std::string`. But `feed("Rex")` does **not** compile even without `explicit`. C++ allows only **one** user-defined conversion in a silent chain, and `"Rex"` would need two: text → `std::string` → `Animal`. Do not rely on this rule for safety; use `explicit`.

### Marking everything `explicit` without thinking

A type that truly *is* its value, such as a string from text or a complex number from a `double`, can reasonably allow conversion. Decide on purpose.

## 7. A short checklist

1. Can this constructor be called with exactly one argument, counting defaults? If so, consider `explicit`.
2. Would a silent conversion ever surprise a reader or hide a bug? If so, add `explicit`.
3. Is the conversion truly natural, like text to string? Then leaving it off may be fine; do it on purpose.
4. Writing `operator bool`? Make it `explicit`.
5. Do not put `explicit` on copy or move constructors.

## 8. Self-test

**What does `explicit` prevent?**

<details>
<summary>Answer</summary>

Implicit conversions: C++ silently creating an object of your class from a value of another type, such as `wait(5)` creating a `Seconds`.

</details>

**With `explicit Seconds(int)`, which of these compile: `Seconds a(5);`, `Seconds b = 5;`, `wait(5);`, `wait(Seconds(5));`?**

<details>
<summary>Answer</summary>

`Seconds a(5);` and `wait(Seconds(5));` compile. `Seconds b = 5;` and `wait(5);` do not, because both need a silent conversion.

</details>

**Why is `feed(name)` a bug when `Animal`'s constructor is not explicit?**

<details>
<summary>Answer</summary>

C++ creates a temporary `Animal` from the string, feeds it, and destroys it. No existing animal is fed, yet the code compiles and prints a normal-looking message.

</details>

**Does `Buffer(int size, char fill = ' ')` need `explicit`?**

<details>
<summary>Answer</summary>

Usually yes. It can be called with one argument, so it allows `Buffer b = 64;` silently.

</details>

**Why make `operator bool` explicit?**

<details>
<summary>Answer</summary>

So `if (conn)` still works, but accidents such as `int n = conn;` or `conn + 1` are rejected.

</details>

**Should copy constructors be explicit?**

<details>
<summary>Answer</summary>

No. That would block ordinary copying, such as passing or returning objects by value, and cause confusing errors.

</details>

## 9. TL;DR

A constructor that can take one argument is also a silent converter. `explicit` turns that off, so `wait(5)` and `Seconds s = 5;` become errors while `Seconds s(5);` and `wait(Seconds(5))` still work. Put `explicit` on single-argument constructors and on `operator bool` by default; leave it off only when a conversion is genuinely natural, and never on copy or move constructors.

## Sources and further reading

- [1] cppreference: [explicit specifier](https://en.cppreference.com/w/cpp/language/explicit).
- [2] cppreference: [Copy-initialization](https://en.cppreference.com/w/cpp/language/copy_initialization) and [Converting constructor](https://en.cppreference.com/w/cpp/language/converting_constructor).
- C++ Core Guidelines: [C.46 — By default, declare single-argument constructors explicit](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#rc-explicit).
- C++ Core Guidelines: [C.164 — Avoid implicit conversion operators](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#ro-conversion).
