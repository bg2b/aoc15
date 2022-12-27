// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

struct reindeer {
  string name;
  int speed;
  int endurance;
  int resting;

  reindeer(string const &s);

  int distance(int time) const;
};

reindeer::reindeer(string const &s) {
  stringstream ss(s);
  string _;
  ss >> name >> _ >> _ >> speed >> _ >> _ >> endurance >> _;
  ss >> _ >> _ >> _ >> _ >> _ >> resting >> _;
}

int reindeer::distance(int time) const {
  int period = endurance + resting;
  int dist = speed * endurance * (time / period);
  time %= period;
  dist += speed * min(endurance, time);
  return dist;
}

vector<reindeer> read() {
  vector<reindeer> result;
  string line;
  while (getline(cin, line))
    result.emplace_back(line);
  return result;
}

void part1() {
  int max_dist = 0;
  for (auto const &deer : read())
    max_dist = max(max_dist, deer.distance(2503));
  cout << max_dist << '\n';
}

void part2() {
  // Too annoying to try to be smarter given the short times
  auto herd = read();
  vector<int> points(herd.size());
  for (int time = 1; time <= 2503; ++time) {
    vector<int> dists;
    for (auto const &deer : herd)
      dists.push_back(deer.distance(time));
    int max_dist = *max_element(dists.begin(), dists.end());
    for (size_t i = 0; i < herd.size(); ++i)
      if (dists[i] == max_dist)
        ++points[i];
  }
  cout << *max_element(points.begin(), points.end()) << '\n';
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
