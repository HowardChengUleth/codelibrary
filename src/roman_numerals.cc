// Converts Roman Numerals to Arabic Numbers (and vice versa)
//
// Author: Darcy Best
// Date  : September 5, 2010
//
// If you are given a valid integer (0 < x < 4000), then it will give
//    the standard roman numeral representation of it. Note that if you give
//    it a number such that x >= 4000, then it will just append as many "M"s
//    as needed.
//
// If you are given a valid roman numberal, then it will give you the answer
//    as a base 10 number.

#include <iostream>
#include <string>
#include <map>
using namespace std;

const string Roman[13]  = {"M","CM","D","CD","C","XC","L","XL","X","IX","V","IV","I"};
const int Arabic[13] = {1000,900,500,400,100,90,50,40,10,9,5,4,1};

string toRoman(int x){
  string roman;
  for(int i=0;i<13;i++)
    while(x >= Arabic[i]){
      x -= Arabic[i];
      roman += Roman[i];
    }
  return roman;
}

int toInt(string s){
  int L1,L2,ind=0,ans=0;
  while(ind < 13){
    L1 = s.length();
    L2 = Roman[ind].length();
    if(s.substr(0,min(L1,L2)) == Roman[ind]){
      ans += Arabic[ind];
      s.erase(0,min(L1,L2));
    } else {
      ind++;
    }
  }
  return ans;
}

int main(){
  char c;
  int x;
  string s;
	
  // Checks to see if the line is Roman Numerals or Arabic Numbers,
  //   then converts to the opposite.
  while(cin >> c){
    cin.putback(c);
    if(c >= '0' && c <= '9'){
      cin >> x;
      cout << toRoman(x) << endl;
    } else {
      cin >> s;
      cout << toInt(s) << endl;
    }
  }
  return 0;
}
