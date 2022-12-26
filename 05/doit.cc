// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <string>
#include <functional>
#include <cassert>

using namespace std;

void count_nice(function<bool(string const &s)> is_nice) {
  int ans = 0;
  string s;
  while (getline(cin, s))
    if (is_nice(s))
      ++ans;
  cout << ans << '\n';
}

void part1() {
  auto is_nice =
    [](string const &s) {
      for (auto const &forbidden : { "ab", "cd", "pq", "xy" })
        if (s.find(forbidden) != string::npos)
          return false;
      int num_vowels = 0;
      auto const none = string::npos;
      for (size_t start = 0; (start = s.find_first_of("aeiou", start)) != none; ) {
        ++start;
        ++num_vowels;
      }
      if (num_vowels < 3)
        return false;
      int num_twins = 0;
      for (size_t i = 0; i + 1 < s.length(); ++i)
        if (s[i] == s[i + 1])
          ++num_twins;
      return num_twins > 0;
    };
  count_nice(is_nice);
}

void part2() {
  auto is_nice =
    [](string const &s) {
      int num_separated_twins = 0;
      bool has_dup_pair = false;
      for (size_t i = 0; i + 2 < s.length(); ++i) {
        if (s[i] == s[i + 2])
          ++num_separated_twins;
        string adj = s.substr(i, 2);
        if (s.find(adj, i + 2) != string::npos)
          has_dup_pair = true;
      }
      return num_separated_twins > 0 && has_dup_pair;
    };
  count_nice(is_nice);
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
