# LLD

Low-Level Design practice in C++, with topic-wise study notes under [`resources/`](./resources/).

## Layout

```
OOPS/
  00_viz/              # optional visualizer helpers (not covered in notes)
  01_ClassesAndObjects/ # class basics, encapsulation
  02_Enums/            # enum basics and scoped enum practice
resources/
  OOPS/                # Markdown notes mirrored to OOPS/ subfolders
derived/               # build outputs (created by the build command)
```

## OOPS

| File | Concept | Notes |
| --- | --- | --- |
| [`Car.h`](./OOPS/01_ClassesAndObjects/Car.h) | Class definition — private fields, ctor initializer list, `const` getters, in-class member init | [01_Classes.md](./resources/OOPS/01_Classes.md) |
| [`1_Classe.cpp`](./OOPS/01_ClassesAndObjects/1_Classe.cpp) | Constructing and using a `Car` object | [01_Classes.md](./resources/OOPS/01_Classes.md) |
| [`2_Bank.cpp`](./OOPS/01_ClassesAndObjects/2_Bank.cpp) | Encapsulation — guarded `deposit` / `withdraw` on `BankAccount` | [02_Bank.md](./resources/OOPS/02_Bank.md) |
| [`3_Library.cpp`](./OOPS/01_ClassesAndObjects/3_Library.cpp) | *(empty — placeholder)* | — |
| [`1_Classe_gui.mm`](./OOPS/01_ClassesAndObjects/1_Classe_gui.mm) | Objective-C++ GUI wrapper around `Car` using the `viz` helpers | — |
| [`1_Practice.cpp`](./OOPS/02_Enums/1_Practice.cpp) | Enums — unscoped enum pitfalls, `enum class`, type-safe comparisons | [03_Enums.md](./resources/OOPS/03_Enums.md) |

## Building

Build outputs are routed to `derived/`. Example:

```
clang++ -std=c++17 OOPS/01_ClassesAndObjects/1_Classe.cpp -o derived/1_Classe
clang++ -std=c++17 OOPS/01_ClassesAndObjects/2_Bank.cpp   -o derived/2_Bank
clang++ -std=c++17 OOPS/02_Enums/1_Practice.cpp           -o derived/1_Practice
```

For the Cocoa GUI variant (macOS only):

```
clang++ -std=c++17 -fobjc-arc -framework Cocoa \
  OOPS/00_viz/visualizer.mm OOPS/01_ClassesAndObjects/1_Classe_gui.mm \
  -o derived/1_Classe_gui
```
