#include <bits/stdc++.h>
using namespace std;

/**
 * @brief Node class for the Aho-Corasick Automaton.
 *
 * @tparam Begin Lowest character in range.
 * @tparam End Highest character in range.
 */
template <int Begin, int End> struct aho_corasick_node_t {
  int next[End - Begin + 1]{}; // Next node index from a given character.
  int link = 0;                // Suffix link.
  int exit = 0;                // Next match link.
  vector<int> matches;         // Index of matches in provided patterns.
};

/**
 * @brief Constructs an Aho-Corasick automaton for multi-pattern string matching.
 *
 * Time Complexity: O(sum of pattern lengths + alphabet size * number of states)
 * Space Complexity: O(sum of pattern lengths * alphabet size)
 *
 * @tparam Begin Lowest character value in the alphabet range.
 * @tparam End Highest character value in the alphabet range.
 * @param p Vector of pattern strings to match.
 * @return The constructed automaton as a vector of nodes.
 *
 * @note Template parameters define the alphabet using C-style arrays for performance.
 */
template <int Begin, int End>
vector<aho_corasick_node_t<Begin, End>>
aho_corasick_automaton(const vector<string> &p) {
  int m = p.size();
  vector<aho_corasick_node_t<Begin, End>> g(1);
  for (int i = 0; i < m; i++) {
    int u = 0;
    for (char c : p[i]) {
      if (!g[u].next[c - Begin]) {
        g[u].next[c - Begin] = g.size();
        g.push_back({});
      }
      u = g[u].next[c - Begin];
    }
    g[u].matches.push_back(i);
  }
  queue<int> q{{0}};
  while (!q.empty()) {
    int u = q.front();
    q.pop();
    for (int a = 0; a <= End - Begin; a++) {
      if (int v = g[u].next[a]) {
        g[v].link = u ? g[g[u].link].next[a] : 0;
        g[v].exit =
            g[g[v].link].matches.empty() ? g[g[v].link].exit : g[v].link;
        q.push(v);
      } else
        g[u].next[a] = g[g[u].link].next[a];
    }
  }
  return g;
}