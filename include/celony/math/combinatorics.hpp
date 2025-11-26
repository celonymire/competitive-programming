#include <bits/stdc++.h>
using namespace std;

#include <celony/math/modint.hpp>

/**
 * @brief Precomputes factorials and inverses for fast combinatorial calculations.
 *
 * Enables \f$O(1)\f$ computation of binomial coefficients and permutations using
 * precomputed factorials, inverse factorials, and modular inverses. Uses
 * Fermat's little theorem for computing modular inverses, requiring prime modulus.
 *
 * Time Complexity:
 * - Construction: \f$O(N)\f$
 * - ncr(), npr(): \f$O(1)\f$
 * Space Complexity: \f$O(N)\f$
 *
 * @tparam M The modulus (must be prime).
 *
 * @note Modulus must be prime for Fermat's little theorem to work.
 * @note Maximum n must be known at construction time.
 */
template <int M> struct combinatorics {
  vector<modint<M>> fact, ifact, inv;
  /**
   * @brief Precomputes factorials and inverses up to n.
   *
   * @param n Maximum value for which to precompute (inclusive).
   */
  combinatorics(int n) : fact(n + 1, 1), ifact(n + 1, 1), inv(n + 1, 1) {
    for (int i = 2; i <= n; i++) {
      fact[i] = fact[i - 1] * i;
      inv[i] = inv[M % i] * (M - M / i);
      ifact[i] = ifact[i - 1] * inv[i];
    }
  }

  /**
   * @brief Computes the binomial coefficient C(n, k) = n! / (k! * (n-k)!).
   *
   * Time Complexity: \f$O(1)\f$
   *
   * @param n Total number of objects.
   * @param k Number of objects to choose.
   * @return The binomial coefficient C(n, k) modulo M.
   */
  modint<M> ncr(int n, int k) const {
    return fact[n] * ifact[k] * ifact[n - k];
  }

  /**
   * @brief Computes the permutation P(n, k) = n! / (n-k)!.
   *
   * Time Complexity: \f$O(1)\f$
   *
   * @param n Total number of objects.
   * @param k Number of objects to arrange.
   * @return The permutation P(n, k) modulo M.
   */
  modint<M> npr(int n, int k) const { return fact[n] * ifact[n - k]; }
};