#include <bits/stdc++.h>
using namespace std;

#include <celony/range_query/sparse_table.hpp>

int main() {
  cin.tie(0)->sync_with_stdio(false);
  int n, q;
  cin >> n >> q;
  vector<int> x(n);
  for (auto &v : x) {
    cin >> v;
  }
  sparse_table sp(x, [](int a, int b) { return min(a, b); });
  for (int i = 0; i < q; i++) {
    int a, b;
    cin >> a >> b;
    cout << sp.query(a - 1, b - 1) << '\n';
  }
}