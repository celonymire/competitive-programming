#include <bits/stdc++.h>
using namespace std;

/**
 * @brief Answers subarray queries in `O(1)` with `O(N log N)` preprocessing.
 *
 * Sparse Tables are immutable, but they can answer range queries very fast if
 * the answer remains correct when merging answers of two (possibly) overlapping
 * ranges. Some example operations are `gcd` and `min`.
 *
 * @tparam T Value type.
 * @tparam Combine Binary functor to merge two values.
 */
template <typename T, typename Combine> class sparse_table {
  vector<vector<T>> dp;
  Combine combine;

public:
  sparse_table(const vector<T> &v, const Combine &combine) : combine(combine) {
    int n = v.size();
    dp.resize(__lg(n) + 1, vector<T>(n));
    dp[0] = v;
    for (int i = 1; i <= __lg(v.size()); i++)
      for (int j = 0; j + (1 << i) <= v.size(); j++)
        dp[i][j] = combine(dp[i - 1][j], dp[i - 1][j + (1 << (i - 1))]);
  }

  /**
   * @brief Queries the range `[l, r]` in `O(1)`.
   *
   * @param l Left bound.
   * @param r Right bound.
   */
  T query(int l, int r) const {
    int i = __lg(r - l + 1);
    return combine(dp[i][l], dp[i][r - (1 << i) + 1]);
  }
};
