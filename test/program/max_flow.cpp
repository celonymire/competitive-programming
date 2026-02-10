#include <bits/stdc++.h>
using namespace std;

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
template <typename F> class max_flow {
  vector<vector<tuple<int, int, int, F>>> g; // [to, rev id, e idx, cap]
  vector<tuple<int, int, F>> e;              // [a, b, cap]

public:
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
    g[a].push_back({b, (int)g[b].size(), (int)e.size(), cap});
    g[b].push_back({a, (int)g[a].size() - 1, -1, 0});
    e.push_back({a, b, cap});
  }

  /**
   * @brief Returns the adjacency list representation of the flow graph.
   */
  const vector<tuple<int, int, F>> &edges() const { return e; }

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
        for (auto &[u, rev, e_idx, flow] : g[v]) {
          if (flow > 0 && lvl[u] == -1) {
            lvl[u] = lvl[v] + 1;
            q.push(u);
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
        for (auto &[u, rev, e_idx, cap] : g[v]) {
          F ff = min(f, cap);
          if (lvl[v] + 1 == lvl[u] && ff > 0) {
            F res = self(self, u, ff);
            cap -= res;
            get<3>(g[u][rev]) += res;
            if (e_idx != -1) {
              get<2>(e[e_idx]) -= res;
            } else {
              get<2>(e[get<2>(g[u][rev])]) += res;
            }
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
    }
    return ans;
  }
};

int main() {
  int n, m;
  cin >> n >> m;
  max_flow<long long> g(n + 1);
  for (int i = 0; i < m; i++) {
    long long a, b, c;
    cin >> a >> b >> c;
    g.add_edge(a, b, c);
  }
  cout << g.flow(1, n) << '\n';
}