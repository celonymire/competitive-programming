#include <bits/stdc++.h>
using namespace std;

/**
 * @brief Iterative Segment Tree for point updates and range queries.
 *
 * A versatile data structure supporting efficient range queries and point
 * updates for any associative operation. This iterative implementation is
 * faster and simpler than recursive versions, with excellent cache locality.
 *
 * @tparam T Value type.
 * @tparam Combine Binary associative operation (e.g., min, max, sum, gcd).
 *
 * @note Combine operation must be associative.
 * @note Supports efficient binary search on prefixes/suffixes.
 */
template <typename T, typename Combine> class segment_tree {
  vector<T> t;
  Combine combine;
  int n0;

public:
  segment_tree(int n, const T &v, const Combine &combine)
      : segment_tree(vector<T>(n, v), v, combine) {}

  /**
   * @brief Constructs a Segment Tree from the array.
   *
   * Time Complexity: \f$O(N)\f$
   *
   * @param v The array.
   * @param v0 Default value for queries.
   * @param combine Binary associative operation for combining range values.
   */
  segment_tree(const vector<T> &v, const T &v0, const Combine &combine)
      : combine(combine) {
    n0 = v.size();
    int n2 = 1 << (__lg(n0) + int((n0 & (n0 - 1)) > 0)); // lowest pow2 >= n
    t.resize(2 * n2, v0);
    for (int i = 0; i < n0; i++)
      t[i + n2] = v[i];
    for (int i = n2 - 1; i > 0; --i)
      t[i] = combine(t[i << 1], t[i << 1 | 1]);
  }

  /**
   * @brief Returns the default value for queries.
   *
   * Time Complexity: \f$O(1)\f$
   */
  T identity() const { return t[0]; }

  /**
   * @brief Updates the value at index i using the combine operation.
   *
   * Time Complexity: \f$O(\log N)\f$
   *
   * @param i Zero-indexed position.
   * @param v Value to combine with the current element.
   */
  void update(int i, const T &v) {
    int n = t.size() >> 1;
    for (t[i += n] = combine(t[i + n], v); i >>= 1;)
      t[i] = combine(t[i << 1], t[i << 1 | 1]);
  }

  /**
   * @brief Sets the value at index i, replacing the current value.
   *
   * Time Complexity: \f$O(\log N)\f$
   *
   * @param i Zero-indexed position.
   * @param v New value.
   */
  void set(int i, const T &v) {
    int n = t.size() >> 1;
    for (t[i += n] = v; i >>= 1;)
      t[i] = combine(t[i << 1], t[i << 1 | 1]);
  }

  /**
   * @brief Queries the range [l, r) using the combine operation.
   *
   * Time Complexity: \f$O(\log N)\f$
   *
   * @param l Left bound (inclusive).
   * @param r Right bound (exclusive).
   * @return The combined result of elements [l, r).
   */
  T query(int l, int r) const {
    int n = t.size() >> 1;
    T ansl = t[0], ansr = t[0];
    for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
      if (l & 1)
        ansl = combine(ansl, t[l++]);
      if (r & 1)
        ansr = combine(t[--r], ansr);
    }
    return combine(ansl, ansr);
  }

  /**
   * @brief Finds the maximum index r >= l such that f(query(l, r)) is true.
   *
   * Time Complexity: \f$O(\log N)\f$
   *
   * @param l Left bound (inclusive).
   * @param f Predicate function that takes a query result and returns bool.
   * @return Maximum valid index r, or n if no valid range exists.
   */
  template <typename F> int max_r(int l, F &&f) {
    int n = t.size() >> 1;
    int i = l + n, d = 1;
    T ans0 = t[0];
    for (T ans;
         l < n && ((i & 1 ^ 1) ||
                   std::invoke(std::forward<F>(f), ans = combine(ans0, t[i])));
         d <<= 1, i = (i >> 1) + (i & 1))
      if (i & 1)
        ans0 = std::move(ans), l += d;
    for (; l < n && i < 2 * n; d >>= 1)
      if (auto ans = combine(ans0, t[exchange(i, i << 1)]); f(ans))
        ans0 = std::move(ans), i += 2, l += d;
    return min(l, n0);
  }

  /**
   * @brief Finds the minimum index l <= r such that f(query(l+1, r+1)) is true.
   *
   * Time Complexity: \f$O(\log N)\f$
   *
   * @param r Right bound (inclusive).
   * @param f Predicate function that takes a query result and returns bool.
   * @return Minimum valid index l.
   */
  template <typename F> int min_l(int r, F &&f) {
    int n = t.size() >> 1;
    int i = r + n, d = 1;
    T ans0 = t[0];
    for (T ans; r >= 0 && i > 1 &&
                ((i & 1) ||
                 std::invoke(std::forward<F>(f), ans = combine(ans0, t[i])));
         d <<= 1, i = (i - 1) >> 1)
      if (i & 1 ^ 1)
        ans0 = std::move(ans), r -= d;
    for (; r >= 0 && i < 2 * n; r -= d, d >>= 1)
      if (auto ans =
              exchange(ans0, combine(ans0, t[exchange(i, (i << 1) - 1)]));
          !f(ans0))
        ans0 = std::move(ans), i += 2, r += d;
    return r;
  }
};
