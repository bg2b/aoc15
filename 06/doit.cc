// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <functional>
#include <algorithm>
#include <numeric>
#include <array>
#include <cassert>

using namespace std;

int const n = 1000;

using brightness = unsigned;
using light_op = function<brightness(brightness current)>;

struct grid {
  light_op const on_op;
  light_op const off_op;
  light_op const toggle_op;

  array<array<brightness, n>, n> lights;

  grid(light_op on_op_, light_op off_op_, light_op toggle_op_);

  void exec(int startr, int endr, int startc, int endc, light_op op);

  void on(int startr, int endr, int startc, int endc) {
    exec(startr, endr, startc, endc, on_op);
  }
  void off(int startr, int endr, int startc, int endc) {
    exec(startr, endr, startc, endc, off_op);
  }
  void toggle(int startr, int endr, int startc, int endc) {
    exec(startr, endr, startc, endc, toggle_op);
  }

  brightness total_brightness() const;
};

grid::grid(light_op on_op_, light_op off_op_, light_op toggle_op_)
    : on_op(on_op_), off_op(off_op_), toggle_op(toggle_op_) {
  for (int i = 0; i < n; ++i)
    fill(lights[i].begin(), lights[i].end(), 0);
}

void grid::exec(int startr, int endr, int startc, int endc, light_op op) {
  for (int i = startr; i < endr; ++i)
    transform(&lights[i][startc], &lights[i][endc], &lights[i][startc], op);
}

brightness grid::total_brightness() const {
  brightness result = 0;
  for (int i = 0; i < n; ++i)
    result = accumulate(lights[i].begin(), lights[i].end(), result);
  return result;
}

void solve(light_op on_op, light_op off_op, light_op toggle_op) {
  grid yard(on_op, off_op, toggle_op);
  string cmd;
  while (cin >> cmd) {
    if (cmd == "turn")
      cin >> cmd;
    int startr, endr, startc, endc;
    string thru;
    char comma;
    cin >> startr >> comma >> startc >> thru >> endr >> comma >> endc;
    assert(cin);
    ++endr;
    ++endc;
    assert(0 <= startr && startr < endr && endr <= n);
    assert(0 <= startc && startc < endc && endc <= n);
    if (cmd == "on")
      yard.on(startr, endr, startc, endc);
    else if (cmd == "off")
      yard.off(startr, endr, startc, endc);
    else {
      assert(cmd == "toggle");
      yard.toggle(startr, endr, startc, endc);
    }
  }
  cout << yard.total_brightness() << '\n';
}

void part1() {
  solve([](brightness) { return 1; }, [](brightness) { return 0; },
        [](brightness b) { return !b; });
}

void part2() {
  solve([](brightness b) { return b + 1; },
        [](brightness b) { return b == 0 ? 0 : b - 1; },
        [](brightness b) { return b + 2; });
}

int main(int argc, char **argv) {
  if (argc != 2) {
    cerr << "usage: " << argv[0] << " partnum < input\n";
    exit(1);
  }
  if (*argv[1] == '1')
    part1();
  else
    part2();
  return 0;
}
