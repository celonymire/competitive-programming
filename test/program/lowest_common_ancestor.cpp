#include <bits/stdc++.h>
using namespace std;

#include <celony/graph/lowest_common_ancestor.hpp>

int main() {
  cin.tie(0)->sync_with_stdio(false);
  int n, q;
  cin >> n >> q;
  vector<int> p(n + 1);
  vector g(n + 1, vector<int>());
  for (int i = 0; i < n - 1; i++) {
    int b;
    cin >> b;
    g[b].push_back(i + 2);
  }
  lowest_common_ancestor lca(g, 1);
  for (int i = 0; i < q; i++) {
    int a, b;
    cin >> a >> b;
    cout << lca.query(a, b) << '\n';
  }
}