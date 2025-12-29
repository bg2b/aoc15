// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <optional>
#include <tuple>
#include <cstdint>
#include <cassert>

using namespace std;

using bits = uint16_t;
using gate_op = function<bits(bits, bits)>;

struct circuit;

gate_op identity_op = [](bits b, bits) { return b; };
gate_op not_op = [](bits b, bits) { return ~b; };
gate_op and_op = [](bits b1, bits b2) { return b1 & b2; };
gate_op or_op = [](bits b1, bits b2) { return b1 | b2; };
gate_op lshift_op = [](bits b1, bits b2) { return b1 << b2; };
gate_op rshift_op = [](bits b1, bits b2) { return b1 >> b2; };

map<string, gate_op> ops = {{"AND", and_op},
                            {"OR", or_op},
                            {"LSHIFT", lshift_op},
                            {"RSHIFT", rshift_op}};

struct gate {
  // Value for a constant, cache for logic functions
  optional<bits> output;
  optional<tuple<gate_op, string, string>> logic;

  gate(bits val) : output(val) {}
  gate(string const &in) : logic({identity_op, in, in}) {}
  gate(gate_op op, string const &in1, string const &in2)
      : logic({op, in1, in2}) {}

  bits eval(circuit &cir);

  // Reset the cached value
  void reset() {
    if (logic)
      output.reset();
  }
};

struct circuit {
  map<string, gate> gates;

  // Construct from stdin
  circuit();

  // Get a gate outputing a constant, return its name
  string const &constant(bits val);

  // Find the gate driving a wire
  gate &find(string const &wire);

  // Add a new gate with output to the named wire
  void add(string const &wire, gate const &g) { gates.emplace(wire, g); }
  // Replace a gate with something new
  void rewire(string const &wire, gate const &g) { find(wire) = g; }

  // Evaluate the wire with given name
  bits eval(string const &wire) { return find(wire).eval(*this); }

  // Reset all gates
  void reset();
};

bits gate::eval(circuit &cir) {
  if (output)
    return *output;
  assert(logic);
  auto [op, in1, in2] = *logic;
  bits v1 = cir.eval(in1);
  bits v2 = cir.eval(in2);
  output = op(v1, v2);
  return *output;
}

circuit::circuit() {
  string line;
  while (getline(cin, line)) {
    vector<string> tokens;
    string token;
    stringstream ss(line);
    while (ss >> token)
      tokens.push_back(token);
    assert(tokens.size() >= 3 && tokens.size() <= 5);
    auto maybe_const = [&](string const &s) {
      if (s.find_first_not_of("0123456789") != string::npos)
        return s;
      return constant(stoul(s));
    };
    assert(tokens[tokens.size() - 2] == "->");
    string const &wire = tokens.back();
    if (tokens.size() == 3)
      add(wire, maybe_const(tokens[0]));
    else if (tokens.size() == 4) {
      assert(tokens[0] == "NOT");
      add(wire, gate(not_op, tokens[1], tokens[1]));
    } else {
      string in1 = maybe_const(tokens[0]);
      string in2 = maybe_const(tokens[2]);
      auto p = ops.find(tokens[1]);
      assert(p != ops.end());
      add(wire, gate(p->second, in1, in2));
    }
  }
}

string const &circuit::constant(bits val) {
  string name = to_string(val);
  auto p = gates.find(name);
  if (p == gates.end())
    p = gates.emplace(name, gate(val)).first;
  return p->first;
}

gate &circuit::find(string const &wire) {
  auto p = gates.find(wire);
  assert(p != gates.end());
  return p->second;
}

void circuit::reset() {
  for (auto &p : gates)
    p.second.reset();
}

void part1() { cout << circuit().eval("a") << '\n'; }

void part2() {
  circuit c;
  c.rewire("b", gate(c.eval("a")));
  c.reset();
  cout << c.eval("a") << '\n';
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
