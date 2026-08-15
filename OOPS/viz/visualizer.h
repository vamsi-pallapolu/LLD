#pragma once

#include <functional>
#include <initializer_list>
#include <string>
#include <utility>
#include <vector>

namespace viz {

struct Bar { std::string label; double value; std::string display; };
using ActionFn = std::function<void()>;
struct Action { char key; std::string hint; ActionFn fn; };

class Snapshot {
public:
    std::string _title;
    std::vector<Bar> _bars;
    std::vector<std::string> _lines;
    std::vector<Action> _actions;

    Snapshot &title(std::string t) { _title = std::move(t); return *this; }
    Snapshot &bar(std::string label, double value, double max, std::string display) {
        _bars.push_back({std::move(label), max > 0 ? value / max : 0.0, std::move(display)});
        return *this;
    }
    Snapshot &line(std::string text) { _lines.push_back(std::move(text)); return *this; }
    Snapshot &action(char key, std::string hint, ActionFn fn) {
        _actions.push_back({key, std::move(hint), std::move(fn)});
        return *this;
    }
};

using DescribeFn = std::function<Snapshot()>;

// Show a set of objects. Each entry is a lambda that returns the object's
// current Snapshot — the framework refreshes it every frame.
void show(std::initializer_list<DescribeFn> describes, const std::string &windowTitle = "Objects");

}
