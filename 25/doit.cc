// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cassert>

using namespace std;

using num = long;

void part1() {
  string line;
  getline(cin, line);
  stringstream ss(line.substr(line.find("row ")));
  string row, column;
  int r, c;
  char comma;
  ss >> row >> r >> comma >> column >> c;
  int diagonal = r + c - 1;
  int ll_of_diag = diagonal * (diagonal - 1) / 2 + 1;
  int code_num = ll_of_diag + c - 1;
  num const code1 = 20151125;
  num const b = 252533;
  num const m = 33554393;
  int expt = code_num - 1;
  // Want code1 * (b ** expt) mod m;
  num ans = code1;
  num b_to_the_2_to_the_k = b;
  for (int k = 0; (1 << k) <= expt; ++k) {
    if ((expt & (1 << k)) != 0)
      ans = (ans * b_to_the_2_to_the_k) % m;
    b_to_the_2_to_the_k *= b_to_the_2_to_the_k;
    b_to_the_2_to_the_k %= m;
  }
  cout << ans << '\n';
}

void part2() {
  cout << "Start the Weather Machine!\n";
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
