// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <algorithm>

using namespace std;

void part1() {
  int l, w, h;
  char x;
  int ans = 0;
  while (cin >> l >> x >> w >> x >> h) {
    int longest = max(l, max(w, h));
    ans += 2 * (l * w + w * h + h * l) + l * w * h / longest;
  }
  cout << ans << '\n';
}

void part2() {
  int l, w, h;
  char x;
  int ans = 0;
  while (cin >> l >> x >> w >> x >> h) {
    int shorted_perim = 2 * min(l + w, min(w + h, h + l));
    ans += shorted_perim + l * w * h;
  }
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
