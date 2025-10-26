#include <bits/stdc++.h>
using namespace std;

#include <celony/miscellaneous/hilbert.hpp>
#include <celony/range_query/mo_array.hpp>

int main() {
  cin.tie(0)->sync_with_stdio(false);
  int n, q;
  cin >> n >> q;
  vector<int> a(n);
  for (auto &v : a) {
    cin >> v;
  }
  vector<int> cnt(1e6 + 1);
  long long cur_ans = 0;
  auto add = [&](int i) -> void {
    cur_ans -= 1LL * cnt[a[i]] * cnt[a[i]] * a[i];
    cnt[a[i]]++;
    cur_ans += 1LL * cnt[a[i]] * cnt[a[i]] * a[i];
  };

  auto remove = [&](int i) -> void {
    cur_ans -= 1LL * cnt[a[i]] * cnt[a[i]] * a[i];
    cnt[a[i]]--;
    cur_ans += 1LL * cnt[a[i]] * cnt[a[i]] * a[i];
  };
  auto get_answer = [&]() -> long long { return cur_ans; };
  mo_array<long long> mo(add, remove, get_answer);
  for (int i = 0; i < q; i++) {
    int l, r;
    cin >> l >> r;
    mo.query(l - 1, r - 1);
  }
  for (auto v : mo.solve(&hilbert)) {
    cout << v << ' ';
  }
}