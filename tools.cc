import <string>;

using namespace std;



// Lets you apply repeated concatenation through multiplication
// For example, "abc" * 5 produces "abcabcabcabcabc"
string operator*(string s, int n) {
  string x = "";
  for (int i = 0; i < n; ++i) {
    x += s;
  }
  return x;
}