// -*- C++ -*-
// Not overly fast for part 2, optimization suggested
// g++ -std=c++17 -Wall -Og -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <cctype>
#include <cassert>

using namespace std;

using atom = string;
using atoms = vector<atom>;

// Production rules (atom => replacement atoms)
vector<pair<atom, atoms>> rules;
// The target
atoms medicine;
// Things that have a rule which can expand them
set<atom> expandable;

// Atoms are all like H (single upper) or Cr (upper then lower),
// except for the starting e
atoms split_atoms(string const &s) {
  atoms result;
  size_t i = 0;
  while (i < s.length()) {
    assert(isupper(s[i]) || s[i] == 'e');
    atom a(1, s[i++]);
    if (i < s.length() && islower(s[i]))
      a.push_back(s[i++]);
    result.push_back(a);
  }
  return result;
}

void read() {
  string line;
  while (getline(cin, line) && line != "") {
    stringstream ss(line);
    string from, arrow, to;
    ss >> from >> arrow >> to;
    assert(arrow == "=>");
    atoms split = split_atoms(to);
    // All rules increasing in size, except e is allowed to have
    // single replacement
    assert(split.size() > 1 || from == "e");
    rules.emplace_back(from, split);
    expandable.insert(from);
  }
  getline(cin, line);
  medicine = split_atoms(line);
}

void part1() {
  read();
  set<atoms> products;
  for (auto const &rule : rules) {
    auto const &[from, to] = rule;
    for (size_t i = 0; i < medicine.size(); ++i)
      if (medicine[i] == from) {
        atoms prod(medicine.begin() + 0, medicine.begin() + i);
        prod.insert(prod.end(), to.begin(), to.end());
        prod.insert(prod.end(), medicine.begin() + i + 1, medicine.end());
        products.insert(prod);
      }
  }
  cout << products.size() << '\n';
}

// Synthesis uses a dynamic programming approach.  The best way to
// synthesize some piece of the medicine is to apply one rule that
// splits the piece into subpieces, optimally synthesize each of
// those, and add up all the steps.
//
// The general case is synthesis of medicine[i, j) from atom a...,
// i.e., (i, j, a) is the key for the dynamic programming cache.
using requirement = tuple<size_t, size_t, atom>;

// That synthesis uses s steps in the optimal case.  If it was
// necessary to actually construct the synthesis at the end, the
// starting rule and how it splits the piece should also be saved,
// i.e., the entries in the cache should be (s, rule, splitting).  I
// only need the size here though.  Entries are an optional, where
// null means the piece can't be done using that starting atom.
map<requirement, optional<size_t>> cache;

// Helper function, finds valid ways to split medicine [i, j) from a
// sequence of atoms.  Returns a vector of partitions, i.e., sequences
// of numbers [i, k, ..., j) in increasing order.  The meaning is that
// the first atom in step should synthesize [i, k), the next [k, ...),
// ..., and the last [..., j).
vector<vector<size_t>> split(size_t i, size_t j, atoms step) {
  assert(i <= j);
  if (i + step.size() > j)
    // Too short, no synthesis possible
    return {};
  if (step.empty()) {
    // Trivial, nothing left to synthesize with
    if (i == j)
      // Nothing left to synthesize, that's OK
      return { vector<size_t>({ i }) };
    // Can't make something from nothing
    return {};
  }
  // See what the last atom should match
  atom last = step.back();
  step.pop_back();
  if (!expandable.count(last) && medicine[j - 1] != last)
    // The last atom has no rules for further expansion, and the
    // element at medicine[j - 1] doesn't match it; this can't work
    return {};
  // Consider all possible prefixes for the rest.  If the last atom
  // has no expansions, the prefix must match everything up to j - 1,
  // otherwise have to consider all possibilities for what the last
  // atom might match.
  size_t prefix_end = expandable.count(last) ? i : j - 1;
  vector<vector<size_t>> result;
  for (size_t k = prefix_end; k < j; ++k)
    for (auto const &prefix_split : split(i, k, step)) {
      auto this_split = prefix_split;
      // Last segment in the partition is [k, j)
      this_split.push_back(j);
      result.push_back(this_split);
    }
  return result;
}

// Find the number of steps in the best way to synthesize [i, j) from
// an atom
//optional<size_t> &synthesize(requirement const &req) {
optional<size_t> &synthesize(size_t i, size_t j, atom const &a) {
  assert(i < j);
  requirement req({ i, j, a });
  auto p = cache.find(req);
  if (p != cache.end())
    return p->second;
  if (i + 1 == j && a != "e") {
    if (medicine[i] == a)
      // Trivial synthesis, no steps
      p = cache.emplace(req, 0).first;
    else
      // Not possible (replacements for anything except e are
      // guaranteed to make things bigger)
      p = cache.emplace(req, nullopt).first;
    return p->second;
  }
  for (size_t r = 0; r < rules.size(); ++r) {
    auto const &rule = rules[r];
    if (rule.first != a)
      // Wrong atom
      continue;
    auto const &step = rule.second;
    // Consider all ways to apply the rule
    auto partitions = split(i, j, step);
    for (auto const &partition : partitions) {
      assert(partition.size() == step.size() + 1);
      bool workable = true;
      // Start with one step (applying the rule)
      size_t total_steps = 1;
      for (size_t k = 0; k < step.size() && workable; ++k) {
        auto &rx = synthesize(partition[k], partition[k + 1], step[k]);
        if (!rx)
          workable = false;
        total_steps += *rx;
      }
      if (workable) {
        // Found a solution
        if (p == cache.end())
          p = cache.emplace(req, total_steps).first;
        assert(p->second);
        // Keep the best
        p->second = min(total_steps, *p->second);
      }
    }
  }
  if (p == cache.end())
    // No synthesis found
    p = cache.emplace(req, nullopt).first;
  return p->second;
}

void part2() {
  read();
  auto ps = split(0, medicine.size(), split_atoms("CRnAlHAr"));
  auto &rx = synthesize(0, medicine.size(), "e");
  assert(rx);
  cout << *rx << '\n';
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
