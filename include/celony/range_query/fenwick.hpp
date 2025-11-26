#include <bits/stdc++.h>
using namespace std;

/**
 * @brief Binary Indexed Tree (Fenwick Tree) for prefix queries.
 *
 * Supports efficient point updates and prefix queries for associative operations.
 * Much faster in practice than segment trees due to simpler operations and
 * better cache locality. However, can only query prefixes, not arbitrary ranges.
 *
 * @tparam T Value type.
 * @tparam Combine Binary associative operation (e.g., addition, multiplication).
 *
 * @note Cannot directly set elements; only update via the combine function.
 * @note For range queries, compute query(r) - query(l-1).
 * @note Works best with invertible operations (addition, XOR, etc.).
 */
template <typename T, typename Combine> class fenwick {
  vector<T> t;
  Combine combine;

public:
  /**
   * @brief Constructs a Fenwick Tree from an array.
   *
   * Time Complexity: O(N)
   *
   * @param v Initial array values.
   */
  fenwick(const vector<T> &v, const Combine &combine) : t(v.size() + 1) {
    int n = v.size();
    for (int i = 0; i < n; i++)
      t[i + 1] = v[i];
    for (int i = 1; i <= n; i++) {
      int j = i + (i & -i);
      if (j <= n)
        t[j] = combine(t[j], t[i]);
    }
  }
  fenwick(int n, T v, const Combine &combine)
      : fenwick(vector<T>(n, v), combine) {}

  /**
   * @brief Updates the element at index i using the combine operation.
   *
   * Time Complexity: O(log N)
   *
   * @param i One-indexed index.
   * @param v Value to combine with the current element.
   */
  void update(int i, T v) {
    for (; i < t.size(); i += i & -i)
      t[i] = combine(t[i], v);
  }

  /**
   * @brief Queries the prefix sum for elements [0, i-1].
   *
   * Time Complexity: O(log N)
   *
   * @param i One-indexed index (exclusive upper bound).
   * @return The combined result of elements [0, i-1].
   */
  T query(int i) const {
    T ans = t[0];
    for (; i > 0; i -= i & -i)
      ans = combine(ans, t[i]);
    return ans;
  }
};