#include <bits/stdc++.h>
using namespace std;

#include <celony/range_query/fenwick.hpp>

int main() {
  cin.tie(0)->sync_with_stdio(false);
  int n, q;
  cin >> n >> q;
  vector<long long> x(n);
  for (auto &v : x) {
    cin >> v;
  }
  fenwick fw(x, plus{});
  for (int i = 0; i < q; i++) {
    int t;
    cin >> t;
    if (t == 1) {
      int k, u;
      cin >> k >> u;
      fw.update(k, -x[k - 1] + u);
      x[k - 1] = u;
    } else {
      int a, b;
      cin >> a >> b;
      cout << fw.query(b) - fw.query(a - 1) << '\n';
    }
  }
}