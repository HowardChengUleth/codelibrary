// Find the longest palindromic substrings (or all)
// Returns the starting index and the length of the palindrome
pair<int,int> longest_palindrome(vector<int> input){
  int a1=-1,a2=-2,a3=-3; // Three DIFFERENT numbers that do NOT appear in your input
  int C,R,n = 2*input.size()+3;   vector<int> v(n,a1), P(n,0);
  v[0] = a2, v[n-1] = a3;
  for(int i=0;i<input.size();i++) v[2*i+2] = input[i];
  for(int i=1;i<n-1;i++){
    for(P[i]=(R>i ? min(R-i,P[2*C-i]) : 0) ; v[i+1+P[i]] == v[i-1-P[i]] ; P[i]++) {}
    if(P[i]+i > R) C = i, R = P[i]+i;
  }
  int loc = max_element(v.begin(),v.end()) - v.begin(); // All ties here are also
  return make_pair((loc-1-v[loc])/2,v[loc]);            // longest palindromes
}
