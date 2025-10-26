#include <bits/stdc++.h>
using namespace std;

#include <celony/range_query/segment_tree.hpp>

/**
 * @brief Answers non-trivial path queries of a tree in `O(log^2 N)`.
 *
 * @tparam T Value type.
 * @tparam Combine Binary callable.
 */
template <typename T, typename Combine> class heavy_light_decomposition {
  vector<int> top, depth, id, par;
  segment_tree<T, Combine> seg;
  Combine combine;
  void _init(const vector<vector<int>> &g, int root) {}

public:
  heavy_light_decomposition(const vector<vector<int>> &g, const T &v,
                            const Combine &combine, int root = 0)
      : heavy_light_decomposition(g, vector<T>(g.size(), v), v, combine, root) {
  }

  heavy_light_decomposition(const vector<vector<int>> &g, const vector<T> &v,
                            const T &v0, const Combine &combine, int root = 0)
      : seg(g.size(), v0, combine), combine(combine) {
    int n = g.size();
    vector<int> size(n, 1);
    top.resize(n);
    depth.resize(n);
    id.resize(n);
    par.resize(n);
    auto dfs = [&](auto &&self, int u, int p) -> int {
      par[u] = p;
      depth[u] = depth[p] + 1;
      for (int v : g[u])
        if (v != p)
          size[u] += self(self, v, u);
      return size[u];
    };
    dfs(dfs, root, root);
    auto dfs2 = [&, time = 0](auto &&self, int u, int p,
                              int t) mutable -> void {
      top[u] = t;
      id[u] = time++;
      int hvy_v = -1, hvy_s = -1;
      for (int v : g[u])
        if (v != p && size[v] > hvy_s) {
          hvy_v = v;
          hvy_s = size[v];
        }
      if (hvy_v == -1)
        return;
      self(self, hvy_v, u, t);
      for (int v : g[u])
        if (v != p && v != hvy_v)
          self(self, v, u, v);
    };
    dfs2(dfs2, root, root, root);
    for (int i = 0; i < n; i++)
      seg.set(id[i], v[i]);
  }

  /**
   * @brief Update the value at node.
   *
   * @param a The node.
   * @param v The value.
   */
  void update(int a, const T &v) { seg.update(id[a], v); }

  /**
   * @brief Set the value at node.
   *
   * @param a The node.
   * @param v The value.
   */
  void set(int a, const T &v) { seg.set(id[a], v); }

  /**
   * @brief Queries the path from `a` to `b`.
   */
  T query(int a, int b) {
    T ans = seg.identity();
    for (; top[a] != top[b]; a = par[top[a]]) {
      if (depth[top[a]] < depth[top[b]])
        swap(a, b);
      auto res = seg.query(id[top[a]], id[a] + 1);
      ans = combine(ans, res);
    }
    if (depth[a] > depth[b])
      swap(a, b);
    auto res = seg.query(id[a], id[b] + 1);
    ans = combine(ans, res);
    return ans;
  }
};
