#include <bits/stdc++.h>
using namespace std;

/**
 * @brief Union Find that automatically calls merge functor.
 *
 * Uses true `O(N)` memory but uses size rather than rank, but size is much more
 * useful information in practice and still runs very fast.
 *
 * @tparam F Merge functor.
 */
template <typename F> class union_find {
  int comps = 0;
  vector<int> v; // size for parent and -parent otherwise
  F f;

public:
  union_find(int n, const F &f) : comps(n), v(n, 1), f(f) {}

  /**
   * @brief Find the leader of component.
   *
   * @param i The component.
   */
  int find(int i) { return v[i] > 0 ? i : -(v[i] = -find(-v[i])); }

  /**
   * @brief Get the size of `i`'s component.
   *
   * @param i The component.
   */
  int size(int i) { return v[find(i)]; }

  /**
   * @brief Get the number of components.
   */
  int components() const { return comps; }

  /**
   * @brief Unites two components if they are different.
   *
   * @param i First component.
   * @param j Second component.
   */
  bool unite(int i, int j) {
    i = find(i), j = find(j);
    if (i == j)
      return false;
    comps--;
    if (v[i] < v[j])
      swap(i, j);
    f(i, j);
    v[i] += v[j];
    v[j] = -i;
    return true;
  }
};