#include <bits/stdc++.h>
using namespace std;

#include <celony/graph/bridges.hpp>

class Solution {
public:
  vector<vector<int>> criticalConnections(int n,
                                          vector<vector<int>> &connections) {
    vector<vector<int>> g(n);
    for (auto &e : connections) {
      g[e[0]].push_back(e[1]);
      g[e[1]].push_back(e[0]);
    }
    vector<vector<int>> ans;
    for (auto &[a, b] : bridges(g)) {
      ans.push_back({a, b});
    }
    return ans;
  }
};

int main() {
  cin.tie(0)->sync_with_stdio(false);
  int n, m;
  cin >> n >> m;
  vector<vector<int>> ed;
  for (int i = 0; i < m; i++) {
    int a, b;
    cin >> a >> b;
    a--, b--;
    ed.push_back({a, b});
  }
  auto ans = Solution().criticalConnections(n, ed);
  cout << ans.size() << '\n';
  for (auto &v : ans) {
    cout << v[0] << ' ' << v[1] << '\n';
  }
}