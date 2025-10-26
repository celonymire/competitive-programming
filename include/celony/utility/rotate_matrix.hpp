#include <bits/stdc++.h>
using namespace std;

/**
 * @brief Rotates a matrix by 90 degrees.
 *
 * @param g The matrix.
 */
template <typename T> vector<vector<T>> rot90(const vector<vector<T>> &g) {
  int n = g.size(), m = g[0].size();
  vector ans(m, vector<int>(n));
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++)
      ans[j][n - i - 1] = g[i][j];
  return ans;
}