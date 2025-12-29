// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// cat tape input | ./doit 1  # part 1
// cat tape input | ./doit 2  # part 2

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <functional>
#include <cassert>

using namespace std;

map<string, int> tape;

void match(stringstream ss, function<bool(string const &, int)> compat) {
  int sue;
  char colon;
  ss >> sue >> colon;
  string item;
  int amount;
  while (ss >> item >> amount) {
    assert(item.back() == ':');
    item.pop_back();
    assert(tape.find(item) != tape.end());
    if (!compat(item, amount))
      return;
    if (ss.peek() == ',')
      ss.ignore(1);
  }
  cout << sue << '\n';
}

void solve(function<bool(string const &, int)> compat) {
  string item;
  while (cin >> item)
    if (item == "Sue") {
      string remembered;
      getline(cin, remembered);
      match(stringstream(remembered), compat);
    } else {
      assert(item.back() == ':');
      item.pop_back();
      cin >> tape[item];
    }
}

void part1() {
  solve([](string const &item, int amount) { return tape[item] == amount; });
}

void part2() {
  solve([](string const &item, int amount) {
    if (item == "cats" || item == "trees")
      return amount > tape[item];
    if (item == "pomeranians" || item == "goldfish")
      return amount < tape[item];
    return amount == tape[item];
  });
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
