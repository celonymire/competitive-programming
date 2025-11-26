#include <bits/stdc++.h>
using namespace std;

/**
 * @brief Find all vertices that are articulation points (cut vertices) in an undirected graph.
 *
 * An articulation point is a vertex whose removal increases the number of connected components.
 * Uses Tarjan's algorithm with DFS to identify such vertices efficiently.
 *
 * Time Complexity: O(V + E) where V is the number of vertices and E is the number of edges.
 * Space Complexity: O(V) for the auxiliary arrays and recursion stack.
 *
 * @param g The adjacency list representation of the undirected graph.
 *            g[i] contains the list of neighbors of vertex i.
 * @return A vector containing the indices of all articulation points.
 *
 * @note The graph is assumed to be undirected.
 * @note Works correctly on graphs with multiple connected components.
 */
vector<int> articulation_points(vector<vector<int>> &g) {
  int n = g.size();
  vector<int> in(n, -1), low(n, -1), ans;
  auto dfs = [&, timer = 0](auto &self, int v, int p = -1) mutable -> void {
    in[v] = low[v] = timer++;
    int children = 0;
    bool is_ap = false;
    for (int to : g[v]) {
      if (to == p)
        continue;
      if (in[to] != -1) {
        low[v] = min(low[v], in[to]);
      } else {
        self(self, to, v);
        low[v] = min(low[v], low[to]);
        if (low[to] >= in[v] && p != -1)
          is_ap = true;
        ++children;
      }
    }
    if (p == -1 && children > 1)
      is_ap = true;
    if (is_ap)
      ans.push_back(v);
  };
  for (int i = 0; i < n; ++i)
    if (in[i] == -1)
      dfs(dfs, i);
  return ans;
};
