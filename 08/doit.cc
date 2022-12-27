// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <string>
#include <cassert>

using namespace std;

char hex(char c) {
  return c >= 'a' ? c - 'a' + 10 : c - '0';
}

string in_memory(string const &coded) {
  assert(coded.length() >= 2 && coded.front() == '"' && coded.back() == '"');
  string result;
  for (size_t i = 1; i < coded.length() - 1; ) {
    if (coded[i] == '\\') {
      ++i;
      if (coded[i] == '"') {
        ++i;
        result.push_back('"');
      } else if (coded[i] == '\\') {
        ++i;
        result.push_back('\\');
      } else {
        assert(coded[i] == 'x' && i + 2 < coded.length() - 1);
        ++i;
        char c = hex(coded[i++]);
        c <<= 4;
        c |= hex(coded[i++]);
        result.push_back(c);
      }
    } else
      result.push_back(coded[i++]);
  }
  return result;
}

void part1() {
  string line;
  int ans = 0;
  while (getline(cin, line))
    ans += line.length() - in_memory(line).length();
  cout << ans << '\n';
}

string encoded(string const &in_memory) {
  string result;
  result.push_back('"');
  for (char c : in_memory) {
    if (c == '"' || c == '\\')
      result.push_back('\\');
    result.push_back(c);
  }
  result.push_back('"');
  return result;
}

void part2() {
  string line;
  int ans = 0;
  while (getline(cin, line))
    ans += encoded(line).length() - line.length();
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
