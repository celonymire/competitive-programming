#include <bits/stdc++.h>
using namespace std;

#include <celony/range_query/segment_tree.hpp>

int main() {
  int n, m;
  cin >> n >> m;
  vector<int> x(n);
  for (auto &v : x) {
    cin >> v;
  }
  struct S {
    long long sum = 0, mxl = 0, mxr = 0, mx = 0;
  };
  auto cb = [](const S &a, const S &b) -> S {
    S s;
    s.sum = a.sum + b.sum;
    s.mxl = max(a.mxl, a.sum + b.mxl);
    s.mxr = max(b.mxr, a.mxr + b.sum);
    s.mx = max(s.sum, max(a.mx, b.mx));
    s.mx = max(s.mx, max(s.mxl, s.mxr));
    s.mx = max(s.mx, a.mxr + b.mxl);
    return s;
  };
  segment_tree seg(n, S{}, cb);
  for (int i = 0; i < n; i++) {
    int v = max(x[i], 0);
    seg.set(i, S{x[i], v, v, v});
  }
  for (int i = 0; i < m; i++) {
    int k, v;
    cin >> k >> v;
    cout << seg.query(k - 1, v).mx << '\n';
  }
}