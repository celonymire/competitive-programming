#include <bits/stdc++.h>
using namespace std;

#include <celony/range_query/treap.hpp>

int main() {
  int n, m;
  string s;
  cin >> n >> m >> s;
  vector<char> v(s.begin(), s.end());
  treap t(v, plus{});
  for (int i = 0; i < m; i++) {
    int l, r;
    cin >> l >> r;
    l--, r--;
    using node_t = decltype(t)::node;
    node_t *tl = nullptr, *tm = nullptr, *tr = nullptr;
    t.split(t.root, l, tl, tm);
    t.split(tm, r - l + 1, tm, tr);
    t.merge(tl, tr, t.root);
    t.merge(t.root, tm, t.root);
  }
  auto ans = t.data(t.root);
  for (char c : ans) {
    cout << c;
  }
  cout << '\n';
}