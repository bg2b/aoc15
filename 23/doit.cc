// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <vector>
#include <functional>
#include <cassert>

using namespace std;

using num = unsigned long;

struct computer {
  int pc{0};
  num reg[2]{0, 0};
  vector<function<void(int &pc, num reg[2])>> program;

  computer(num initial_a);

  num run();
};

computer::computer(num initial_a) {
  reg[0] = initial_a;
  string op;
  while (cin >> op) {
    function<void(int &pc, num reg[2])> fn;
    if (op == "hlf" || op == "tpl" || op == "inc") {
      string reg;
      cin >> reg;
      assert(reg == "a" || reg == "b");
      int reg_num = reg == "a" ? 0 : 1;
      if (op == "hlf")
        fn = [=](int &pc, num reg[2]) { reg[reg_num] /= 2; ++pc; };
      else if (op == "tpl")
        fn = [=](int &pc, num reg[2]) { reg[reg_num] *= 3; ++pc; };
      else
        fn = [=](int &pc, num reg[2]) { ++reg[reg_num]; ++pc; };
    } else if (op == "jmp") {
      int offset;
      cin >> offset;
      fn = [=](int &pc, num reg[2]) { pc += offset; };
    } else {
      assert(op == "jie" || op == "jio");
      string reg;
      int offset;
      cin >> reg >> offset;
      assert(reg.back() == ',');
      reg.pop_back();
      assert(reg == "a" || reg == "b");
      int reg_num = reg == "a" ? 0 : 1;
      if (op == "jie")
        fn = [=](int &pc, num reg[2]) {
               pc += (reg[reg_num] & 1) == 0 ? offset : 1;
             };
      else
        fn = [=](int &pc, num reg[2]) {
               pc += reg[reg_num] == 1 ? offset : 1;
             };
    }
    program.push_back(fn);
  }
}

num computer::run() {
  while (pc >= 0 && pc < int(program.size()))
    program[pc](pc, reg);
  return reg[1];
}

void part1() { cout << computer(0).run() << '\n'; }
void part2() { cout << computer(1).run() << '\n'; }

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
