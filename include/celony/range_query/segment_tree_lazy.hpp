#include <bits/stdc++.h>
using namespace std;

/**
 * @brief Segment Tree for efficient range updates and queries.
 *
 * The Segment Tree is implemented iteratively.
 *
 * @tparam T Value type.
 * @tparam U Update type.
 * @tparam Apply Applies accumulated updates to value.
 * @tparam Push Accumulates updates with new update.
 * @tparam Combine Binary callable.
 */
template <typename T, typename U, typename Apply, typename Push,
          typename Combine>
class segment_tree_lazy {
  vector<T> t;
  vector<U> u;
  vector<bool> f; // avoid using optional for updates
  Apply apply;
  Push push;
  Combine combine;
  int n0;

  void _apply(int i, const U &v, int k) {
    int n = u.size();
    t[i] = apply(t[i], v, k);
    if (i < n) {
      u[i] = push(u[i], v, k);
      f[i] = true;
    }
  }
  void _push(int p) {
    int n = u.size();
    int h = __lg(n) + 1;
    int k = 1 << (h - 1);
    for (p += n; h > 0; h--, k >>= 1) {
      int i = p >> h;
      if (f[i]) {
        _apply(i << 1, u[i], k);
        _apply(i << 1 | 1, u[i], k);
        u[i] = u[0];
        f[i] = false;
      }
    }
  }
  void _compute(int i, int k) {
    t[i] = f[i] ? apply(t[i], u[i], k) : combine(t[i << 1], t[i << 1 | 1]);
  }

public:
  segment_tree_lazy(const vector<T> &v, const T &v0, const U &u0,
                    const Apply &apply, const Push &push,
                    const Combine &combine)
      : apply(apply), push(push), combine(combine) {
    n0 = v.size();
    int n2 = 1 << (__lg(n0) + int((n0 & (n0 - 1)) > 0)); // lowest pow2 >= n
    t.resize(2 * n2, v0);
    u.resize(n2, u0);
    f.resize(n2);
    for (int i = 0; i < n0; i++)
      t[i + n2] = v[i];
    for (int i = n2 - 1; i > 0; --i)
      t[i] = combine(t[i << 1], t[i << 1 | 1]);
  }
  segment_tree_lazy(int n, const T &v, const U &u0, const Apply &apply,
                    const Push &push, const Combine &combine)
      : segment_tree_lazy(vector<T>(n, v), v, u0, apply, push, combine) {}

  /**
   * @brief Returns the default value for queries.
   */
  T identity() const { return t[0]; }

  /**
   * @brief Update the range `[l, r)`.
   *
   * @param l Left bound.
   * @param r Right bound.
   * @param v Value to update with.
   */
  void update(int l, int r, const U &v) {
    _push(l);
    _push(r - 1);
    int k = 1, n = u.size(), cl = 0, cr = 0;
    for (l += n, r += n; l < r; l >>= 1, r >>= 1, k <<= 1) {
      if (cl)
        _compute(l - 1, k);
      if (cr)
        _compute(r, k);
      if (l & 1) {
        _apply(l++, v, k);
        cl = true;
      }
      if (r & 1) {
        _apply(--r, v, k);
        cr = true;
      }
    }
    for (--l; r > 0; l >>= 1, r >>= 1, k <<= 1) {
      if (cl)
        _compute(l, k);
      if (cr && (!cl || l != r))
        _compute(r, k);
    }
  }

  /**
   * @brief Query the range `[l, r)`.
   *
   * @param l Left bound.
   * @param r Right bound.
   */
  T query(int l, int r) {
    _push(l);
    _push(r - 1);
    int n = u.size();
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
    int n = u.size();
    int i = l + n, d = 1;
    _push(l);
    T ans0 = t[0];
    for (T ans;
         l < n && ((i & 1 ^ 1) ||
                   std::invoke(std::forward<F>(f), ans = combine(ans0, t[i])));
         d <<= 1, i = (i >> 1) + (i & 1))
      if (i & 1)
        ans0 = std::move(ans), l += d;
    for (; l < n && i < 2 * n; d >>= 1) {
      _push(l);
      if (auto ans = combine(ans0, t[exchange(i, i << 1)]); f(ans))
        ans0 = std::move(ans), i += 2, l += d;
    }
    return min(l, n0);
  }

  /**
   * @brief Find minimum `l<=r` s.t. `f(a[l+1..r])` remains true.
   *
   * @param r Right bound.
   * @param f Predicate callable.
   */
  template <typename F> int min_l(int r, F &&f) {
    int n = u.size();
    int i = r + n, d = 1;
    _push(r);
    T ans0 = t[0];
    for (T ans; r >= 0 && i > 1 &&
                ((i & 1) ||
                 std::invoke(std::forward<F>(f), ans = combine(ans0, t[i])));
         d <<= 1, i = (i - 1) >> 1)
      if (i & 1 ^ 1)
        ans0 = std::move(ans), r -= d;
    for (; r >= 0 && i < 2 * n; r -= d, d >>= 1) {
      _push(r);
      if (auto ans =
              exchange(ans0, combine(ans0, t[exchange(i, (i << 1) - 1)]));
          !f(ans0))
        ans0 = std::move(ans), i += 2, r += d;
    }
    return r;
  }
};
