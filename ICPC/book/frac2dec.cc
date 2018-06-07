// Fraction to decimal expansion. O(m) where m is the denomintor
const int MAX_DENOM = 1001;
string itoa(int x){ stringstream ss; ss << x; return ss.str(); }

int firstSeen[MAX_DENOM];
void frac2dec(int numer,int denom,string& decimal,int& numRepDigs){
  if(numer == 0) { decimal = "0"; numRepDigs = 0; return; }
  decimal = ((numer<0 && denom>0) || (numer>0 && denom<0)) ? "-" : "+";
  numer = abs(numer);   denom = abs(denom);
  decimal += itoa(numer / denom);
  numer %= denom;
  if(!numer){ numRepDigs = 0;   return;   }
  decimal += '.';
  
  fill(firstSeen,firstSeen+denom,-1);
  int rem = numer;
  while(rem != 0 && firstSeen[rem] == -1){
    firstSeen[rem] = decimal.length();
    rem *= 10; decimal += itoa(rem / denom); rem %= denom;
  }
  numRepDigs = (rem ? decimal.length() - firstSeen[rem] : 0);
}
