#include <bits/stdc++.h>
using namespace std;
#define fst first
#define snd second

#include <ext/pb_ds/assoc_container.hpp>  // for both
#include <ext/pb_ds/tag_and_trait.hpp>    // for trie
#include <ext/pb_ds/trie_policy.hpp>      // for trie
#include <ext/pb_ds/tree_policy.hpp>      // for set
using namespace __gnu_pbds;
typedef trie<string, null_type, trie_string_access_traits<>, pat_trie_tag, trie_prefix_search_node_update> pftrie;
typedef tree<int, null_type, less<int>, rb_tree_tag, tree_order_statistics_node_update> ordered_set;

int main() {
  // compressed prefix trie
  pftrie t;
  t.insert("banana"); t.insert("orange");
  auto match = t.prefix_range("ban");
  for (auto it = match.fst; it != match.snd; it++) cout << *it << endl; // banana

  // zero-based ordered set
  ordered_set s;
  for (int i = 0; i < 10; i++) s.insert(i); // 0,1,2,3,4,5,6,7,8,9
  cout << *s.find_by_order(1) << endl; //        ^
  cout << s.order_of_key(3) << endl; //              ^
}

