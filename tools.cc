export module tools;

import <string>;
import <random>;
import <vector>;



// Lets you apply repeated concatenation through multiplication
// For example, "abc" * 5 produces "abcabcabcabcabc"
export std::string operator*(std::string s, int n) {
  std::string x = "";
  for (int i = 0; i < n; ++i) {
    x += s;
  }
  return x;
}

// Generates a random number between min and max
export int generateRandom(int min, int max) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(min, max);
  return distrib(gen);
}

// randomizes the order of a given vector
export template <typename T>
void shuffleVector(std::vector<T>& vec) {
    for (int i = vec.size() - 1; i > 0; --i) {
        int j = generateRandom(0, i);
        std::swap(vec[i], vec[j]);
    }
}
