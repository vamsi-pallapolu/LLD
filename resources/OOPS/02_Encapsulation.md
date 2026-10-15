# Encapsulation: Let an Object Protect Its Data

**Encapsulation is the bundling of data and the methods that operate on that data into a single unit, such as a class, with controlled access to the unit's internal state.**

**Data hiding** is the restriction of direct access to an object's internal data. In C++, access specifiers such as `private` support data hiding as part of encapsulation.

For example, a parking meter lets you add time by paying, but it does not let you directly set its remaining time to any number you choose.

The object handles the checks, so every place that uses it does not have to repeat them.

## 1. Think of a thermostat

A thermostat lets you choose a target temperature using buttons.

Suppose it supports temperatures from 10 to 30 degrees Celsius. Pressing the buttons should never take the target outside that range.

You ask for a change. The thermostat decides whether that change is allowed.

In code:

- The stored temperature is `private`: outside code cannot change it directly.
- The buttons are `public` functions: outside code can call them.
- Each function keeps the temperature within the allowed range.

## 2. The problem with directly changing data

```cpp
#include <iostream>

struct Thermostat {
    int targetCelsius = 22;
};

int main() {
    Thermostat room;
    room.targetCelsius = 500;

    std::cout << room.targetCelsius << '\n';
}
```

This prints `500`. There is no check to stop the assignment.

If many parts of the program change `targetCelsius` directly, every one of them must remember the allowed range. Forget the check once, and the thermostat stores a value it should never accept.

## 3. Put the checks inside the class

This example uses whole degrees and changes the target one degree at a time.

```cpp
#include <iostream>
#include <stdexcept>

class Thermostat {
public:
    explicit Thermostat(int target) : targetCelsius_(target) {
        if (target < 10 || target > 30) {
            throw std::invalid_argument("Target must be between 10 and 30");
        }
    }

    void warmer() {
        if (targetCelsius_ < 30) {
            targetCelsius_ += 1;
        }
    }

    void cooler() {
        if (targetCelsius_ > 10) {
            targetCelsius_ -= 1;
        }
    }

    int target() const {
        return targetCelsius_;
    }

private:
    int targetCelsius_;
};

int main() {
    Thermostat room(22);

    room.warmer();
    std::cout << room.target() << '\n';

    room.cooler();
    std::cout << room.target() << '\n';

    Thermostat hottest(30);
    hottest.warmer();
    std::cout << hottest.target() << '\n';
}
```

Output:

```text
23
22
30
```

### Follow the example

1. `Thermostat room(22)` creates a thermostat with a target of 22.
2. The constructor, the function named `Thermostat`, checks that the starting target is allowed.
3. `room.warmer()` changes 22 to 23.
4. `room.cooler()` changes 23 back to 22.
5. `hottest` starts at 30. Calling `warmer()` does nothing because it is already at the upper limit.

The lower limit works the same way: calling `cooler()` at 10 leaves the target at 10.

### Read the C++ syntax

| Code | Plain meaning |
|------|---------------|
| `private:` | Outside code cannot directly use the data listed here |
| `public:` | Outside code can call the functions listed here |
| `explicit Thermostat(int target)` | Create a thermostat by supplying a target; do not let C++ automatically turn an integer into a thermostat |
| `: targetCelsius_(target)` | Give the stored temperature its starting value |
| `target < 10 \|\| target > 30` | The target is too low **or** too high |
| `void warmer()` | This function changes the target without returning a result |
| `int target() const` | Return the target without changing the thermostat |

`throw std::invalid_argument(...)` reports an error and stops creation of a thermostat with a bad starting target. This kind of error is called an **exception**. If the program does not handle it, the program stops.

The ending `_` in `targetCelsius_` is just a naming choice for a variable stored inside the object.

### What can outside code do?

It can request changes through `warmer()` and `cooler()`, or read the current target through `target()`.

This direct change is rejected by the compiler:

```cpp
room.targetCelsius_ = 500; // Error: targetCelsius_ is private.
```

The checks belong inside the thermostat. Code using it only needs to choose which action to request.

## 4. Private alone is not enough

Suppose we add this public function to the class:

```cpp
void setTarget(int value) {
    targetCelsius_ = value;
}
```

Now outside code can call `room.setTarget(500)`. The data is still private, but the new function lets a bad value through.

A function that directly sets a value is often called a **setter**. Setters are useful when a user needs to choose a value, but they must check any rules that apply.

Here is a checked version to use instead:

```cpp
void setTarget(int value) {
    if (value < 10 || value > 30) {
        throw std::invalid_argument("Target must be between 10 and 30");
    }
    targetCelsius_ = value;
}
```

The check happens before the stored temperature changes. If the value is rejected, the old temperature stays in place.

**A class invariant is a condition that must hold for every valid object of a class.** Here, the invariant is `10 <= targetCelsius_ && targetCelsius_ <= 30`. The constructor establishes it, and the public operations must preserve it.

The important point is **every way of changing the data must follow the rules**, including the constructor.

## 5. Reading data without giving away control

The example's `target()` function returns a copy of the stored number:

```cpp
int chosen = room.target();
chosen = 500;
```

Changing `chosen` does not change `room`. They are separate values.

Be careful about returning `int&` instead:

```cpp
int& target() {
    return targetCelsius_;
}
```

The `&` means a **reference**: another name for the actual stored number. With this version, outside code could write `room.target() = 500` and skip the checks.

For a small value like this temperature, returning a copy is simple and keeps control inside the class.

For larger data, C++ also supports `const&`: access to existing data without permission to change it through that reference. You can learn that detail when you need it.

## 6. The key intents

### Fewer checks to repeat

Code using the thermostat can call `warmer()` without checking the upper limit first. The thermostat already does that.

### Easier changes

Suppose you later store the temperature differently inside the class. If `warmer()`, `cooler()`, and `target()` still mean the same thing, code using those functions can keep working.

This is easier when other code does not depend on the stored variable's name or format.

### A clearer place to look for mistakes

If the stored target becomes 500, inspect the code inside the class that can change it. The allowed ways to change the value are gathered in one place.

## 7. Ask for an action instead of repeating its steps

Without a checked function, outside code might do this:

```text
Read the target.
Check whether it is below 30.
Add one.
Store the new target.
```

With the class, it simply calls:

```cpp
room.warmer();
```

The class handles the steps.

Reading data is still useful. For example, a screen needs `target()` to display the temperature. The goal is to keep the checks for changing the temperature inside the thermostat.

## 8. How is this different from abstraction?

| Idea | What it helps with | Thermostat example |
|------|--------------------|--------------------|
| **Encapsulation** | Bundling data and methods in a unit with controlled access to internal state | `Thermostat` groups the target and its operations, restricting direct access to the target |
| **Abstraction** | Representing essential features while hiding unnecessary implementation details | Call `warmer()` without knowing how the thermostat stores the target |

They work together. Encapsulation establishes the boundary around data and its operations; abstraction exposes the essential operations without requiring knowledge of their implementation.

See [Abstraction](03_Abstraction.md) for the next topic.

## 9. Common mistakes

- **Checking changes but not the starting value:** the constructor must also reject a bad target.
- **Adding an unchecked setter:** private data is not protected if a public function accepts any value.
- **Returning a reference that allows changes:** outside code may then bypass the checked functions.
- **Forgetting the limits:** check what happens at exactly 10 and 30, including repeated button presses.
- **Adding rules that are not needed:** a simple point with freely editable `x` and `y` values may be fine as a `struct`.

## 10. Before writing the class

1. List the data the object stores.
2. Write its rules in plain words, such as "the target stays between 10 and 30."
3. Keep data private when outside changes need to be controlled.
4. Check the starting values in the constructor.
5. Provide the actions other code needs.
6. Make every function that changes data keep the rules.
7. Let other code read what it needs without giving it an unchecked way to change the data.

## 11. Check your understanding

**Why is a public temperature variable a problem here?**

<details>
<summary>Answer</summary>

Any code can assign a value such as 500 without checking the allowed range.

</details>

**Does making the temperature private automatically fix everything?**

<details>
<summary>Answer</summary>

No. Every public function that changes it must also check the rules. An unchecked setter can still store 500.

</details>

**What happens when warmer() is called at 30?**

<details>
<summary>Answer</summary>

Nothing changes. The function only adds one when the target is below 30.

</details>

**Why return int from target() instead of int&?**

<details>
<summary>Answer</summary>

Returning int gives outside code a copy. Returning int& would let outside code change the stored temperature directly.

</details>

**Is a struct always a bad choice?**

<details>
<summary>Answer</summary>

No. It can be a good choice for simple data that other code is allowed to edit directly. Use checked functions when changes must follow rules.

</details>

## 12. TL;DR

Encapsulation bundles data and the methods that operate on it within a class and controls access to internal state. Data hiding restricts direct access to that state. A well-designed class establishes its invariants during construction and preserves them through its public operations.

## Sources and further reading

- Wikipedia: [Encapsulation](https://en.wikipedia.org/wiki/Encapsulation_(computer_programming))
- Martin Fowler: [Keeping actions with the data they use](https://martinfowler.com/bliki/TellDontAsk.html)
- C++ Core Guidelines: [Limit direct access to members](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#rc-private)
- C++ Core Guidelines: [Set up objects completely](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#rc-complete)
