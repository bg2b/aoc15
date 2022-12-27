// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <string>
#include <cassert>

using namespace std;

string next_passwd(string pwd) {
  assert(pwd.length() == 8);
  assert(pwd.find_first_not_of("abcdefghijklmnopqrstuvwxyz") == string::npos);
  while (true) {
    // Increment
    for (size_t i = pwd.length(); i-- > 0; )
      if (pwd[i] == 'z')
        pwd[i] = 'a';
      else {
        ++pwd[i];
        break;
      }
    // Check for illegal characters
    auto illegal = pwd.find_first_of("iol");
    if (illegal != string::npos) {
      // Skip ahead, next increment will roll over this character
      fill(pwd.begin() + illegal + 1, pwd.end(), 'z');
      continue;
    }
    // Check for a run like abc or pqr
    bool has_run = false;
    for (size_t i = 0; i + 2 < pwd.length(); ++i)
      if (pwd[i] + 1 == pwd[i + 1] && pwd[i] + 2 == pwd[i + 2])
        has_run = true;
    if (!has_run)
      continue;
    // Check for at least two pairs
    int num_pairs = 0;
    char last_pair = '_';
    for (size_t i = 0; i + 1 < pwd.length(); ++i)
      if (pwd[i] == pwd[i + 1] && pwd[i] != last_pair) {
        ++num_pairs;
        last_pair = pwd[i];
      }
    if (num_pairs >= 2)
      break;
  }
  return pwd;
}

void part1() {
  string pwd;
  cin >> pwd;
  cout << next_passwd(pwd) << '\n';
}

void part2() {
  string pwd;
  cin >> pwd;
  cout << next_passwd(next_passwd(pwd)) << '\n';
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
