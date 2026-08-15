# Encapsulation — BankAccount

Source: `OOPS/ClassesAndObjects/2_Bank.cpp`

## What is encapsulation?

**Encapsulation** is the OOP principle of bundling data with the methods that operate on it and hiding the data behind a controlled interface. In C++ this is enforced with access specifiers: fields go under `private:`, and callers can only touch them through `public:` methods. The class becomes responsible for its own invariants — e.g. "balance never goes negative" — rather than trusting every caller to be careful.

## Class shape

```cpp
class BankAccount {
private:
    double _accountNumber;
    std::string _ownerName;
    double _balance;

public:
    BankAccount(double accountNumer, const std::string &ownerName)   // sic: typo preserved from source
        : _accountNumber{accountNumer},
          _ownerName{ownerName},
          _balance{0} {}
    // ...
};
```

- All fields are `private` — no caller can write `account._balance = -1e9;`.
- The constructor forces `_balance` to start at `0` regardless of what the caller passes; the balance is not a construction parameter.
- `const std::string &ownerName` avoids copying the name and forbids the constructor from mutating it.

## Guarded mutators

The two mutators (`deposit`, `withdraw`) validate their input before touching state — this is the whole point of hiding `_balance`.

```cpp
void deposit(double amount) {
    if (amount > 0) {
        _balance += amount;
    }
    // silently ignore non-positive deposits
}

bool withdraw(double amount) {
    if (_balance - amount >= 0) {
        _balance -= amount;
        return true;
    } else {
        std::cout << "Insufficient funds" << std::endl;
        return false;
    }
}
```

- `deposit` returns nothing and silently drops non-positive amounts. A stricter design would return `bool` or throw.
- `withdraw` returns `bool` so the caller knows whether the operation succeeded.
- `_balance - amount >= 0` is equivalent to `_balance >= amount` and slightly safer against negative `amount` (a negative `amount` would *increase* balance under naive subtraction — worth guarding against explicitly).

## `const` accessor

```cpp
double getBalance() const { return _balance; }
```

- Trailing `const` marks this method as non-mutating — it can be called on a `const BankAccount &`.
- Returns by value (`double` is cheap to copy). For a `std::string` you'd typically return `const std::string &` instead.

## Usage

```cpp
BankAccount account(1, "Vamsi");
account.deposit(1000);
std::cout << account.getBalance() << std::endl;   // 1000
```

Contrast with the "no encapsulation" version:
```cpp
struct BadAccount { double balance; };
BadAccount a{0};
a.balance = -5000;   // compiles fine — nothing enforces the invariant
```

## Design notes

| Choice | Rationale |
| --- | --- |
| Fields `private` | Prevents external code from breaking invariants |
| `deposit` ignores non-positive input | Keeps `_balance` monotonic on deposit path |
| `withdraw` returns `bool` | Lets caller react to insufficient funds |
| `_balance{0}` in initializer list | Balance is not a caller-controlled value |
| `getBalance() const` | Read-only access; usable in `const` contexts |

## Gotchas

- **`double` for money is a bug.** Floating-point can't represent `0.1` exactly, so repeated deposits/withdrawals accumulate rounding errors. Real code uses an integer count of the smallest unit (cents) or a decimal type.
- **Printing from a domain method** (`std::cout` inside `withdraw`) couples the class to a specific I/O sink. Prefer returning a status and letting the caller decide how to report it.
- **`_accountNumber` is a `double`.** Account numbers are identifiers, not quantities — `std::string` or a fixed-width integer type is a better fit. Doubles lose precision past ~15 digits.
- **No accessor for `_ownerName`.** The class stores the owner but exposes no way to read it — encapsulation is only useful if the class also offers a read path (`const std::string &ownerName() const`) for legitimate uses.
- **Missing `#include <string>`.** The source uses `std::string` but only includes `<iostream>`. It compiles because `<iostream>` transitively pulls in `<string>` on most implementations, but that's not guaranteed by the standard — always include the header for every type you name.
- **Silent failures** (deposit dropping negative amounts) are hard to debug. Return a `bool` or throw so bugs surface at the call site.
