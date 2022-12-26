// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc md5.c
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <string>
#include <array>
#include <cassert>
#include "md5.h"

using namespace std;

string md5(string const &s) {
  md5_state_t pms;
  md5_init(&pms);
  md5_append(&pms, (md5_byte_t *)s.data(), s.length());
  array<unsigned char, 16> digest;
  md5_finish(&pms, digest.data());
  string sig;
  sig.reserve(2 * digest.size());
  auto hex = [](auto c) { return c > 9 ? c - 10 + 'a' : c + '0'; };
  for (auto c : digest) {
    sig.push_back(hex(c >> 4));
    sig.push_back(hex(c & 0xf));
  }
  return sig;
}

void mine(string const &wanted) {
  string prefix;
  cin >> prefix;
  unsigned ans = 1;
  while (md5(prefix + to_string(ans)).substr(0, wanted.length()) != wanted)
    ++ans;
  cout << ans << '\n';
}

void part1() { mine("00000"); }
void part2() { mine("000000"); }

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
