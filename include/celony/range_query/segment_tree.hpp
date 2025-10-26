#include <bits/stdc++.h>
using namespace std;

/**
 * @brief Segment Tree for efficient point update and range queries.
 *
 * The Segment Tree is implemented iteratively.
 *
 * @tparam T Value type.
 * @tparam Combine Binary callable.
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
   * @param v The array.
   * @param v0 Default value for queries.
   * @param combine Binary callable.
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
   */
  T identity() const { return t[0]; }

  /**
   * @brief Update the value at the index.
   *
   * @param i The index.
   * @param v Value to update with.
   */
  void update(int i, const T &v) {
    int n = t.size() >> 1;
    for (t[i += n] = combine(t[i + n], v); i >>= 1;)
      t[i] = combine(t[i << 1], t[i << 1 | 1]);
  }

  /**
   * @brief Sets the value at the index.
   *
   * @param i The index.
   * @param v Value to set.
   */
  void set(int i, const T &v) {
    int n = t.size() >> 1;
    for (t[i += n] = v; i >>= 1;)
      t[i] = combine(t[i << 1], t[i << 1 | 1]);
  }

  /**
   * @brief Queries the range `[l, r)`.
   *
   * @param l Left bound.
   * @param r Right bound.
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
   * @brief Find maximum `r>=l` s.t. `f(a[l..r-1])` remains true.
   *
   * @param l Left bound.
   * @param f Predicate callable.
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
   * @brief Find minimum `l<=r` s.t. `f(a[l+1..r])` remains true.
   *
   * @param r Right bound.
   * @param f Predicate callable.
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
