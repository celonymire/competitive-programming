#include <bits/stdc++.h>
using namespace std;

#include <celony/data_structure/union_find.hpp>

int main() {
  cin.tie(0)->sync_with_stdio(false);
  int n, m;
  cin >> n >> m;
  int mx_size = 1, comps = n;
  vector<int> size(n + 1, 1);
  union_find uf(n + 1, [&](int a, int b) {
    size[a] += size[b];
    mx_size = max(mx_size, size[a]);
  });
  for (int i = 0; i < m; i++) {
    int a, b;
    cin >> a >> b;
    uf.unite(a, b);
    cout << uf.components() - 1 << ' ' << mx_size << '\n';
  }
}