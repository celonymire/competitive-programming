#include <bits/stdc++.h>
using namespace std;

/**
 * @brief Linear Sieve of Eratosthenes for prime factorization.
 *
 * Efficiently computes all primes up to N and stores the smallest prime
 * divisor (SPD) for each number. The linear time complexity is achieved
 * by ensuring each composite number is marked exactly once by its SPD.
 *
 * Time Complexity:
 * - Construction: \f$O(N)\f$
 * - is_prime(): \f$O(1)\f$
 * - divisors(): \f$O(d(N))\f$ where d(N) is the number of divisors
 * Space Complexity: \f$O(N)\f$
 *
 * @note This is optimal for when you need both primes and factorization.
 * @note The SPD array enables efficient prime factorization.
 */
struct sieve {
  vector<int> primes, spd;

  sieve(int n) : spd(n + 1) {
    iota(spd.begin(), spd.end(), 0);
    for (int i = 2; i <= n; i++) {
      if (spd[i] == i)
        primes.push_back(i);
      for (int j = 0; j < (int)primes.size() && i * primes[j] <= n; j++) {
        spd[i * primes[j]] = primes[j];
        if (i % primes[j] == 0)
          break;
      }
    }
  }

  /**
   * @brief Returns if a number is a prime.
   *
   * @param x The number.
   */
  bool is_prime(int x) const { return x > 1 && spd[x] == x; }

  /**
   * @brief Returns a list of divisors of a number.
   *
   * @param x The number.
   */
  vector<int> divisors(int x) {
    vector<int> ans;
    while (x > 1)
      for (int d = spd[x], sd = spd[x], n = ans.size(); spd[x] == sd;
           d *= sd, x /= sd) {
        for (int i = 0; i < n; i++)
          ans.push_back(d * ans[i]);
        ans.push_back(d);
      }
    return ans;
  }
};