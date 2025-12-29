// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <variant>
#include <cctype>
#include <cassert>

using namespace std;

struct json: variant<int, string, vector<json>, map<string, json>> {
  json(int i) : variant(i) {}
  json(string const &s) : variant(s) {}
  json(vector<json> const &array) : variant(array) {}
  json(map<string, json> const &dict) : variant(dict) {}
};

json read() {
  if (isdigit(cin.peek()) || cin.peek() == '-') {
    int i;
    cin >> i;
    return json(i);
  }
  if (cin.peek() == '"') {
    cin.ignore(1);
    string s;
    char c;
    while (cin >> c, c != '"')
      s.push_back(c);
    return json(s);
  }
  if (cin.peek() == '[') {
    cin.ignore(1);
    auto result = json(vector<json>());
    auto &array = get<vector<json>>(result);
    while (cin.peek() != ']') {
      array.push_back(read());
      if (cin.peek() == ',')
        cin.ignore(1);
    }
    cin.ignore(1);
    return result;
  }
  assert(cin.peek() == '{');
  cin.ignore(1);
  auto result = json(map<string, json>());
  auto &dict = get<map<string, json>>(result);
  while (cin.peek() != '}') {
    string key = get<string>(read());
    assert(cin.peek() == ':');
    cin.ignore(1);
    json value = read();
    dict.emplace(key, value);
    if (cin.peek() == ',')
      cin.ignore(1);
  }
  cin.ignore(1);
  return result;
}

int add_up(json const &obj, bool ignore_red) {
  switch (obj.index()) {
  case 0:
    return get<int>(obj);
  case 1:
    return 0;
  case 2: {
    int total = 0;
    for (auto const &child : get<vector<json>>(obj))
      total += add_up(child, ignore_red);
    return total;
  }
  default: {
    int total = 0;
    bool saw_red = false;
    for (auto const &kv : get<map<string, json>>(obj)) {
      total += add_up(kv.second, ignore_red);
      if (kv.second.index() == 1 && get<string>(kv.second) == "red")
        saw_red = true;
    }
    return ignore_red && saw_red ? 0 : total;
  }
  }
}

void part1() { cout << add_up(read(), false) << '\n'; }
void part2() { cout << add_up(read(), true) << '\n'; }

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
