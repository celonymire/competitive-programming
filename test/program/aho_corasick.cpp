#include <bits/stdc++.h>
using namespace std;

#include <celony/string/aho_corasick.hpp>

int main() {
  string s;
  int k;
  cin >> s >> k;
  vector<string> p(k);
  for (auto &v : p) {
    cin >> v;
  }
  auto g = aho_corasick_automaton<'a', 'z'>(p);
  int m = g.size();
  vector<vector<int>> rlink(m);
  for (int i = 1; i < m; i++) {
    rlink[g[i].link].push_back(i);
  }
  vector<int> dp(m, 1e9);
  for (int i = 0, u = 0; i < s.size(); i++) {
    int c = s[i] - 'a';
    u = g[u].next[c];
    int p = u;
    if (g[p].matches.empty()) {
      p = g[p].exit;
    }
    dp[p] = min(dp[p], i + 1);
  }
  vector<int> ans(k, -1);
  auto dfs = [&](auto &&self, int u) -> int {
    for (int l : rlink[u]) {
      dp[u] = min(dp[u], self(self, l));
    }
    for (int p : g[u].matches) {
      ans[p] = dp[u];
    }
    return dp[u];
  };
  dfs(dfs, 0);
  for (int i = 0; i < k; i++) {
    cout << (ans[i] == 1e9 ? -1 : ans[i] - (int)p[i].size() + 1) << '\n';
  }
}