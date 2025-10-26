#include <bits/stdc++.h>
using namespace std;

#include <celony/math/modint.hpp>

/**
 * @brief Computes information necessary for combinatoric calculations in `O(1)`
 * with `O(N)` precomputation. This assumes the modulus is prime.
 *
 * @tparam M The modulus.
 */
template <int M> struct combinatorics {
  vector<modint<M>> fact, ifact, inv;
  combinatorics(int n) : fact(n + 1, 1), ifact(n + 1, 1), inv(n + 1, 1) {
    for (int i = 2; i <= n; i++) {
      fact[i] = fact[i - 1] * i;
      inv[i] = inv[M % i] * (M - M / i);
      ifact[i] = ifact[i - 1] * inv[i];
    }
  }

  /**
   * @brief Compute `C(n, k)`.
   *
   * @param n Total number of objects.
   * @param k Number of objects to choose.
   */
  modint<M> ncr(int n, int k) const {
    return fact[n] * ifact[k] * ifact[n - k];
  }

  /**
   * @brief Compute `P(n, k)`.
   *
   * @param n Total number of objects.
   * @param k Number of objects to choose.
   */
  modint<M> npr(int n, int k) const { return fact[n] * ifact[n - k]; }
};