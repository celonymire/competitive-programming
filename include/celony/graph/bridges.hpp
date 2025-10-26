#include <bits/stdc++.h>
using namespace std;

/**
 * @brief Find all the edges that are bridges.
 *
 * @param g The graph.
 */
vector<pair<int, int>> bridges(vector<vector<int>> &g) {
  int n = g.size();
  vector<int> in(n, -1), low(n, -1);
  vector<pair<int, int>> ans;
  auto dfs = [&, timer = 0](auto &self, int v, int p = -1) mutable -> void {
    in[v] = low[v] = timer++;
    for (int to : g[v]) {
      if (to == p)
        continue;
      if (in[to] != -1) {
        low[v] = min(low[v], in[to]);
      } else {
        self(self, to, v);
        low[v] = min(low[v], low[to]);
        if (low[to] > in[v])
          ans.push_back({v, to});
      }
    }
  };
  for (int i = 0; i < n; ++i)
    if (in[i] == -1)
      dfs(dfs, i);
  return ans;
};