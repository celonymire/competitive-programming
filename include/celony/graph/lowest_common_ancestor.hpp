#include <bits/stdc++.h>
using namespace std;

/**
 * @brief Lowest Common Ancestor (LCA) data structure using ±1 RMQ reduction.
 *
 * Answers LCA queries in O(1) time after O(N) preprocessing. This implementation
 * uses the Euler tour technique with ±1 Range Minimum Query (RMQ) optimization,
 * which is theoretically optimal. The algorithm splits the tour into blocks,
 * uses sparse table for inter-block queries, and precomputes all possible
 * block configurations for intra-block queries.
 *
 * @note This is one of the most efficient LCA implementations for competitive programming.
 * @note The tree must be connected and rooted at the specified root vertex.
 */
class lowest_common_ancestor {
  vector<int> tour, start, end, node, depth, mask;
  vector<vector<int>> dp;
  vector<vector<vector<int>>> blocks;

  int _min_by_depth(int i, int j) const {
    return depth[tour[i]] < depth[tour[j]] ? i : j;
  }
  int _lca_block(int b, int l, int r) const {
    int b_size = max(1, (int)__lg(tour.size()) >> 1);
    return blocks[mask[b]][l][r] + b * b_size;
  }

  template <typename R, typename... Args> friend class mo_tree;

public:
  /**
   * @brief Constructs the LCA data structure for the given tree.
   *
   * Time Complexity: O(N)
   * Space Complexity: O(N)
   *
   * @param g The adjacency list representation of the tree.
   * @param root The root vertex of the tree (default is 0).
   */
  lowest_common_ancestor(const vector<vector<int>> &g, int root = 0) {
    int n = g.size();
    tour.reserve(2 * n);
    start.resize(n);
    end.resize(n);
    node.resize(2 * n);
    depth.resize(n);
    auto dfs = [&](auto &self, int u, int p, int d) -> void {
      node[start[u] = end[u] = tour.size()] = u;
      depth[u] = d;
      tour.push_back(u);
      for (int v : g[u]) {
        if (v != p) {
          self(self, v, u, d + 1);
          tour.push_back(u);
        }
      }
      node[end[u] = tour.size()] = u;
    };
    dfs(dfs, root, root, 0);
    int m = tour.size();
    int b_size = max(1, __lg(m) >> 1);
    int b_cnt = (m + b_size - 1) / b_size;
    dp.resize(b_cnt, vector(__lg(b_cnt) + 1, 0));
    for (int i = 0, j = 0, b = 0; i < m; i++, j++) {
      if (j == b_size)
        j = 0, b++;
      if (j == 0 || _min_by_depth(i, dp[b][0]) == i)
        dp[b][0] = i;
    }
    for (int l = 1; l <= __lg(b_cnt); l++) {
      for (int i = 0; i < b_cnt; i++) {
        if (int j = i + (1 << (l - 1)); j >= b_cnt)
          dp[i][l] = dp[i][l - 1];
        else
          dp[i][l] = _min_by_depth(dp[i][l - 1], dp[j][l - 1]);
      }
    }
    mask.resize(b_cnt);
    for (int i = 0, j = 0, b = 0; i < m; i++, j++) {
      if (j == b_size)
        j = 0, b++;
      if (j > 0 && (i >= m || _min_by_depth(i - 1, i) == i - 1))
        mask[b] += 1 << (j - 1);
    }
    blocks.resize(1 << (b_size - 1));
    for (int b = 0; b < b_cnt; b++) {
      int msk = mask[b];
      if (!blocks[msk].empty())
        continue;
      blocks[msk].resize(b_size, vector(b_size, 0));
      for (int l = 0; l < b_size; l++) {
        blocks[msk][l][l] = l;
        for (int r = l + 1; r < b_size; r++) {
          blocks[msk][l][r] = blocks[msk][l][r - 1];
          if (b * b_size + r < m)
            blocks[msk][l][r] =
                _min_by_depth(b * b_size + blocks[msk][l][r], b * b_size + r) -
                b * b_size;
        }
      }
    }
  }

  /**
   * @brief Computes the lowest common ancestor of two nodes.
   *
   * Time Complexity: O(1)
   *
   * @param v First node index.
   * @param u Second node index.
   * @return The index of the lowest common ancestor of u and v.
   */
  int query(int v, int u) const {
    int l = start[v], r = start[u],
        b_size = max(1, (int)__lg(tour.size()) >> 1);
    if (l > r)
      swap(l, r);
    int bl = l / b_size;
    int br = r / b_size;
    if (bl == br)
      return tour[_lca_block(bl, l % b_size, r % b_size)];
    int ans = _min_by_depth(_lca_block(bl, l % b_size, b_size - 1),
                            _lca_block(br, 0, r % b_size));
    if (bl + 1 < br) {
      int l = __lg(br - bl - 1);
      ans = _min_by_depth(ans,
                          _min_by_depth(dp[bl + 1][l], dp[br - (1 << l)][l]));
    }
    return tour[ans];
  }
};
