export module tools;

import <string>;



// Lets you apply repeated concatenation through multiplication
// For example, "abc" * 5 produces "abcabcabcabcabc"
export std::string operator*(std::string s, int n) {
  std::string x = "";
  for (int i = 0; i < n; ++i) {
    x += s;
  }
  return x;
}
