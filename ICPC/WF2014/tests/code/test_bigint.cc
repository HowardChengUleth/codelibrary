#include "everything"
#include "../../bigint.cc"

int main(void){
  BigInteger a, b;
  string s,t;
  while (cin >> s >> t){
    a = s;
    b = t;
    cout << "toString(): " << a.toString() << " " << b.toString() << endl;
    cout << "Comps (ab): " << (a < b) << (a == b) << (a > b) << (a <= b) << (a >= b) << endl;
    cout << "Comps (ba): " << (b < a) << (b == a) << (b > a) << (b <= a) << (b >= a) << endl;
    cout << "Comps (self): " << (a == a) << (a < a) << (b == b) << (b < b) << endl;
    if(a < (1ll << 62) && -a < (1ll << 62)) cout << a.toLongLong() << endl;
    if(b < (1ll << 62) && -b < (1ll << 62)) cout << b.toLongLong() << endl;
    
    cout << "a+b: " << a+b << endl;
    cout << "a-b: " << a-b << endl;
    if(!b.isZero()) cout << "a/b: " << a/b << endl;
    cout << "a*b: " << a*b << endl;
    if(b != BigInteger(0)) cout << "a%b: " << a%b << endl;
    cout << "a-a: " << a-a << endl;
    cout << "b+b: " << b+b << endl;
    cout << "a*a: " << a*a << endl;
    if(!b.isZero()) cout << "b%b: " << b%b << endl;
    if(!(a-1).isZero()) cout << "(a+1)/(a-1): " << (a+1)/(a-1) << endl;
    if(a >= BigInteger(0)){ BigInteger tmp; cout << a.sqrt(tmp) << endl;
      cout << tmp << endl;
    }
    if(b >= BigInteger(0)){ BigInteger tmp; cout << b.sqrt(tmp) << endl;
      cout << tmp << endl;
    }
  }
  return 0;
}
