#include <bits/stdc++.h>
using namespace std;

/**
 * @brief Sparse Table for O(1) range queries on immutable arrays.
 *
 * Precomputes answers for all ranges of length 2^k, enabling constant-time
 * queries for idempotent operations. An operation is idempotent if
 * f(x, x) = x, which allows overlapping ranges in queries.
 *
 * @tparam T Value type.
 * @tparam Combine Binary idempotent operation (e.g., min, max, gcd, lcm).
 *
 * @note The array is immutable after construction.
 * @note Combine must be idempotent: f(x, x) = x.
 * @note Perfect for static RMQ (Range Minimum Query) problems.
 * @note More space-efficient than segment trees for read-only scenarios.
 */
template <typename T, typename Combine> class sparse_table {
  vector<vector<T>> dp;
  Combine combine;

public:
  /**
   * @brief Constructs a sparse table from an array.
   *
   * Time Complexity: O(N log N)
   * Space Complexity: O(N log N)
   *
   * @param v The array.
   */
  sparse_table(const vector<T> &v, const Combine &combine) : combine(combine) {
    int n = v.size();
    dp.resize(__lg(n) + 1, vector<T>(n));
    dp[0] = v;
    for (int i = 1; i <= __lg(v.size()); i++)
      for (int j = 0; j + (1 << i) <= v.size(); j++)
        dp[i][j] = combine(dp[i - 1][j], dp[i - 1][j + (1 << (i - 1))]);
  }

  /**
   * @brief Queries the range [l, r] using the idempotent combine operation.
   *
   * Time Complexity: O(1)
   *
   * @param l Left bound (inclusive).
   * @param r Right bound (inclusive).
   * @return The combined result of elements [l, r].
   */
  T query(int l, int r) const {
    int i = __lg(r - l + 1);
    return combine(dp[i][l], dp[i][r - (1 << i) + 1]);
  }
};
