// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <array>
#include <optional>
#include <cassert>

using namespace std;

// Inputs for game
int boss_initial_hp;
int boss_damage;
bool hard_mode;

enum { magic_missile = 0, drain, shield, poison, recharge, nspells };

int mana_costs[nspells] = {53, 73, 113, 173, 229};
int effect_durations[nspells] = {0, 0, 6, 6, 5};

// Search state
struct state {
  int player_hp{50};
  int player_armor{0};
  int boss_hp{boss_initial_hp};
  int mana{500};
  int mana_spent{0};
  array<int, nspells> effects{0, 0, 0, 0, 0};

  // Can the indicated spell be cast (enough mana, not already in
  // effect)
  bool castable(int spell) const;

  // Apply effects before player or boss turn
  void do_effects();
  // Do one turn for the player (casting the given spell) followed by
  // one turn for the boss
  void advance(int spell);

  // Is this state a win (did the boss die during the last player
  // turn)?
  bool win() const { return boss_hp <= 0; }
  // Is this state a loss (did the player die, or, in hard mode, is
  // the player going to die before getting a chance to do anything)?
  bool loss() const { return player_hp <= hard_mode ? 1 : 0; }
};

bool state::castable(int spell) const {
  // <= 1 since do_effects will decrement the remaining duration, and
  // it's allowable to cast an effect spell on the same turn that the
  // effect expires
  return mana >= mana_costs[spell] &&
         (effect_durations[spell] == 0 || effects[spell] <= 1);
}

void state::do_effects() {
  if (effects[shield] > 0)
    --effects[shield];
  else
    player_armor = 0;
  if (effects[poison] > 0) {
    boss_hp -= 3;
    --effects[poison];
  }
  if (effects[recharge] > 0) {
    mana += 101;
    --effects[recharge];
  }
}

void state::advance(int spell) {
  // Initially player's turn, going to cast the given spell
  if (hard_mode) {
    --player_hp;
    // Should have already detected a loss before generating successor
    // states
    assert(player_hp > 0);
  }
  do_effects();
  assert(spell < nspells);
  mana -= mana_costs[spell];
  assert(mana >= 0);
  mana_spent += mana_costs[spell];
  // Start effect duration counters
  if (effect_durations[spell] != 0) {
    assert(effects[spell] == 0);
    effects[spell] = effect_durations[spell];
  }
  // Do any immediate actions
  switch (spell) {
  case magic_missile:
    boss_hp -= 4;
    break;
  case drain:
    player_hp += 2;
    boss_hp -= 2;
    break;
  case shield:
    player_armor = 7;
    break;
  default:
    break;
  }
  if (boss_hp <= 0)
    // Boss died, game ends
    return;
  do_effects();
  // Boss turn
  int damage = max(boss_damage - player_armor, 1);
  player_hp -= damage;
}

int operator==(state const &s1, state const &s2) {
  return s1.player_hp == s2.player_hp && s1.boss_hp == s2.boss_hp &&
         s1.mana == s2.mana && s1.mana_spent == s2.mana_spent &&
         s1.effects == s2.effects;
}

int comp(state const &s1, state const &s2) {
  // < 0 for more preferred states for search
  auto then = [](int i1, int i2) { return i1 != 0 ? i1 : i2; };
  int mana_spent = s1.mana_spent - s2.mana_spent;
  int player_hp = s2.player_hp - s1.player_hp;
  int boss_hp = s1.boss_hp - s2.boss_hp;
  int mana = s2.mana - s1.mana;
  int effects = 0;
  for (int i = 0; i < nspells; ++i)
    effects = then(effects, s2.effects[i] - s1.effects[i]);
  return then(mana_spent, then(player_hp, then(boss_hp, then(mana, effects))));
}

// > 0 since the priority queue is a max queue
bool operator<(state const &s1, state const &s2) { return comp(s1, s2) > 0; }

void solve(bool hard) {
  string _;
  cin >> _ >> _ >> boss_initial_hp;
  cin >> _ >> boss_damage;
  hard_mode = hard;
  set<state> visited;
  priority_queue<state> q;
  auto visit = [&](state const &s) {
    if (visited.count(s))
      return;
    visited.insert(s);
    q.push(s);
  };
  visit(state());
  optional<int> min_mana_spent;
  while (!q.empty()) {
    state s = q.top();
    q.pop();
    if (s.win()) {
      min_mana_spent = s.mana_spent;
      break;
    }
    if (s.loss())
      continue;
    for (int spell = magic_missile; spell <= recharge; ++spell) {
      if (s.castable(spell)) {
        state next(s);
        next.advance(spell);
        visit(next);
      }
    }
  }
  assert(min_mana_spent);
  cout << *min_mana_spent << '\n';
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
