// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

struct grid {
  bool corners_stuck;
  vector<string> cells;

  grid(bool corners_stuck_);

  // Handle stuck corners
  void maybe_light_corners();

  // . for out of range
  char at(int i, int j) const;

  // How many neighbors are on?
  int lit_neighbors(int i, int j) const;

  // Animate one step
  void next();

  // How many lights are on?
  size_t total_lit() const;
};

grid::grid(bool corners_stuck_) : corners_stuck(corners_stuck_) {
  string line;
  while (getline(cin, line)) {
    cells.push_back(line);
    assert(line.length() == cells.front().length());
  }
  maybe_light_corners();
}

void grid::maybe_light_corners() {
  if (corners_stuck) {
    cells.front().front() = '#';
    cells.front().back() = '#';
    cells.back().front() = '#';
    cells.back().back() = '#';
  }
}

char grid::at(int i, int j) const {
  if (i < 0 || j < 0 || i >= int(cells.size()) || j >= int(cells[i].length()))
    return '.';
  return cells[i][j];
}

int grid::lit_neighbors(int i, int j) const {
  int lit = at(i, j) != '.' ? -1 : 0;
  for (int di = -1; di <= +1; ++di)
    for (int dj = -1; dj <= +1; ++dj)
      lit += at(i + di, j + dj) != '.' ? 1 : 0;
  return lit;
}

void grid::next() {
  vector<string> next_cells = cells;
  for (size_t i = 0; i < next_cells.size(); ++i)
    for (size_t j = 0; j < next_cells[i].length(); ++j)
      switch (lit_neighbors(i, j)) {
      case 3:
        next_cells[i][j] = '#';
        break;
      case 2:
        next_cells[i][j] = cells[i][j];
        break;
      default:
        next_cells[i][j] = '.';
        break;
      }
  cells = next_cells;
  maybe_light_corners();
}

size_t grid::total_lit() const {
  size_t lit = 0;
  for (auto const &s : cells)
    lit += count(s.begin(), s.end(), '#');
  return lit;
}

void solve(bool corners_stuck) {
  grid g(corners_stuck);
  for (int _ = 0; _ < 100; ++_)
    g.next();
  cout << g.total_lit() << '\n';
}

void part1() { solve(false); }
void part2() { solve(true); }

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
