// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include <cassert>

using namespace std;

struct item {
  string name;
  int cost;
  int damage;
  int armor;

  item(string const &name_, int cost_, int damage_, int armor_) :
    name(name_), cost(cost_), damage(damage_), armor(armor_) {}
};

vector<item> weapons
  { 
   { "Dagger",        8,     4,       0 },
   { "Shortsword",   10,     5,       0 },
   { "Warhammer",    25,     6,       0 },
   { "Longsword",    40,     7,       0 },
   { "Greataxe",     74,     8,       0 },
  };

vector<item> suits
  { 
   { "No armor",      0,     0,       0 },
   { "Leather",      13,     0,       1 },
   { "Chainmail",    31,     0,       2 },
   { "Splintmail",   53,     0,       3 },
   { "Bandedmail",   75,     0,       4 },
   { "Platemail",   102,     0,       5 },
  };

vector<item> rings
  {
   { "No left",       0,     0,       0 },
   { "No right",      0,     0,       0 },
   { "Damage +1",    25,     1,       0 },
   { "Damage +2",    50,     2,       0 },
   { "Damage +3",   100,     3,       0 },
   { "Defense +1",   20,     0,       1 },
   { "Defense +2",   40,     0,       2 },
   { "Defense +3",   80,     0,       3 },
  };   

int boss_hp;
int boss_damage;
int boss_armor;

int turns_to_win(int damage, int opp_armor, int opp_hp) {
  int dmg_per_turn = max(damage - opp_armor, 1);
  return (opp_hp + dmg_per_turn - 1) / dmg_per_turn;
}

bool player_wins(int damage, int armor) {
  int player_turns_to_win = turns_to_win(damage, boss_armor, boss_hp);
  int boss_turns_to_win = turns_to_win(boss_damage, armor, 100);
  return player_turns_to_win <= boss_turns_to_win;
}

void solve(bool player_to_win) {
  string _;
  cin >> _ >> _ >> boss_hp;
  cin >> _ >> boss_damage;
  cin >> _ >> boss_armor;
  optional<int> best_cost;
  for (item const &weapon : weapons)
    for (item const &suit : suits)
      for (size_t lhand = 0; lhand < rings.size(); ++lhand)
        for (size_t rhand = lhand + 1; rhand < rings.size(); ++rhand) {
          item const &left = rings[lhand];
          item const &right = rings[rhand];
          int damage = weapon.damage + left.damage + right.damage;
          int armor = suit.armor + left.armor + right.armor;
          if (player_wins(damage, armor) == player_to_win) {
            int cost = weapon.cost + suit.cost + left.cost + right.cost;
            if (player_to_win)
              best_cost = min(best_cost.value_or(cost), cost);
            else
              best_cost = max(best_cost.value_or(cost), cost);
          }
        }
  assert(best_cost);
  cout << *best_cost << '\n';
}

void part1() { solve(true); }
void part2() { solve(false); }

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
