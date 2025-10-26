#include <bits/stdc++.h>
using namespace std;

/**
 * @brief Treap class that automatically handles updating subtree values.
 *
 * @tparam T Value type.
 * @tparam Combine Combines two subtree values with signature `Fn(left, right)
 * -> new-value`.
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
   * @param v Node value.
   */
  static node *new_node(const T &v) { return new node{v}; }

  /**
   * @brief Destroy the treap.
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
   * @brief Split the treap where `l` have first `k` elements and `r` have the
   * rest.
   *
   * @param root Root of treap.
   * @param k Number of elements for `l`.
   * @param l Pointer reference for left root.
   * @param r Pointer reference for right root.
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
   * This implemenetation assumes all left treap node values are less than
   * right treap node.
   *
   * @param l Left treap root.
   * @param r Right treap root.
   * @param root Pointer reference for new root;
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
   * @brief Reverses the range `[l, r]`.
   *
   * @param root Treap root.
   * @param l Left boundary.
   * @param r Right boundary.
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
   * @brief Queries the range `[l, r]`.
   *
   * @param root Treap root.
   * @param l Left boundary.
   * @param r Right boundary.
   * @param ans0 Default answer for nodes.
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
   * @brief Returns the values of nodes ordered by their indices.
   *
   * @param root Treap root.
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
