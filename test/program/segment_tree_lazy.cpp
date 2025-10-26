#include <bits/stdc++.h>
using namespace std;

#include <celony/range_query/segment_tree_lazy.hpp>

int main() {
  int n, q;
  cin >> n >> q;
  vector<int> t(n);
  for (auto &v : t) {
    cin >> v;
  }
  struct S {
    int l, r;
    long long s;
  };
  struct D {
    int c;
    long long s;
  };
  auto g = [](long long x) -> long long { return x * (x + 1) / 2; };
  auto apply = [&](S s, D d, int k) -> S {
    return {s.l, s.r, s.s + (g(s.r) - g(s.l - 1)) * d.c - d.s * k};
  };
  auto push = [](D a, D b, int) -> D { return {a.c + b.c, a.s + b.s}; };
  auto cb = [&](S a, S b) -> S { return {a.l, b.r, a.s + b.s}; };
  vector<S> init(n);
  for (int i = 0; i < n; i++) {
    init[i].l = init[i].r = i;
    init[i].s = t[i];
  }
  segment_tree_lazy seg(init, S{}, D{}, apply, push, cb);
  for (int i = 0; i < q; i++) {
    int t, a, b;
    cin >> t >> a >> b;
    a--, b--;
    if (t == 1) {
      seg.update(a, b + 1, D{1, a - 1});
    } else {
      cout << seg.query(a, b + 1).s << '\n';
    }
  }
}