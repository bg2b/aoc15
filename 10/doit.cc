// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <string>

using namespace std;

string say(string const &seq) {
  string result;
  for (size_t i = 0; i < seq.length(); ) {
    char digit = seq[i++];
    int num_repeats = 1;
    while (seq[i] == digit && i < seq.length()) {
      ++num_repeats;
      ++i;
    }
    result += to_string(num_repeats) + digit;
  }
  return result;
}

void solve(int num_times) {
  string seq;
  cin >> seq;
  for (int _ = 0; _ < num_times; ++_)
    seq = say(seq);
  cout << seq.length() << '\n';
}

void part1() { solve(40); }
void part2() { solve(50); }

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
