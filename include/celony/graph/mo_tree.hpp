#include <bits/stdc++.h>
using namespace std;

#include <celony/graph/lowest_common_ancestor.hpp>

/**
 * @brief Mo's Algorithm for offline path queries on trees.
 *
 * Efficiently answers multiple path queries on a tree by reordering them
 * to minimize the number of add/remove operations. Uses the Euler tour
 * technique to flatten the tree into an array, then applies Mo's algorithm.
 *
 * The actual performance heavily depends on the heuristic used to order queries.
 * Hilbert curve ordering provides the best practical performance.
 *
 * @tparam R Query answer type.
 * @tparam Args Additional argument types to pass to the answer function.
 *
 * @note Requires a lowest_common_ancestor object to be constructed first.
 * @see https://codeforces.com/blog/entry/61203 for Hilbert curve details.
 */
template <typename R, typename... Args> class mo_tree {
  vector<pair<int, int>> q;
  vector<tuple<Args...>> f_args;
  function<void(int)> add, erase;
  function<R(Args...)> f;

public:
  /**
   * @brief Constructs a Mo's algorithm solver for tree paths.
   */
  mo_tree(function<void(int)> add, function<void(int)> remove,
          function<R(Args...)> f)
      : add(std::move(add)), erase(std::move(erase)), f(std::move(f)) {}

  /**
   * @brief Adds a tree path query to be solved offline.
   *
   * Time Complexity: \f$O(1)\f$
   *
   * @param l First node index.
   * @param r Second node index.
   * @param args Additional arguments to pass to the answer function.
   */
  void add_query(int l, int r, const Args &...args) {
    q.push_back({l, r});
    f_args.push_back(args...);
  };

  /**
   * @brief Solves all tree path queries and returns answers in insertion order.
   *
   * Time Complexity: \f$O((N + Q)\sqrt{N})\f$ where Q is the number of queries
   *
   * @param f Heuristic function that assigns comparable values to queries.
   *          Queries are processed in increasing order of these values.
   *          For best performance, use Hilbert curve ordering.
   * @param lca The LCA data structure for the tree.
   * @return Vector of answers corresponding to each query in the order they were added.
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
