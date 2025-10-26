#include <bits/stdc++.h>
using namespace std;

#include <celony/graph/strongly_connected_components.hpp>

int main() {
  int n, m;
  cin >> n >> m;
  vector<vector<int>> g(n + 1);
  for (int i = 0; i < m; i++) {
    int a, b;
    cin >> a >> b;
    g[a].push_back(b);
  }
  int k = 0;
  vector<int> ans(n + 1);
  for (auto &scc : strongly_connected_components(g)) {
    k++;
    for (int v : scc) {
      ans[v] = k;
    }
  }
  cout << k - 1 << '\n';
  for (int i = 1; i <= n; i++) {
    cout << ans[i] << ' ';
  }
  cout << '\n';
}