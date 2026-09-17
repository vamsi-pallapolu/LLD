# Enums in C++

Source: `OOPS/02_Enums/1_Practice.cpp`

## What is an enum?

An **enum** is a user-defined type whose values come from a fixed set of named constants. Use it when a variable should only represent one of a small number of states: colors, traffic-light states, order statuses, directions, modes, etc.

```cpp
enum Color { RED, YELLOW, GREEN };

Color c = RED;
```

By default, enumerators start at `0` and increase by `1`:

| Enumerator | Default value |
| --- | ---: |
| `RED` | `0` |
| `YELLOW` | `1` |
| `GREEN` | `2` |

You can also assign values explicitly:

```cpp
enum HttpStatus {
    OK = 200,
    NOT_FOUND = 404,
    SERVER_ERROR = 500
};
```

## Unscoped enums

The older enum form is called an **unscoped enum**:

```cpp
enum Color { RED, YELLOW, GREEN };
enum TrafficLight { RED_LIGHT, ORANGE_LIGHT, GREEN_LIGHT };
```

Its enumerator names are injected into the surrounding scope, so callers write `RED` instead of `Color::RED`.

```cpp
Color c = RED;       // no qualification needed
int x = c;           // implicit conversion to int
std::cout << x;      // prints 0
```

That convenience is also the problem: unscoped enums implicitly convert to integers.

```cpp
if (c == RED_LIGHT) {
    std::cout << "This shouldn't be allowed\n";
}
```

`c` is a `Color`, while `RED_LIGHT` is a `TrafficLight`. They are different concepts, but the comparison can still compile because both values can be treated like integers. `clang++` warns with `-Wenum-compare`, but it does not necessarily reject the code.

## Name pollution and collisions

Unscoped enum values share the surrounding namespace.

```cpp
enum Color { RED, YELLOW, GREEN };
enum Alert { RED, ORANGE };     // error: RED already exists in this scope
```

The sample also declares an enum inside `main`:

```cpp
enum TrafficLight {
    RED
};
```

This local enum is in the function block scope, so its `RED` is different from the global `RED`. Code like this is legal in some cases, but it is confusing because the nearest scoped name hides the outer one. Avoid reusing enumerator names unless the enum is scoped with `enum class`.

## Scoped enums: `enum class`

Modern C++ usually prefers **scoped enums**, written with `enum class`.

```cpp
enum class Color2 { RED, YELLOW, GREEN };
enum class TrafficLight2 { RED_LIGHT, YELLOW_LIGHT, GREEN_LIGHT };
```

Scoped enums fix the two big problems:

- Enumerator names stay inside the enum type.
- Values do not implicitly convert to `int`.

```cpp
Color2 c2 = Color2::RED;    // must qualify with Color2::

// int y = c2;              // compiler error: no implicit conversion

if (c2 == Color2::RED) {
    std::cout << "Safe comparison\n";
}
```

This comparison is type-safe:

```cpp
// c2 == TrafficLight2::RED_LIGHT;    // compiler error
```

`Color2` and `TrafficLight2` are different enum types, so C++ prevents accidentally comparing unrelated states.

## Underlying type

Every enum has an underlying integer type. For unscoped enums, the compiler chooses one unless you specify it. For scoped enums, the default underlying type is `int`.

```cpp
enum class Direction : unsigned char {
    North,
    East,
    South,
    West
};
```

Specifying the underlying type is useful when storage size, binary format, or API boundaries matter. In ordinary application code, the default is usually fine.

To intentionally convert a scoped enum to an integer, use an explicit cast:

```cpp
Color2 c = Color2::GREEN;
int raw = static_cast<int>(c);   // raw == 2
```

The cast is visible at the call site, which makes it harder to mix enum values with arbitrary integers by accident.

## `switch` with enums

Enums work well with `switch` because the set of valid cases is known.

```cpp
enum class TrafficLight {
    Red,
    Yellow,
    Green
};

void react(TrafficLight light) {
    switch (light) {
    case TrafficLight::Red:
        std::cout << "Stop\n";
        break;
    case TrafficLight::Yellow:
        std::cout << "Slow down\n";
        break;
    case TrafficLight::Green:
        std::cout << "Go\n";
        break;
    }
}
```

If you omit `default`, many compilers can warn when a new enum value is added but not handled. That warning is useful during maintenance. Add `default` only when there is a real fallback behavior.

## Printing enum values

Enums do not automatically print their names.

```cpp
std::cout << Color2::RED;    // error for enum class
```

Create a helper when you need readable output:

```cpp
const char *toString(Color2 color) {
    switch (color) {
    case Color2::RED:
        return "RED";
    case Color2::YELLOW:
        return "YELLOW";
    case Color2::GREEN:
        return "GREEN";
    }
    return "UNKNOWN";
}
```

Then:

```cpp
std::cout << toString(Color2::RED) << '\n';
```

## Unscoped enum vs scoped enum

| Feature | `enum` | `enum class` |
| --- | --- | --- |
| Enumerator access | `RED` | `Color::Red` |
| Name leakage | Leaks into surrounding scope | Stays inside enum scope |
| Implicit `int` conversion | Yes | No |
| Accidental cross-enum comparison | Can compile | Rejected |
| Modern default choice | Avoid unless needed | Prefer |

## Common patterns

| Pattern | When to use |
| --- | --- |
| `enum class Status { Pending, Paid, Failed };` | Domain states with type safety |
| `static_cast<int>(value)` | Intentional conversion for logging, serialization, or indexing |
| `switch` over enum values | Branching over a closed set of states |
| Explicit underlying type `: uint8_t` | Storage/binary protocol constraints |
| `toString(Enum value)` helper | Human-readable output |

## Gotchas

- **Prefer `enum class` by default.** It avoids name collisions and accidental integer behavior.
- **Unscoped enums can compare across different enum types.** Compilers may warn, but relying on warnings is weaker than using a type that rejects the bug.
- **Enumerator values are not strings.** `RED` is a named constant, not the text `"RED"`.
- **Do not depend on implicit numeric values unless they are part of the design.** If the numeric value matters, assign it explicitly.
- **Be careful when using enum values as array indexes.** Scoped enums require an explicit cast; this is good because it makes the indexing assumption visible.
- **Avoid reusing names like `RED` across unscoped enums.** Use `enum class` or unique prefixes such as `COLOR_RED` if you must work with old-style enums.
