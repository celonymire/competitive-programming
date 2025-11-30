#include <bits/stdc++.h>
using namespace std;

/**
 * @brief Mo's Algorithm for offline range queries on arrays.
 *
 * Efficiently answers multiple range queries by reordering them to minimize
 * the number of add/remove operations in a sliding window. Particularly useful
 * when direct computation is expensive but incremental updates are cheap.
 *
 * The actual performance heavily depends on the query ordering heuristic.
 * Hilbert curve ordering typically provides 2-3x speedup over simpler
 * heuristics.
 *
 * @tparam R Query answer type.
 * @tparam Args Additional argument types passed to answer function.
 *
 * @note Queries must be answered offline (all queries known in advance).
 * @note Best for problems where incremental updates are \f$O(1)\f$ or \f$O(\log
 * N)\f$.
 * @see https://codeforces.com/blog/entry/61203 for Hilbert curve details.
 */
template <typename R, typename... Args> class mo_array {
  vector<pair<int, int>> q;
  vector<tuple<Args...>> f_args;
  function<void(int)> add, erase;
  function<R(Args...)> f;

public:
  /**
   * @brief Constructs a Mo's algorithm solver.
   */
  mo_array(function<void(int)> add, function<void(int)> erase,
           function<R(Args...)> f)
      : add(std::move(add)), erase(std::move(erase)), f(std::move(f)) {}

  /**
   * @brief Adds a range query to be solved offline.
   *
   * Time Complexity: \f$O(1)\f$
   *
   * @param l Left boundary (inclusive).
   * @param r Right boundary (inclusive).
   * @param args Additional arguments to pass to the answer function.
   */
  void query(int l, int r, const Args &...args) {
    q.push_back({l, r});
    f_args.emplace_back(args...);
  };

  /**
   * @brief Solves all queries and returns answers in insertion order.
   *
   * Time Complexity: \f$O((N + Q)\sqrt{N})\f$ where Q is the number of queries
   *
   * @param heuristic Function that assigns a comparable value to each query.
   *                  Queries are processed in increasing order of these values.
   * @return Vector of answers corresponding to each query in the order they
   * were added.
   */
  template <typename H> auto solve(H &&heuristic) {
    int m = q.size();
    auto h = invoke(std::forward<H>(heuristic), q);
    vector<int> ord(m);
    iota(ord.begin(), ord.end(), 0);
    sort(ord.begin(), ord.end(), [&](int i, int j) { return h[i] < h[j]; });
    int cl = 0, cr = -1;
    vector<R> ans(m);
    for (int i = 0; i < m; i++) {
      while (cr < q[ord[i]].second)
        add(++cr);
      while (q[ord[i]].first < cl)
        add(--cl);
      while (q[ord[i]].second < cr)
        erase(cr--);
      while (cl < q[ord[i]].first)
        erase(cl++);
      ans[ord[i]] = apply(f, f_args[ord[i]]);
    }
    return ans;
  }
};
