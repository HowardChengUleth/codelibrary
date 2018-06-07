// Use built-in hashing for numbers, strings, pointers, vector<bool>,
// bitset. Others require custom hashing.

struct XYZ {
  int a; string b;
  bool operator==(const XYZ& r) const { // unordered_map only needs equality
    return a == r.a && b == r.b; }
};

namespace std {
  template<> struct hash<XYZ> {
    size_t operator()(const XYZ& xyz) const {
      return hash<int>()(xyz.a) ^ hash<string>()(xyz.b); }
  };
}

unordered_map<XYZ,string> um; // Elements are NOT sorted. Access is O(1).
