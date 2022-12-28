// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cassert>

using namespace std;

// np = number of properties
enum property { capacity = 0, durability, flavor, texture, calories, np };

struct ingredient {
  string name;
  int properties[np];

  ingredient(string const &s);
};

ingredient::ingredient(string const &s) {
  auto pos = s.find(':');
  assert(pos != string::npos);
  name = string(s.begin(), s.begin() + pos);
  for (int i = 0; i < np; ++i) {
    pos = s.find_first_of("-0123456789", pos);
    properties[i] = atoi(s.c_str() + pos);
    pos = s.find(',', pos);
  }
}

vector<ingredient> ingredients;

void read() {
  string line;
  while (getline(cin, line))
    ingredients.emplace_back(line);
}

void search(size_t next, int tsps, int desired_calories,
            vector<int> &amounts, int &best) {
  if (next == ingredients.size() - 1) {
    // Only one choice for last ingredient amount
    amounts[next] = tsps;
    int total_score = 1;
    for (int prop = capacity; prop < np; ++prop) {
      int prop_score = 0;
      for (size_t i = 0; i < ingredients.size(); ++i)
        prop_score += ingredients[i].properties[prop] * amounts[i];
      if (prop == calories) {
        if (desired_calories > 0 && prop_score != desired_calories)
          // Desired number of calories not met
          prop_score = 0;
        else
          // Calories don't count during the holidays
          prop_score = 1;
      }
      total_score *= max(prop_score, 0);
    }
    best = max(best, total_score);
    return;
  }
  for (int tsp = 0; tsp <= tsps; ++tsp) {
    amounts[next] = tsp;
    search(next + 1, tsps - tsp, desired_calories, amounts, best);
  }
}

void solve(int desired_calories) {
  read();
  vector<int> amounts(ingredients.size(), 0);
  int best = 0;
  search(0, 100, desired_calories, amounts, best);
  cout << best << '\n';
}

void part1() { solve(0); }
void part2() { solve(500); }

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
