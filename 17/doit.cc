// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <vector>
#include <map>
#include <tuple>
#include <cassert>

using namespace std;

vector<int> containers;

void read() {
  int container;
  while (cin >> container)
    containers.push_back(container);
}

// max_containers == -1 means no constraint on number of containers
size_t count(int eggnog, int max_containers, size_t next,
             map<tuple<int, int, size_t>, size_t> &cache) {
  if (eggnog > 0 && max_containers == 0)
    return 0;
  auto key = make_tuple(eggnog, max_containers, next);
  auto p = cache.find(key);
  if (p != cache.end())
    return p->second;
  int capacity = containers[next];
  size_t ways;
  if (next == containers.size())
    ways = eggnog == 0 ? 1 : 0;
  else {
    ways = count(eggnog, max_containers, next + 1, cache);
    if (max_containers != 0) {
      int remaining = max_containers == -1 ? -1 : max_containers - 1;
      ways += count(eggnog - capacity, remaining, next + 1, cache);
    }
  }
  cache.emplace(key, ways);
  return ways;
}

void part1() {
  read();
  map<tuple<int, int, size_t>, size_t> cache;
  cout << count(150, -1, 0, cache) << '\n';
}

void part2() {
  read();
  for (size_t maxc = 1; maxc <= containers.size(); ++maxc) {
    map<tuple<int, int, size_t>, size_t> cache;
    size_t ways = count(150, maxc, 0, cache);
    if (ways > 0) {
      cout << ways << '\n';
      break;
    }
  }
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
