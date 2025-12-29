// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <cassert>

using namespace std;

void seating(bool with_self) {
  string person, _, what, neighbor;
  int amount;
  set<string> people;
  map<pair<string, string>, int> happiness;
  while (cin >> person >> _ >> what >> amount >> _ >> _ >> _ >> _ >> _ >> _ >>
         neighbor) {
    people.insert(person);
    if (what == "lose")
      amount = -amount;
    assert(neighbor.back() == '.');
    neighbor.pop_back();
    happiness.emplace(make_pair(person, neighbor), amount);
  }
  if (with_self) {
    for (auto const &other : people) {
      happiness.emplace(make_pair("me", other), 0);
      happiness.emplace(make_pair(other, "me"), 0);
    }
    people.insert("me");
  }
  vector<string> ppl(people.begin(), people.end());
  size_t np = ppl.size();
  int most_happiness = 0;
  do {
    int total_happiness = 0;
    for (size_t i = 0; i < np; ++i) {
      total_happiness += happiness[{ppl[i], ppl[(i + 1) % np]}];
      total_happiness += happiness[{ppl[i], ppl[(i + np - 1) % np]}];
    }
    most_happiness = max(most_happiness, total_happiness);
  } while (next_permutation(ppl.begin(), ppl.end()));
  cout << most_happiness << '\n';
}

void part1() { seating(false); }
void part2() { seating(true); }

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
