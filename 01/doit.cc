// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <string>
#include <algorithm>
#include <cassert>

using namespace std;

void part1() {
  string line;
  getline(cin, line);
  int up = count(line.begin(), line.end(), '(');
  int down = count(line.begin(), line.end(), ')');
  cout << up - down << '\n';
}

void part2() {
  string line;
  getline(cin, line);
  int floor = 0;
  size_t ans;
  for (ans = 0; ans < line.size() && floor >= 0; ++ans)
    floor += line[ans] == '(' ? +1 : -1;
  assert(floor < 0);
  cout << ans << '\n';
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
