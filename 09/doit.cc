// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <optional>
#include <algorithm>

using namespace std;

void solve(int (*best_fn)(int, int)) {
  string place1, to, place2, equals;
  int dist;
  set<string> places;
  map<pair<string, string>, int> dists;
  while (cin >> place1 >> to >> place2 >> equals >> dist) {
    places.insert(place1);
    places.insert(place2);
    dists.emplace(make_pair(place1, place2), dist);
    dists.emplace(make_pair(place2, place1), dist);
  }
  vector<string> order(places.begin(), places.end());
  optional<int> best_dist;
  do {
    int total_dist = 0;
    for (size_t i = 0; i + 1 < order.size(); ++i)
      total_dist += dists[{ order[i], order[i + 1] }];
    best_dist = best_fn(best_dist.value_or(total_dist), total_dist);
  } while (next_permutation(order.begin(), order.end()));
  cout << *best_dist << '\n';
}

void part1() { solve([](int d1, int d2) { return min(d1, d2); }); }
void part2() { solve([](int d1, int d2) { return max(d1, d2); }); }

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
