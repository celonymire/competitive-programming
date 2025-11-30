// LeetCode keeps up to date with latest compilers, therefore we can adapt to
// newer standards and features

#pragma once

#include <bits/stdc++.h>
using namespace std;

struct TreeNode {
  int val;
  TreeNode *left = nullptr;
  TreeNode *right = nullptr;
  TreeNode() : val(0), left(nullptr), right(nullptr) {}
  TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
  TreeNode(int x, TreeNode *left, TreeNode *right)
      : val(x), left(left), right(right) {}
  ~TreeNode() {
    delete left;
    delete right;
  }
};
struct ListNode {
  int val;
  ListNode *next = nullptr;
  ListNode() : val(0), next(nullptr) {}
  ListNode(int x) : val(x), next(nullptr) {}
  ListNode(int x, ListNode *next) : val(x), next(next) {}
  ~ListNode() { delete next; }
};

template <typename T>
concept has_to_string = requires(T t) {
  { t.to_string() } -> same_as<std::string>;
};
template <typename T>
concept tuple_like = requires(T t) { typename tuple_size<T>::type; };

template <typename T>
void print_impl(ostream &os, const T &val, bool write_newline) {
  if constexpr (same_as<T, char>)
    os << '"' << val << '"';
  else if constexpr (same_as<T, bool>)
    os << (val ? "true" : "false");
  else if constexpr (same_as<T, string>)
    os << '"' << val << '"';
  else if constexpr (is_arithmetic_v<T>)
    os << val;
  else if constexpr (has_to_string<T>)
    os << '"' << val.to_string() << '"';
  else if constexpr (same_as<T, TreeNode *>) {
    queue<TreeNode *> q;
    os << '[';
    if (val) {
      q.push(val);
      os << val->val;
    }
    while (!q.empty()) {
      auto cur = q.front();
      q.pop();
      if (cur->left) {
        q.push(cur->left);
        os << ',' << cur->left->val;
      } else
        os << ",null";
      if (cur->right) {
        q.push(cur->right);
        os << ',' << cur->right->val;
      } else
        os << ",null";
    }
    os << ']';
  } else if constexpr (tuple_like<T>) {
    os << '{';
    apply(
        [&, c = 0](auto &&...args) mutable {
          ((c++ ? os << ',' : os, print_impl(os, args, false)), ...);
        },
        val);
    os << '}';
  } else if constexpr (same_as<T, ListNode *>) {
    auto cur = val;
    os << '[';
    while (cur) {
      if (cur != val)
        os << ',';
      os << cur->val;
      cur = cur->next;
    }
    os << ']';
  } else if constexpr (ranges::range<T>) {
    os << '[';
    for (auto it = val.begin(); it != val.end(); it++) {
      if (it != val.begin())
        os << ',';
      print_impl(os, *it, false);
    }
    os << ']';
  } else
    static_assert(false, "printing for type not supported");
  if (write_newline)
    os << '\n';
}

#define CONCAT_IMPL(x, y) x##y
#define CONCAT(x, y) CONCAT_IMPL(x, y)
#define NUM_ARGS_IMPL(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) N
#define NUM_ARGS(...) NUM_ARGS_IMPL(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)
#define DBG_VAL(x)                                                             \
  [&]() {                                                                      \
    auto val = x;                                                              \
    cerr << "[" << #x << " = ";                                                \
    print_impl(cerr, val, false);                                              \
    cerr << "] ";                                                              \
  }()
#define DBG_1(x) DBG_VAL(x)
#define DBG_2(x, ...) DBG_VAL(x), DBG_1(__VA_ARGS__)
#define DBG_3(x, ...) DBG_VAL(x), DBG_2(__VA_ARGS__)
#define DBG_4(x, ...) DBG_VAL(x), DBG_3(__VA_ARGS__)
#define DBG_5(x, ...) DBG_VAL(x), DBG_4(__VA_ARGS__)
#define DBG_6(x, ...) DBG_VAL(x), DBG_5(__VA_ARGS__)
#define DBG_7(x, ...) DBG_VAL(x), DBG_6(__VA_ARGS__)
#define DBG_8(x, ...) DBG_VAL(x), DBG_7(__VA_ARGS__)
#define DBG_9(x, ...) DBG_VAL(x), DBG_8(__VA_ARGS__)
#define DBG_10(x, ...) DBG_VAL(x), DBG_9(__VA_ARGS__)

// supports up to 10 arguments debugging at one time
#define dbg(...) CONCAT(DBG_, NUM_ARGS(__VA_ARGS__))(__VA_ARGS__), cerr << '\n';

template <typename T> T parse(istream &is = cin) {
  T ans;
  char ch;
  is >> ws;
  if constexpr (same_as<T, char>) {
    if (!(is >> ch) || ch != '"')
      throw runtime_error("Expected '\"' before char");
    if (!(is >> ans))
      throw runtime_error("Failed to parse char");
    if (!(is >> ch) || ch != '"')
      throw runtime_error("Expected '\"' after char");
  } else if constexpr (same_as<T, bool>) {
    string s;
    if (!(is >> s))
      throw runtime_error("Failed to parse bool");
    if (s != "true" && s != "false")
      throw runtime_error("Expected 'true' or 'false' for bool, got: " + s);
    ans = (s == "true");
  } else if constexpr (same_as<T, string>) {
    if (!(is >> ch) || ch != '"')
      throw runtime_error("Expected '\"' before string");
    if (!getline(is, ans, '"'))
      throw runtime_error("Failed to parse string or missing closing '\"'");
  } else if constexpr (is_arithmetic_v<T>) {
    if (!(is >> ans))
      throw runtime_error("Failed to parse numeric value");
  } else if constexpr (same_as<T, TreeNode *>) {
    if (!(is >> ch) || ch != '[')
      throw runtime_error("Expected '[' at start of TreeNode array");
    is >> ws;
    if (is.peek() == ']') {
      is >> ch;
      ans = nullptr;
    } else {
      bool right = true;
      auto dummy = new TreeNode{};
      queue<TreeNode *> q;
      q.push(dummy);

      try {
        while (is && is.peek() != ']') {
          is >> ws;
          string token;
          char c;
          while (is && (c = is.peek()) != ',' && c != ']') {
            token += c;
            is.get();
          }
          if (token.empty())
            throw runtime_error("Empty token in TreeNode array");

          if (token != "null") {
            int val;
            try {
              val = stoi(token);
            } catch (...) {
              throw runtime_error("Invalid integer in TreeNode array: " +
                                  token);
            }
            auto new_node = new TreeNode{val};
            if (right)
              q.front()->right = new_node;
            else
              q.front()->left = new_node;
            q.push(new_node);
          }
          if (right)
            q.pop();
          right = !right;

          if (is.peek() == ',')
            is >> ch;
        }
        if (!is || is.peek() != ']')
          throw runtime_error("Expected ']' at end of TreeNode array");
        is >> ch; // skip ']'
        ans = dummy->right;
        dummy->right = nullptr;
        delete dummy;
      } catch (...) {
        dummy->right = nullptr;
        delete dummy;
        throw;
      }
    }
  } else if constexpr (same_as<T, ListNode *>) {
    auto dummy = new ListNode{};
    auto cur = dummy;
    if (!(is >> ch) || ch != '[')
      throw runtime_error("Expected '[' at start of ListNode array");
    is >> ws;
    if (is.peek() != ']') {
      try {
        while (true) {
          cur->next = new ListNode{parse<int>(is)};
          cur = cur->next;
          is >> ws;
          if (is.peek() == ']')
            break;
          if (!(is >> ch) || ch != ',')
            throw runtime_error("Expected ',' between ListNode elements");
        }
      } catch (...) {
        dummy->next = nullptr;
        delete dummy;
        throw;
      }
    }
    if (!(is >> ch) || ch != ']')
      throw runtime_error("Expected ']' at end of ListNode array");
    ans = dummy->next;
    dummy->next = nullptr;
    delete dummy;
  } else if constexpr (ranges::range<T>) {
    if (!(is >> ch) || ch != '[')
      throw runtime_error("Expected '[' at start of array");
    is >> ws;
    if (is.peek() != ']') {
      while (true) {
        ans.emplace_back(parse<typename T::value_type>(is));
        is >> ws;
        if (!is)
          throw runtime_error("Stream error while parsing array");
        if (is.peek() == ']')
          break;
        if (!(is >> ch) || ch != ',')
          throw runtime_error("Expected ',' between array elements");
      }
    }
    if (!(is >> ch) || ch != ']')
      throw runtime_error("Expected ']' at end of array");
  } else
    static_assert(false, "parsing for type not supported");
  return ans;
}

template <typename Solution, typename R, typename... Ts>
void run(R (Solution::*fn)(Ts...)) {
  tuple<Solution, decay_t<Ts>...> args;
  get<0>(args) = Solution{};
  try {
    [&]<size_t... Idx>(index_sequence<Idx...>) {
      size_t arg_num = 0;
      (
          [&]() {
            arg_num = Idx + 1;
            get<Idx + 1>(args) = parse<decay_t<Ts>>();
          }(),
          ...);
    }(index_sequence_for<Ts...>{});
  } catch (const exception &e) {
    cerr << "Error: " << e.what() << endl;
    return;
  }

  try {
    if constexpr (same_as<R, void>) {
      apply(fn, args);
      []<size_t... Idx>(auto &&args, index_sequence<Idx...>) {
        ((cout << "#" << (Idx + 1) << ": ",
          print_impl(cout, get<Idx + 1>(args), true)),
         ...);
      }(args, index_sequence_for<Ts...>{});
    } else {
      auto res = apply(fn, args);
      print_impl(cout, res, true);
    }
  } catch (const exception &e) {
    cerr << "Runtime error: " << e.what() << endl;
  }
}
