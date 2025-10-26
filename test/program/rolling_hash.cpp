#include <bits/stdc++.h>
using namespace std;

#include <celony/string/rolling_hash.hpp>

int main() {
  cin.tie(0)->sync_with_stdio(false);
  string S;
  cin >> S;
  int N = S.size();
  rolling_hash hs(S);
  int l = 0, r = N;
  int p = -1;
  while (r - l > 1) {
    int m = (l + r) >> 1;
    unordered_set<uint64_t> st;
    bool ok = false;
    for (int i = 0; i + m <= N; i++) {
      auto h = hs.hash(i, i + m);
      if (st.find(h) != st.end()) {
        p = i;
        ok = true;
      }
      st.insert(h);
    }
    if (ok) {
      l = m;
    } else {
      r = m;
    }
  }
  if (p == -1) {
    cout << "-1\n";
  } else {
    cout << S.substr(p, l) << '\n';
  }
}