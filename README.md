# LLD

Low-Level Design practice in C++, with topic-wise study notes under [`resources/`](./resources/).

## Layout

```
OOPS/
  ClassesAndObjects/   # class basics, encapsulation
  viz/                 # optional visualizer helpers (not covered in notes)
resources/
  OOPS/                # Markdown notes mirrored to OOPS/ subfolders
derived/               # build outputs (created by the build command)
```

## OOPS / ClassesAndObjects

| File | Concept | Notes |
| --- | --- | --- |
| [`Car.h`](./OOPS/ClassesAndObjects/Car.h) | Class definition — private fields, ctor initializer list, `const` getters, in-class member init | [01_Classes.md](./resources/OOPS/ClassesAndObjects/01_Classes.md) |
| [`1_Classe.cpp`](./OOPS/ClassesAndObjects/1_Classe.cpp) | Constructing and using a `Car` object | [01_Classes.md](./resources/OOPS/ClassesAndObjects/01_Classes.md) |
| [`2_Bank.cpp`](./OOPS/ClassesAndObjects/2_Bank.cpp) | Encapsulation — guarded `deposit` / `withdraw` on `BankAccount` | [02_Bank.md](./resources/OOPS/ClassesAndObjects/02_Bank.md) |
| [`3_Library.cpp`](./OOPS/ClassesAndObjects/3_Library.cpp) | *(empty — placeholder)* | — |
| [`1_Classe_gui.mm`](./OOPS/ClassesAndObjects/1_Classe_gui.mm) | Objective-C++ GUI wrapper around `Car` using the `viz` helpers | — |

## Building

Build outputs are routed to `derived/`. Example:

```
clang++ -std=c++17 OOPS/ClassesAndObjects/1_Classe.cpp -o derived/1_Classe
clang++ -std=c++17 OOPS/ClassesAndObjects/2_Bank.cpp   -o derived/2_Bank
```

For the Cocoa GUI variant (macOS only):

```
clang++ -std=c++17 -fobjc-arc -framework Cocoa \
  OOPS/viz/visualizer.mm OOPS/ClassesAndObjects/1_Classe_gui.mm \
  -o derived/1_Classe_gui
```
