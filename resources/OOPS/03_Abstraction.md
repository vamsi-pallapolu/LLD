# Abstraction: Show What Matters

**Abstraction is the process of representing the essential features of an entity while hiding implementation details that are unnecessary to its users.** It emphasizes what an entity does rather than how it does it [1].

Think about playing music on your phone. You press **Play**. You do not need to know how the phone reads the song, processes the sound, or sends it to your headphones.

In code, the idea is the same: give other code a simple function to call, and keep the detailed work behind that function.

## 1. Start with something familiar

When you use a TV remote, you see buttons such as:

- Power
- Volume up
- Volume down

You do not need to understand the circuits inside the TV to use these buttons.

The buttons show **what you can do**. The TV handles **how it happens**.

That is abstraction.

**Hiding details does not mean the code must be secret.** It means you do not need to read or understand those details to use it.

### You already use abstraction in code

```cpp
player.play("Imagine");
```

Someone reading this line can understand the request: play the song called "Imagine".

They do not need to know how the song is loaded or which device produces the sound. Those details belong inside the code that does the work.

Even a well-named function like `calculateTotal()` is an abstraction. You do not need special C++ keywords every time you use this idea.

## 2. Bad vs. good: a music player with several devices

Imagine a music player that can send sound to a phone speaker or to wired headphones. To keep the examples easy to follow, each device prints a message instead of producing real sound.

### A player that knows every device

This complete program works:

```cpp
#include <iostream>
#include <string>

class MusicPlayer {
public:
    explicit MusicPlayer(const std::string& device) : device_(device) {}

    void connect(const std::string& device) {
        device_ = device;
    }

    void play(const std::string& song) {
        if (device_ == "speaker") {
            std::cout << "Phone speaker: " << song << '\n';
        } else if (device_ == "headphones") {
            std::cout << "Wired headphones: " << song << '\n';
        } else {
            std::cout << "Unknown device\n";
        }
    }

private:
    std::string device_;
};

int main() {
    MusicPlayer player("speaker");
    player.play("Imagine");

    player.connect("headphones");
    player.play("Hotel California");
}
```

Output:

```text
Phone speaker: Imagine
Wired headphones: Hotel California
```

The player knows the details of every device. Now add a smart TV: you must open `MusicPlayer` and add another `else if`. If pausing, stopping and changing volume each have their own `if` chain, every one of them must be updated. A typo such as `"speeker"` also compiles and only fails when the program runs.

### A player that only knows "something that can play"

Instead, give every device the same operation: `play(song)`. The player then says **"play this song"**, and the connected device handles the details.

The following steps form one program when placed together in order.

#### Step 1: Describe what every audio device must do

```cpp
#include <iostream>
#include <string>

class AudioOutput {
public:
    virtual ~AudioOutput() = default;
    virtual void play(const std::string& song) = 0;
};
```

Read `AudioOutput` as **"something that can play a song."**

It says that a `play()` function must exist, but it does not provide the steps for playing a song.

| Code | Plain meaning |
|------|---------------|
| `public:` | Other code is allowed to use these functions |
| `virtual` before `play` | Calling through `AudioOutput` runs the connected device's version of `play()` |
| `= 0` | `AudioOutput` gives no steps of its own; each device must supply them. This makes `play()` a **pure virtual function** |
| `const std::string& song` | Receive the song name without copying it, and do not change it |
| `virtual ~AudioOutput() = default;` | Allow correct cleanup if a device is deleted through an `AudioOutput` pointer |

You only need a reading-level understanding of `virtual` here. [Inheritance](04_Inheritance.md) and [Polymorphism](05_Polymorphism.md) explain how it works in detail.

**An abstract class is a class that cannot be instantiated directly.** In plain language, you cannot create an `AudioOutput` object by itself, because its `play()` function has no steps yet. You can only create a specific device that fills those steps in [2].

**An interface specifies the operations available to clients and the behavioral contract those operations must satisfy, independently of their implementation.** In plain language, an interface is a list of things you can ask for, plus a promise about what each one does. `AudioOutput` serves as an interface for audio devices. C++ does not have a separate `interface` keyword; an abstract class with no data plays that role [3][4].

#### Step 2: Let each device do the work its own way

```cpp
class PhoneSpeaker : public AudioOutput {
public:
    void play(const std::string& song) override {
        std::cout << "Phone speaker: " << song << '\n';
    }
};

class WiredHeadphones : public AudioOutput {
public:
    void play(const std::string& song) override {
        std::cout << "Wired headphones: " << song << '\n';
    }
};
```

`: public AudioOutput` means that a `PhoneSpeaker` or `WiredHeadphones` object can be used wherever an `AudioOutput` is expected.

`override` asks the compiler to check that this function correctly supplies the device's version of `play()`. It catches mistakes such as misspelling the function name.

Both devices accept the same request, but each has its own code to carry it out.

#### Step 3: Give the music player a device

```cpp
class MusicPlayer {
public:
    explicit MusicPlayer(AudioOutput& output) : output_(&output) {}

    void connect(AudioOutput& output) {
        output_ = &output;
    }

    void play(const std::string& song) {
        output_->play(song);
    }

private:
    AudioOutput* output_;
};
```

`MusicPlayer` knows only that its device has a `play()` function. It does not mention speakers or headphones.

The constructor, `MusicPlayer(AudioOutput& output)`, receives an existing device. The part `output_(&output)` stores that device's address. `explicit` means C++ should not automatically turn a device into a `MusicPlayer`.

`output_` is a **pointer**: it remembers where the connected device is. `connect()` changes which device it points to. `output_->play(song)` calls `play()` on that device.

**The player borrows the device.** It does not create or delete it. The device must still exist whenever the player uses it.

#### Step 4: Use the player

```cpp
int main() {
    PhoneSpeaker speaker;
    WiredHeadphones headphones;

    MusicPlayer player(speaker);
    player.play("Imagine");

    player.connect(headphones);
    player.play("Hotel California");
}
```

Output:

```text
Phone speaker: Imagine
Wired headphones: Hotel California
```

Follow the first call:

1. `player.play("Imagine")` asks the music player to play the song.
2. The player calls `play()` on its connected device.
3. The connected device is `speaker`, so `PhoneSpeaker::play()` runs.

After `connect(headphones)`, the same player code calls `WiredHeadphones::play()` instead.

**The request stays the same. The device doing the work changes.**

#### Step 5: Add another device

Place this class before `main()`:

```cpp
class SmartTV : public AudioOutput {
public:
    void play(const std::string& song) override {
        std::cout << "Smart TV: " << song << '\n';
    }
};
```

Then add these lines inside `main()`, after creating `player`:

```cpp
SmartTV tv;
player.connect(tv);
player.play("Imagine");
```

You add the new device and choose it in `main()`. **The code inside `MusicPlayer` needs no changes**, because the TV provides the same `play()` operation. Compare that with the bad version, where every new device meant editing the player.

## 3. The key intents

### Reduce what you need to understand

You can read `MusicPlayer` without learning how any device works. Each piece of code can be understood on its own.

### Change details without breaking users

A device can change its internal steps while keeping `play(song)` the same. Code that calls `play()` keeps working.

### Let one piece of code work with many implementations

The player can use any device that provides the expected operation, including ones written later.

### Make testing easier

You can give the player a pretend device that records song names, then check what it was asked to play, with no real speaker needed.

### Separate "what" from "how" without mixing it with "who may touch the data"

Abstraction and [Encapsulation](02_Encapsulation.md) often work together, but they answer different questions:

| Idea | Question | Music player example |
|------|----------|----------------------|
| **Abstraction** | What essential operations are exposed, independently of their implementation? | The player calls `play(song)` without knowing how any device makes sound |
| **Encapsulation** | How are data and methods bundled, and how is access to internal state controlled? | `MusicPlayer` keeps `output_` private, so outside code must use `connect()` to change the device |

## 4. Common mistakes

### Forcing operations that do not fit every implementation

`play(song)` makes sense for speakers, headphones and TVs. `pairWithBluetooth()` does not make sense for wired headphones. Keep it in the Bluetooth device's own class instead of requiring every audio device to provide it.

### Letting details leak into the shared operation

Avoid making the player pass a Bluetooth setting every time it plays a song:

```cpp
play(song, bluetoothChannel);
```

A speaker has no use for that setting. Set it up on the Bluetooth device separately, and let the player keep calling `play(song)`.

### Hiding failures along with the details

What if the device is disconnected? The player does not need to know how Bluetooth works, but it still needs a way to learn that playback failed.

For example, `play()` could return `true` when playback starts and `false` when it cannot. Every device would then have to follow that same rule. The example above leaves this out to focus on the main idea.

### Keeping the same name but breaking the promise

A device whose `play()` silently skips the song, or plays a different one, compiles fine but breaks the player. The benefits of abstraction depend on every implementation keeping the same promise, not merely having the right function name.

### Using a borrowed object after it is gone

The player only stores a pointer to its device. If the device is destroyed while the player still points to it, calling `play()` is undefined behavior: C++ makes no promise about what happens. Make sure the device lives at least as long as the player uses it.

### Building an interface too early

You do not need a shared base class for every class you write. A plain function or class can already give users a simple way to do something. Add a shared interface when it solves a real problem, such as letting one player work with several devices or using a pretend device in a test.

## 5. A short design checklist

1. Write down what users of your code need to ask for, in plain words.
2. Name each operation after **what** it does, not **how** it does it.
3. Include only operations that make sense for every implementation.
4. Keep settings and steps that belong to one implementation inside that implementation.
5. Decide how failures are reported, and make every implementation follow that rule.
6. Add an abstract class only when you have, or clearly expect, more than one implementation or a test double [5].
7. Check that adding a new implementation does not require editing code that uses the interface.

## 6. Check your understanding

**What is being hidden in the music-player example?**

<details>
<summary>Answer</summary>

The steps each device uses to play a song. The player only needs to know that it can call `play(song)`.

</details>

**What problem does the bad version have when a smart TV is added?**

<details>
<summary>Answer</summary>

You must edit `MusicPlayer` to add another `else if`, and repeat that in every function that checks the device. Device names are plain strings, so typos are only caught at runtime.

</details>

**Does abstraction always require an abstract class?**

<details>
<summary>Answer</summary>

No. A function such as `calculateTotal()` can hide detailed calculations behind one clear name. An abstract class is one C++ tool for sharing operations across different types of objects.

</details>

**Why can't we create an `AudioOutput` object directly?**

<details>
<summary>Answer</summary>

`AudioOutput` is abstract because `play()` is pure virtual: it has no steps of its own. Create an object of a concrete class, such as `PhoneSpeaker`, that supplies them.

</details>

**What changes when we add a smart TV in the good version?**

<details>
<summary>Answer</summary>

We add a `SmartTV` class and connect a TV object to the player in `main()`. The `MusicPlayer` class stays the same.

</details>

**Why shouldn't `play()` take a `bluetoothChannel` argument?**

<details>
<summary>Answer</summary>

Only Bluetooth devices need it. Adding it to the shared operation forces every device and every caller to deal with a detail that belongs to one implementation.

</details>

## 7. TL;DR

Abstraction represents essential features while hiding unnecessary implementation details, so users depend on **what** an operation does, not **how**. Functions, classes and interfaces can all provide abstractions; in C++, an abstract class with pure virtual functions is the usual way to describe an interface. Keep the shared operations small, keep implementation details inside each implementation, and make every implementation keep the same promise.

## Sources and further reading

- [1] Wikipedia: [Abstraction (computer science)](https://en.wikipedia.org/wiki/Abstraction_(computer_science)).
- [2] cppreference: [Abstract class](https://en.cppreference.com/w/cpp/language/abstract_class).
- [3] C++ Core Guidelines: [C.121 — If a base class is used as an interface, make it a pure abstract class](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#rh-abstract).
- [4] C++ Core Guidelines: [C.122 — Use abstract classes as interfaces when complete separation of interface and implementation is needed](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#rh-separation).
- [5] C++ Core Guidelines: [I.25 — Prefer empty abstract classes as interfaces to class hierarchies](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#ri-abstract).
