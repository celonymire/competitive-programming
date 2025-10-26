#include <bits/stdc++.h>
using namespace std;

#include <celony/range_query/heavy_light_decomposition.hpp>

int main() {
  cin.tie(0)->sync_with_stdio(false);
  int n, q;
  cin >> n >> q;
  vector<int> t(n);
  for (auto &v : t) {
    cin >> v;
  }
  vector<vector<int>> g(n);
  for (int i = 0; i < n - 1; i++) {
    int a, b;
    cin >> a >> b;
    a--, b--;
    g[a].push_back(b);
    g[b].push_back(a);
  }
  heavy_light_decomposition hld(g, t, 0,
                                [](int a, int b) { return max(a, b); });
  for (int i = 0; i < q; i++) {
    int t;
    cin >> t;
    if (t == 1) {
      int s, x;
      cin >> s >> x;
      s--;
      hld.set(s, x);
    } else {
      int a, b;
      cin >> a >> b;
      a--, b--;
      cout << hld.query(a, b) << ' ';
    }
  }
  cout << '\n';
}