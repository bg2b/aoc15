// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <cassert>

using namespace std;

void solve(size_t num_santas) {
  string directions;
  getline(cin, directions);
  assert(directions.length() % num_santas == 0);
  vector<pair<int, int>> locs(num_santas, {0, 0});
  set<pair<int, int>> visited;
  visited.insert(locs.front());
  for (size_t i = 0; i < directions.length(); i += num_santas) {
    for (size_t santa = 0; santa < num_santas; ++santa) {
      char dir = directions[i + santa];
      switch (dir) {
      case '<': --locs[santa].first; break;
      case '>': ++locs[santa].first; break;
      case '^': --locs[santa].second; break;
      case 'v': ++locs[santa].second; break;
      default: assert(dir == '<' || dir == '>' || dir == '^' || dir == 'v');
      }
      visited.insert(locs[santa]);
    }
  }
  cout << visited.size() << '\n';
}

void part1() { solve(1); }
void part2() { solve(2); }

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
