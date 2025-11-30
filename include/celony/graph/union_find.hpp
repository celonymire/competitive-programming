#include <bits/stdc++.h>
using namespace std;

/**
 * @brief Disjoint Set Union (DSU) / Union-Find data structure with custom merge
 * callback.
 *
 * Supports efficient union and find operations with path compression.
 * Uses union-by-size for optimization and provides a callback mechanism
 * to execute custom logic when merging components.
 *
 * @tparam F Merge functor type with signature `void(int parent, int child)`.
 *           Called when merging components, where parent is the leader that
 *           absorbs the child component.
 *
 * @note \f$\alpha(N)\f$ is the inverse Ackermann function, effectively constant
 * (< 5) for all practical values of N.
 */
template <typename F> class union_find {
  int comps = 0;
  vector<int> v; // size for parent and -parent otherwise
  F f;

public:
  /**
   * @brief Constructs a Union-Find structure with n elements.
   *
   * @param n Number of elements (0 to n-1).
   * @param f Merge callback function with signature void(int parent, int
   * child).
   */
  union_find(int n, const F &f) : comps(n), v(n, 1), f(f) {}

  /**
   * @brief Finds the leader of the component containing element i.
   *
   * Time Complexity: \f$O(\alpha(N))\f$ amortized
   *
   * @param i Element index.
   * @return The leader (representative) of the component.
   */
  int find(int i) { return v[i] > 0 ? i : -(v[i] = -find(-v[i])); }

  /**
   * @brief Returns the size of the component containing element i.
   *
   * Time Complexity: \f$O(\alpha(N))\f$ amortized
   *
   * @param i Element index.
   * @return The number of elements in the component.
   */
  int size(int i) { return v[find(i)]; }

  /**
   * @brief Returns the current number of disjoint components.
   *
   * Time Complexity: \f$O(1)\f$
   * @return The number of connected components.
   */
  int components() const { return comps; }

  /**
   * @brief Unites the components containing elements i and j.
   *
   * Time Complexity: \f$O(\alpha(N))\f$ amortized
   *
   * @param i First element index.
   * @param j Second element index.
   * @return True if the elements were in different components and were merged,
   * false otherwise.
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