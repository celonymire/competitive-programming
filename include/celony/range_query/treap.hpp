#include <bits/stdc++.h>
using namespace std;

/**
 * @brief Implicit Treap for sequence operations with range queries.
 *
 * A randomized binary search tree that maintains sequence order implicitly
 * through subtree sizes. Supports split/merge for flexible range operations,
 * including reversals, insertions, deletions, and range queries.
 *
 * @tparam T Value type.
 * @tparam Combine Binary associative operation for combining subtree answers.
 *
 * @note All operations are \f$O(\log N)\f$ in expectation due to random
 * priorities.
 * @note Supports lazy propagation through the reversal flag.
 * @note Perfect for problems requiring sequence modifications.
 */
template <typename T, typename Combine> class treap {
public:
  struct node {
    T v, ans;
    int p = rand(), size{1};
    bool rev;
    node *l, *r;
  } *root = nullptr;

private:
  Combine combine;

  static int _size(node *nd) { return nd ? nd->size : 0; }
  void _push(node *root) {
    if (root && root->rev) {
      root->rev = false;
      swap(root->l, root->r);
      if (root->l)
        root->l->rev ^= true;
      if (root->r)
        root->r->rev ^= true;
    }
    _update(root);
  }
  void _update(node *root) {
    if (root) {
      root->size = _size(root->l) + _size(root->r) + 1;
      root->ans = root->l && root->r ? combine(root->l->ans, root->r->ans)
                  : root->l          ? root->l->ans
                  : root->r          ? root->r->ans
                                     : T{};
      root->ans = combine(root->ans, root->v);
    }
  }
  static void _heapify(node *root) {
    while (root) {
      auto mx = root;
      if (root->l && root->l->p > mx->p)
        mx = root->l;
      if (root->r && root->r->p > mx->p)
        mx = root->r;
      if (mx == root)
        break;
      swap(root->p, mx->p);
      root = mx;
    }
  }

public:
  /**
   * @brief Constructs a treap from an array.
   *
   * Time Complexity: \f$O(N)\f$ expected
   *
   * @param v Initial array values.
   * @param combine Binary associative operation for combining subtree answers.
   */
  treap(const vector<T> &v, const Combine &combine) : combine(combine) {
    int n = v.size();
    auto build = [&](auto &&self, int l, int r) -> node * {
      if (l == r)
        return nullptr;
      int m = (l + r) / 2;
      auto root = new node{v[m]};
      root->l = self(self, l, m);
      root->r = self(self, m + 1, r);
      _heapify(root);
      _update(root);
      return root;
    };
    root = build(build, 0, n);
  }
  treap(int n, const T &v, const Combine &combine)
      : treap(vector<T>(n, v), combine) {}

  /**
   * @brief Returns a new node with specified value.
   *
   * Time Complexity: \f$O(1)\f$
   *
   * @param v Node value.
   */
  static node *new_node(const T &v) { return new node{v}; }

  /**
   * @brief Destroy the treap.
   *
   * Time Complexity: \f$O(N)\f$
   *
   * @param root Treap root.
   */
  static void destroy(node *root) {
    if (!root)
      return;
    destroy(root->l);
    destroy(root->r);
    delete root;
  }

  /**
   * @brief Splits the treap at position k into two treaps.
   *
   * Time Complexity: \f$O(\log N)\f$ expected
   *
   * @param root Root of the treap to split.
   * @param k Split position (left subtree will contain first k elements).
   * @param l Output parameter for the left subtree root.
   * @param r Output parameter for the right subtree root.
   */
  void split(node *root, int k, node *&l, node *&r) {
    if (!root) {
      l = r = nullptr;
      return;
    }
    _push(root);
    int sz = _size(root->l);
    if (sz < k) {
      split(root->r, k - sz - 1, root->r, r);
      l = root;
    } else {
      split(root->l, k, l, root->l);
      r = root;
    }
    _update(root);
  }

  /**
   * @brief Merges two treaps into one.
   *
   * Time Complexity: \f$O(\log N)\f$ expected
   *
   * @param l Root of the left treap.
   * @param r Root of the right treap.
   * @param root Output parameter for the merged treap root.
   *
   * @note This implementation assumes all elements in the left treap
   *       come before all elements in the right treap in sequence order.
   */
  void merge(node *l, node *r, node *&root) {
    _push(l);
    _push(r);
    if (!l || !r)
      root = l ? l : r;
    else if (l->p > r->p) {
      merge(l->r, r, l->r);
      root = l;
    } else {
      merge(l, r->l, r->l);
      root = r;
    }
    _update(root);
  }

  /**
   * @brief Reverses the elements in the range [l, r].
   *
   * Time Complexity: \f$O(\log N)\f$ expected
   *
   * @param root Root of the treap.
   * @param l Left boundary (inclusive, 0-indexed).
   * @param r Right boundary (inclusive, 0-indexed).
   */
  void reverse(node *root, int l, int r) {
    node *tl, *tm, *tr;
    split(root, l, tl, tm);
    split(tm, r - l + 1, tm, tr);
    tm->rev ^= true;
    merge(tl, tm, root);
    merge(root, tr, root);
  }

  /**
   * @brief Queries the range [l, r] using the combine operation.
   *
   * Time Complexity: \f$O(\log N)\f$ expected
   *
   * @param root Root of the treap.
   * @param l Left boundary (inclusive, 0-indexed).
   * @param r Right boundary (inclusive, 0-indexed).
   * @param ans0 Identity value for empty ranges.
   * @return The combined result of elements [l, r].
   */
  T query(node *root, int l, int r, const T &ans0) {
    node *tl, *tm, *tr;
    split(root, l, tl, tm);
    split(tm, r - l + 1, tm, tr);
    auto ans = tm ? tm->ans : ans0;
    merge(tl, tm, root);
    merge(root, tr, root);
    return ans;
  }

  /**
   * @brief Extracts all values from the treap in sequence order.
   *
   * Time Complexity: \f$O(N)\f$
   *
   * @param root Root of the treap.
   * @return Vector containing all elements in their sequence order.
   */
  vector<T> data(node *root) {
    vector<T> ans;
    auto dfs = [&](auto &&self, node *root) -> void {
      if (!root)
        return;
      _push(root);
      self(self, root->l);
      ans.push_back(root->v);
      self(self, root->r);
    };
    dfs(dfs, root);
    return ans;
  }
};
