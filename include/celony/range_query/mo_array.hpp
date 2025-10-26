#include <bits/stdc++.h>
using namespace std;

/**
 * @brief Mo's Algorithm to answer offline range queries in `O(sqrt(N))`.
 *
 * Use this when the queries are non-trivial to merge from both halves, and the
 * answer can be computed whenever an element is added and deleted in a "sliding
 * window" fashion.
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
template <typename R, typename... Args> class mo_array {
  vector<pair<int, int>> q;
  vector<tuple<Args...>> f_args;
  function<void(int)> add, erase;
  function<R(Args...)> f;

public:
  mo_array(function<void(int)> add, function<void(int)> erase,
           function<R(Args...)> f)
      : add(std::move(add)), erase(std::move(erase)), f(std::move(f)) {}

  /**
   * @brief Adds a query of `f(a[l..r])` to solve.
   *
   * @param l Left boundary.
   * @param r right boundary.
   * @param args Additional arguments for the current query.
   */
  void query(int l, int r, const Args &...args) {
    q.push_back({l, r});
    f_args.emplace_back(args...);
  };

  /**
   * @brief Solves all the queries and return list of answers in the order of
   * the queries added.
   *
   * @param f The heuristic function with signature `Fn(queries) -> H` where
   * `Q[i]` will be processed before `Q[j]` if `H[i]<H[j]`.
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
