// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <algorithm>
#include <optional>
#include <cassert>

using namespace std;

vector<unsigned> nums;
unsigned target;

void read(unsigned parts) {
  unsigned n;
  unsigned sum = 0;
  while (cin >> n) {
    nums.push_back(n);
    sum += n;
  }
  assert(sum % parts == 0);
  target = sum / parts;
  sort(nums.begin(), nums.end(), greater());
}

struct state {
  unsigned sum{0};
  unsigned next{0};
  set<unsigned> used;
};

bool operator<(state const &s1, state const &s2) {
  if (s1.sum != s2.sum)
    // Remember that the priority queue is a max queue.  Give priority
    // to larger sums (none of which can exceed the target).  The idea
    // is to bias towards large items to try to minimize the total
    // number of packages.
    return s1.sum < s2.sum;
  if (s1.used.size() != s2.used.size())
    return s1.used.size() > s2.used.size();
  if (s1.next != s2.next)
    return s1.next > s2.next;
  return s1.used < s2.used;
}

unsigned long entanglement(set<unsigned> const &used) {
  unsigned long result = 1;
  for (auto n : used)
    result *= n;
  return result;
}

// Search for splittings among candidates into a given number of
// parts.  The passenger compartment is the main search with
// candidates == nums and remaining_parts set to however many parts to
// split into.  For that one, I have to find a minimal number of
// packages, and secondarily by minimal entanglement.  Recursive
// searches with smaller numbers of parts are for validity checking
// only.  E.g., if I find a candidate for the passenger compartment, I
// still have to make sure that whatever remains can be split evenly
// for the sides.
set<unsigned> search(vector<unsigned> const &candidates, int remaining_parts) {
  priority_queue<state> q;
  q.push(state());
  optional<set<unsigned>> best;
  while (!q.empty()) {
    state s = q.top();
    q.pop();
    assert(s.sum <= target);
    if (s.sum == target) {
      if (candidates.size() != nums.size() && remaining_parts == 2)
        // This is a search just to find if there's a legal way to
        // split the remaining packages for the two sides
        return s.used;
      if (best && (best->size() < s.used.size() ||
                   entanglement(s.used) >= entanglement(*best)))
        // There's already a legal candidate, and even if this set
        // turns out to be legal it won't beat that
        continue;
      // I have to make sure that whatever is left can be split
      vector<unsigned> remaining;
      for (auto n : candidates)
        if (!s.used.count(n))
          remaining.push_back(n);
      if (search(remaining, remaining_parts - 1).empty())
        // There's no valid splitting of whatever was left, continue
        // searching for other possiblities
        continue;
      if (candidates.size() != nums.size())
        // This was a search for the trunk and there was a way to
        // balance the sides
        return s.used;
      // This is a search for the passenger compartment, it's legal,
      // and either this is the first candidate found, or it's better
      // than whatever the previous best was
      best = s.used;
      continue;
    }
    // Sum is less than target...
    if (best && s.used.size() >= best->size())
      // This is a search for the minimum number of packages, some
      // candidate has been found, and this set already has at least
      // that many elements.  Since more items will need to be added
      // to get to target, nothing following from this set can be the
      // best.
      continue;
    if (s.next >= candidates.size())
      // Ran out of packages to consider adding, no more successors
      continue;
    unsigned n = candidates[s.next++];
    if (s.sum + n <= target) {
      // This package wouldn't go over the target
      state include(s);
      include.sum += n;
      include.used.insert(n);
      q.push(include);
    }
    q.push(s);
  }
  if (candidates.size() < nums.size())
    // No way to split the candidates evenly, tell whatever called for
    // the subsearch that the splitting it's considering isn't valid
    return {};
  assert(best);
  return *best;
}

void solve(unsigned parts) {
  read(parts);
  cout << entanglement(search(nums, parts)) << '\n';
}

void part1() { solve(3); }
void part2() { solve(4); }

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
