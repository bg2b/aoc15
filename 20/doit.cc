// -*- C++ -*-
// g++ -std=c++17 -Wall -g -o doit doit.cc
// ./doit 1 < input  # part 1
// ./doit 2 < input  # part 2

#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

using num = unsigned;

vector<num> odd_primes{3};

void more_primes() {
  for (num next_p = odd_primes.back() + 2;; next_p += 2) {
    bool is_prime = true;
    for (num p : odd_primes) {
      if (p * p > next_p)
        break;
      if (next_p % p == 0) {
        is_prime = false;
        break;
      }
    }
    if (is_prime) {
      odd_primes.push_back(next_p);
      break;
    }
  }
}

num sum_of_divisors(num n) {
  num pow2 = 0;
  while ((n & 1) == 0) {
    ++pow2;
    n >>= 1;
  }
  while (odd_primes.back() * odd_primes.back() < n)
    more_primes();
  num sum_div = (1 << (pow2 + 1)) - 1;
  for (num p : odd_primes) {
    if (p * p > n)
      // No more divisors
      break;
    num pm = 1;
    num sum_pm = 1;
    while (n % p == 0) {
      n /= p;
      pm *= p;
      sum_pm += pm;
    }
    sum_div *= sum_pm;
  }
  if (n > 1)
    // Whatever's left is a prime
    sum_div *= n + 1;
  return sum_div;
}

void part1() {
  num min_presents;
  cin >> min_presents;
  num const elf_scaling = 10;
  num min_sum_div = (min_presents + elf_scaling - 1) / elf_scaling;
  num ans;
  for (ans = 1; ans <= 630 && sum_of_divisors(ans) < min_sum_div; ++ans)
    ;
  if (ans > 630)
    // Beyond 630, all candidate highly abundant numbers (i.e., ones
    // that can have a larger sum of divisors than any previous
    // number, i.e., corresponding to houses that could have received
    // more presents than any previous house) are divisible by 4
    for (ans = 632; sum_of_divisors(ans) < min_sum_div; ans += 4)
      ;
  cout << ans << '\n';
}

void part2() {
  num min_presents;
  cin >> min_presents;
  num const elf_scaling = 11;
  num min_sum_div = (min_presents + elf_scaling - 1) / elf_scaling;
  num ans;
  // It's not 100% clear that the highly-abundant 630 observation is
  // applicable when lazy elves are taken into account
  for (ans = 1;; ++ans) {
    num sum_div = sum_of_divisors(ans);
    if (sum_div < min_sum_div)
      // Not enough even without laziness
      continue;
    // Subtract presents that were not received due to lazy elves...
    num presents = sum_div * 11;
    for (num lazy_elf = 1; 50 * lazy_elf < ans; ++lazy_elf)
      if (ans % lazy_elf == 0)
        presents -= lazy_elf * elf_scaling;
    if (presents >= min_presents)
      break;
  }
  cout << ans << '\n';
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
