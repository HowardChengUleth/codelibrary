// Converts a fraction (with integral numerator and denominator)
//   to its decimal expansion.
//
// Author: Darcy Best
// Date  : August 22, 2010
//
// Since we are dealing with rational numbers, one of two cases
//   occur:
//      1. The number will terminate
//      2. The number will repeat
//
// The algorithm is O(D) where D is the absolute value of the 
//   denominator.

#include <iostream>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <cassert>
using namespace std;

const int MAX_DENOM = 1001;

string itoa(int x){
  string ans;
  while(x){
    ans += (x % 10) + '0';
    x /= 10;
  }
  reverse(ans.begin(),ans.end());
  return (ans.length() ? ans : "0");
}

int firstSeen[MAX_DENOM];

void frac2dec(int numer,int denom,string& decimal,int& numRepDigs){
  assert(denom != 0);
	
  // Determine if it is a plus or a minus
  decimal = "";
  if(numer < 0 && denom >= 0 || numer >= 0 && denom < 0){
    decimal += "-";
  } else {
    decimal += "+";
  }
  numer = abs(numer);
  denom = abs(denom);
	
  // Left of the decimal point
  decimal += itoa(numer / denom);
  numer %= denom;
  if(!numer){
    numRepDigs = 0;
    return;
  }
	
  // Add the decimal point
  decimal += '.';
	
  // Right of the decimal point
  fill(firstSeen,firstSeen+denom,-1);
  int rem = numer;
  while(rem != 0 && firstSeen[rem] == -1){
    firstSeen[rem] = decimal.length();
    rem *= 10;
		
    decimal += itoa(rem / denom);
    rem %= denom;
  }
	
  numRepDigs = (rem ? decimal.length() - firstSeen[rem] : 0);
}

int main(){
  int numerator,denominator,repDigs;
  string decimal;
  while(cin >> numerator >> slash >> denominator){
    frac2dec(numerator,denominator,decimal,repDigs);
    cout << numerator << " / " << denominator << " = " << decimal << endl;
    if(repDigs == 0)
      cout << "This expansion terminates." << endl;
    else
      cout << "The last " << repDigs << " digits repeat forever." << endl;
    cout << endl;
  }
  return 0;
}
