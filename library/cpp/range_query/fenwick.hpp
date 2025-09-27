#include <bits/stdc++.h>
using namespace std;

/**
 * @brief Fenwick Tree that answers prefix queries in `O(log N)` with very low
 * constant factor.
 *
 * The difference with Segment Tree is:
 * 1) Cannot directly set an element, instead update it through the combine
 * function.
 * 2) Very low constant factor for its operations.
 *
 * @tparam T Value type.
 * @tparam Combine Binary callable.
 */
template <typename T, typename Combine> class fenwick {
  vector<T> t;
  Combine combine;

public:
  fenwick(int n, T v, Combine combine) : t(n + 1, v), combine(combine) {}

  /**
   * @brief Update the element.
   *
   * @param i One-indexed index.
   * @param v The value.
   */
  void update(int i, T v) {
    for (; i < t.size(); i += i & -i)
      t[i] = combine(t[i], v);
  }

  /**
   * @brief Query the prefix `a[0..i-1]`.
   *
   * @param i One-indexed index.
   */
  T query(int i) const {
    T ans = t[0];
    for (; i > 0; i -= i & -i)
      ans = combine(ans, t[i]);
    return ans;
  }
};