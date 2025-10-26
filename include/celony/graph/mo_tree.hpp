#include <bits/stdc++.h>
using namespace std;

#include <celony/graph/lowest_common_ancestor.hpp>

/**
 * @brief Mo's Algorithm to answer offline path queries in `O(sqrt(N))`.
 *
 * The actual performance depends on the heuristic used to determine the order
 * of the queries. The best all-around heuristic used is Hilbert Curve, but this
 * implementation allows passing arbitrary heuristic function.
 *
 * Learn more about Hilbert Curve at https://codeforces.com/blog/entry/61203.
 *
 * @tparam R Query answer type.
 * @tparam Args Additional argument types to pass to the answer function.
 */
template <typename R, typename... Args> class mo_tree {
  vector<pair<int, int>> q;
  vector<tuple<Args...>> f_args;
  function<void(int)> add, erase;
  function<R(Args...)> f;

public:
  mo_tree(function<void(int)> add, function<void(int)> remove,
          function<R(Args...)> f)
      : add(std::move(add)), erase(std::move(erase)), f(std::move(f)) {}

  /**
   * @brief Adds a query of `f(path-from-a-to-b)` to solve.
   *
   * @param l Left node.
   * @param r right node.
   * @param args Additional arguments for the current query.
   */
  void add_query(int l, int r, const Args &...args) {
    q.push_back({l, r});
    f_args.push_back(args...);
  };

  /**
   * @brief Solves all the queries and return list of answers in the order of
   * the queries added.
   *
   * @param f The heuristic function with signature `Fn(queries) -> H` where
   * `Q[i]` will be answered before `Q[j]` if `H[i]<H[j]`.
   * @param lca The LCA object of the graph.
   */
  template <typename F>
  vector<R> solve(F &&f, const lowest_common_ancestor &lca) {
    int n = lca.depth.size(), m = q.size();
    vector<pair<int, int>> q2(m);
    for (int i = 0; i < m; i++) {
      auto [u, v] = q[i];
      if (lca.start[u] > lca.start[v])
        swap(u, v);
      q2[i] = {lca.query(u, v) == u ? lca.start[u] : lca.end[u], lca.start[v]};
    }
    auto h = f(q2);
    vector<int> ord(m);
    iota(ord.begin(), ord.end(), 0);
    sort(ord.begin(), ord.end(), [&](int i, int j) { return h[i] < h[j]; });
    vector<int> cnt(n);
    vector<R> ans(m);
    auto modify = [&](int u, int c) {
      if (c & 1)
        add(u);
      else
        erase(u);
    };
    for (int i = 0, cl = 0, cr = -1; i < m; i++) {
      auto [l, r] = q2[ord[i]];
      int u = lca.node[l], v = lca.node[r],
          a = lca.query(lca.node[l], lca.node[r]);
      while (cr < r)
        ++cr, modify(lca.node[cr], ++cnt[lca.node[cr]]);
      while (l < cl)
        --cl, modify(lca.node[cl], ++cnt[lca.node[cl]]);
      while (r < cr)
        modify(lca.node[cr], --cnt[lca.node[cr]]), --cr;
      while (cl < l)
        modify(lca.node[cl], --cnt[lca.node[cl]]), ++cl;
      if (a == u)
        ans[ord[i]] = f();
      else {
        modify(a, ++cnt[a]);
        ans[ord[i]] = apply(f, f_args[ord[i]]);
        modify(a, --cnt[a]);
      }
    }
    return ans;
  }
};
