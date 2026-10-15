# LLD

Low-Level Design practice in C++, with topic-wise study notes under [`resources/`](./resources/).

## Layout

```
src/OOPS/
  00_viz/               # optional visualizer helpers (not covered in notes)
  01_ClassesAndObjects/ # class basics, encapsulation
  02_Enums/             # enum basics and scoped enum practice
  03_Interfaces/        # abstract classes as interfaces
Practice/               # scratch practice programs
resources/
  OOPS/                 # Markdown notes, numbered by topic
derived/                # build outputs (created by the build command)
```

## OOPS

| File | Concept | Notes |
| --- | --- | --- |
| [`Car.h`](./src/OOPS/01_ClassesAndObjects/Car.h) | Class definition — private fields, ctor initializer list, `const` getters, in-class member init | [01_ClassesAndObjects.md](./resources/OOPS/01_ClassesAndObjects.md) |
| [`1_Classe.cpp`](./src/OOPS/01_ClassesAndObjects/1_Classe.cpp) | Constructing and using a `Car` object | [01_ClassesAndObjects.md](./resources/OOPS/01_ClassesAndObjects.md) |
| [`2_Bank.cpp`](./src/OOPS/01_ClassesAndObjects/2_Bank.cpp) | Encapsulation — guarded `deposit` / `withdraw` on `BankAccount` | [02_Encapsulation.md](./resources/OOPS/02_Encapsulation.md) |
| [`3_Library.cpp`](./src/OOPS/01_ClassesAndObjects/3_Library.cpp) | *(empty — placeholder)* | — |
| [`1_Classe_gui.mm`](./src/OOPS/01_ClassesAndObjects/1_Classe_gui.mm) | Objective-C++ GUI wrapper around `Car` using the `viz` helpers | — |
| [`1_Practice.cpp`](./src/OOPS/02_Enums/1_Practice.cpp) | Enums — unscoped enum pitfalls, `enum class`, type-safe comparisons | — |
| [`PaymentGateway.cpp`](./src/OOPS/03_Interfaces/PaymentGateway.cpp) | Interfaces — pure virtual `initiatePayment`, virtual destructor | [03_Abstraction.md](./resources/OOPS/03_Abstraction.md) |

### Notes

1. [Classes and Objects](./resources/OOPS/01_ClassesAndObjects.md)
2. [Encapsulation](./resources/OOPS/02_Encapsulation.md)
3. [Abstraction](./resources/OOPS/03_Abstraction.md)
4. [Inheritance](./resources/OOPS/04_Inheritance.md)
5. [Polymorphism](./resources/OOPS/05_Polymorphism.md)

C++ language features:

- [`explicit`](./resources/Cpp/01_Explicit.md)

## Building

Build outputs are routed to `derived/`. Example:

```
clang++ -std=c++17 src/OOPS/01_ClassesAndObjects/1_Classe.cpp -o derived/1_Classe
clang++ -std=c++17 src/OOPS/01_ClassesAndObjects/2_Bank.cpp   -o derived/2_Bank
clang++ -std=c++17 src/OOPS/02_Enums/1_Practice.cpp           -o derived/1_Practice
```

For the Cocoa GUI variant (macOS only):

```
clang++ -std=c++17 -fobjc-arc -framework Cocoa \
  src/OOPS/00_viz/visualizer.mm src/OOPS/01_ClassesAndObjects/1_Classe_gui.mm \
  -o derived/1_Classe_gui
```
