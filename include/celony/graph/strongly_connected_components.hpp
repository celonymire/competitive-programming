#include <bits/stdc++.h>
using namespace std;

/**
 * @brief Find all strongly connected components (SCCs) in a directed graph.
 *
 * A strongly connected component is a maximal subgraph where every vertex
 * is reachable from every other vertex. Uses Kosaraju's algorithm with two
 * DFS passes to identify all SCCs.
 *
 * Time Complexity: \f$O(V + E)\f$ where V is vertices and E is edges.
 * Space Complexity: \f$O(V + E)\f$ for storing the transpose graph.
 *
 * @param g The adjacency list representation of the directed graph.
 * @return A vector of vectors, where each inner vector contains the vertices
 *         of one strongly connected component.
 *
 * @note The SCCs are returned in reverse topological order of the
 *       condensation graph.
 */
vector<vector<int>> strongly_connected_components(vector<vector<int>> &g) {
  int n = g.size();
  vector<int> vis(n), ord;
  auto dfs1 = [&](auto &self, int u) -> void {
    vis[u] = true;
    for (int v : g[u])
      if (!vis[v])
        self(self, v);
    ord.push_back(u);
  };
  for (int i = 0; i < n; i++)
    if (!vis[i])
      dfs1(dfs1, i);
  reverse(ord.begin(), ord.end());
  vector<vector<int>> g2(n);
  for (int i = 0; i < n; i++)
    for (int j : g[i])
      g2[j].push_back(i);
  auto dfs2 = [&](auto &self, int u, vector<int> &comp) -> void {
    comp.push_back(u);
    vis[u] = false;
    for (int v : g2[u])
      if (vis[v])
        self(self, v, comp);
  };
  vector<vector<int>> ans;
  for (int u : ord)
    if (vis[u]) {
      ans.push_back({});
      dfs2(dfs2, u, ans.back());
    }
  return ans;
}