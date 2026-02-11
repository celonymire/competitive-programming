#include <bits/stdc++.h>
using namespace std;

/**
 * @brief Dinic's algorithm for maximum flow.
 *
 * Computes the maximum flow in a directed graph with given capacities in
 * \f$O(VE^2)\f$ time with \f$O(VE)\f$ space.
 *
 * @tparam F Flow capacity type.
 *
 * @see https://codeforces.com/blog/entry/66006 for scaling optimization.
 */
template <typename F> struct max_flow {
  /**
   * @brief Edge structure for the flow graph.
   *
   * The original vertex is omitted since the graph is an adjacency list.
   */
  struct edge_t {
    int to;  // Destination vertex index.
    int rev; // Index of the reverse edge in `g[to]`.
    F cap;   // Remaining capacity of the edge.
  };
  vector<vector<edge_t>> g; // Adjacency list graph.

  /**
   * @brief Constructs a max flow graph with the given number of vertices.
   *
   * @param n Number of vertices in the graph.
   */
  max_flow(int n) : g(n) {}

  /**
   * @brief Adds a directed edge with the given capacity to the flow graph.
   *
   * @param a From vertex index.
   * @param b To vertex index.
   * @param cap Capacity of the edge.
   */
  void add_edge(int a, int b, F cap) {
    g[a].push_back({b, (int)g[b].size(), cap});
    g[b].push_back({a, (int)g[a].size() - 1, 0});
  }

  /**
   * @brief Computes the maximum flow from source to target.
   *
   * Time Complexity: \f$O(VE^2)\f$.
   *
   * @param source Source vertex index.
   * @param target Target vertex index.
   * @param initial_flow Initial flow to send (default is infinity).
   */
  F flow(int source, int target, F initial_flow = numeric_limits<F>::max()) {
    int n = g.size();
    F ans = 0;
    while (1) {
      vector<int> lvl(n, -1);
      lvl[source] = 0;
      queue<int> q{{source}};
      while (q.size()) {
        auto v = q.front();
        q.pop();
        for (auto &e : g[v]) {
          if (e.cap > 0 && lvl[e.to] == -1) {
            lvl[e.to] = lvl[v] + 1;
            q.push(e.to);
          }
        }
      }
      if (lvl[target] == -1) {
        break;
      }
      auto dfs = [&](auto &&self, int v, F f) -> F {
        if (v == target) {
          return f;
        }
        F ans = 0;
        for (auto &e : g[v]) {
          F ff = min(f, e.cap);
          if (lvl[v] + 1 == lvl[e.to] && ff > 0) {
            F res = self(self, e.to, ff);
            e.cap -= res;
            g[e.to][e.rev].cap += res;
            f -= res;
            ans += res;
          }
        }
        lvl[v] = -1;
        return ans;
      };
      F flow = dfs(dfs, source, initial_flow);
      if (flow == 0) {
        break;
      }
      ans += flow;
      initial_flow -= flow;
    }
    return ans;
  }
};