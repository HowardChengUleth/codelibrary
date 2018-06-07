/*
 * Big integer implementation
 *
 * Author: Howard Cheng
 *
 * Each digit in our representation represents LOG_BASE decimal digits
 *
 */

#include <vector>
#include <string>
#include <cstdio>
#include <cctype>
#include <iostream>
#include <algorithm>
#include <utility>
#include <cassert>

using namespace std;
using namespace std::rel_ops;

typedef long long Digit;
#define BASE 1000000000
#define LOG_BASE 9
#define FMT_STR "%lld"
#define FMT_STR0 "%09lld"

class BigInteger {
private:

  int sign;           // +1 = positive, 0 = zero, -1 = negative
  vector<Digit> mag;  // magnitude
  
  void normalize();

public:
  BigInteger(Digit n = 0);
  BigInteger(const string &s);    // no error checking

  long long toLongLong() const;   // convert to long long (assumes no overflow)
  string toString() const;        // convert to string

  void clear();  // set to zero

  // comparison
  bool operator<(const BigInteger &a) const;
  bool operator==(const BigInteger &a) const;
  bool isZero() const;

  // arithmetic
  BigInteger &operator+=(const BigInteger &a);
  BigInteger &operator-=(const BigInteger &a);
  BigInteger &operator*=(const BigInteger &a);
  BigInteger &operator*=(Digit a);
  BigInteger &operator<<=(Digit a);
  BigInteger &operator/=(const BigInteger &a);
  BigInteger &operator/=(Digit a);
  BigInteger &operator%=(const BigInteger &a);
  friend Digit operator%(const BigInteger &a, Digit b);
  
  // we have *this = b * q + r
  // r is such that 0 <= r < |b|
  void divide(const BigInteger &b, BigInteger &q, BigInteger &r) const;
  void divide(Digit b, BigInteger &q, Digit &r) const;

  // root = floor(sqrt(a)).  Returns 1 if a is a perfect square, 0 otherwise.
  // assume >= 0
  int sqrt(BigInteger &root) const;
};

BigInteger operator+(const BigInteger &a, const BigInteger &b);
BigInteger operator-(const BigInteger &a, const BigInteger &b);
BigInteger operator*(const BigInteger &a, const BigInteger &b);
BigInteger operator*(const BigInteger &a, Digit b);
BigInteger operator<<(const BigInteger &a, Digit b);
BigInteger operator/(const BigInteger &a, const BigInteger &b);
BigInteger operator/(const BigInteger &a, Digit b);
BigInteger operator%(const BigInteger &a, const BigInteger &b);
Digit operator%(const BigInteger &a, Digit b);

BigInteger power(BigInteger x, Digit y);
istream &operator>>(istream &is, BigInteger &a);
ostream &operator<<(ostream &os, const BigInteger &a);

void BigInteger::normalize()
{
  if (mag.size() == 0) {
    return;
  }
  vector<Digit>::iterator p = mag.end();
  do {
    if (*(--p) != 0) break;
  } while (p != mag.begin());
  if (p == mag.begin() && *p == 0) {
    clear();
    sign = 0;
  } else {
    mag.erase(++p, mag.end());
  }
}

BigInteger::BigInteger(Digit n)
{
  if (n == 0) {
    sign = 0;
    return;
  }
  if (n < 0) {
    sign = -1;
    n = -n;
  } else {
    sign = 1;
  }
  
  while (n > 0) {
    mag.push_back(n % BASE);
    n /= BASE;
  }
}

BigInteger::BigInteger(const string &s)
{
  int l = 0;
  bool zero = true;
  bool neg = false;

  clear();

  sign = 1;
  if (s[l] == '-') {
    neg = true;
    l++;
  }

  for (; l < s.length(); l++) {
    *this *= 10;
    *this += s[l] - '0';
    zero &= s[l] == '0';
  }

  if (zero) {
    clear();
  }
  if (neg) {
    sign = -1;
  }
}

long long BigInteger::toLongLong() const
{
  long long a = 0;
  for (int i = mag.size()-1; i >= 0; i--) {
    a *= BASE;
    a += mag[i];
  }
  return sign * a;
}

string BigInteger::toString() const
{
  char buffer[LOG_BASE+1];
  string s;

  if (isZero()) {
    return "0";
  } else {
    if (sign < 0) {
      s += "-";
    }
    for (int i = mag.size()-1; i >= 0; i--) {
      if (i == (int)(mag.size()-1)) {
	sprintf(buffer, FMT_STR, mag[i]);
      } else {
	sprintf(buffer, FMT_STR0, mag[i]);
      }
      s += buffer;
    }
    return s;
  }
}

void BigInteger::clear()
{
  sign = 0;
  mag.clear();
}

bool BigInteger::operator<(const BigInteger &a) const
{
  if (sign != a.sign) {
    return sign < a.sign;
  } else if (sign == 0) {
    return false;
  } else if (mag.size() < a.mag.size()) {
    return sign > 0;
  } else if (mag.size() > a.mag.size()) {
    return sign < 0;
  } else {
    for (int i = mag.size()-1; i >= 0; i--) {
      if (mag[i] < a.mag[i]) {
	return sign > 0;
      } else if (mag[i] > a.mag[i]) {
	return sign < 0;
      }
    }
    return false;
  }
}

bool BigInteger::operator==(const BigInteger &a) const
{
  return sign == a.sign && mag == a.mag;
}

bool BigInteger::isZero() const
{
  return sign == 0;
}

BigInteger &BigInteger::operator+=(const BigInteger &a)
{
  if (a.sign == 0) {
    return *this;
  } else if (sign == 0) {
    sign = a.sign;
    mag = a.mag;
    return *this;
  } else if (sign < 0 && a.sign > 0) {
    BigInteger b(a);
    sign = 1;
    b -= *this;
    return *this = b;
  } else if (sign > 0 && a.sign < 0) {
    BigInteger b(a);
    b.sign = 1;
    return (*this) -= b;
  } else {
    Digit carry = 0;
    unsigned int limit = max(mag.size(), a.mag.size());
    for (unsigned int i = 0; i < limit; i++) {
      Digit s1 = (i < mag.size()) ? mag[i] : 0;
      Digit s2 = (i < a.mag.size()) ? a.mag[i] : 0;
      Digit sum = s1 + s2 + carry;
      Digit result = (sum < BASE) ? sum : sum - BASE;
      carry = (sum >= BASE);
      if (i < mag.size()) {
	mag[i] = result;
      } else {
	mag.push_back(result);
      }
    }
    if (carry) {
      mag.push_back(carry);
    }
    return *this;
  }
}

BigInteger &BigInteger::operator-=(const BigInteger &a)
{
  if (a.sign == 0) {
    return *this;
  } else if (sign == 0) {
    sign = -a.sign;
    mag = a.mag;
    return *this;
  } else if (sign != a.sign) {
    BigInteger b(a);
    b.sign *= -1;
    return *this += b;
  } else if (sign < 0) {
    BigInteger b(a);
    b.sign *= -1;
    sign *= -1;
    b -= *this;
    return *this = b;
  } else {
    if (*this == a) {
      clear();
      return *this;
    } else if (*this < a) {
      BigInteger b(a);
      b -= *this;
      b.sign *= -1;
      return *this = b;
    } else {
      // we know that *this > a
      unsigned int limit = mag.size();
      Digit borrow = 0;
      for (unsigned int i = 0; i < limit; i++) {
	Digit s1 = mag[i];
	Digit s2 = (i < a.mag.size()) ? a.mag[i] : 0;
	Digit diff = s1 - s2 - borrow;
	mag[i] = (diff >= 0) ? diff : diff + BASE;
	borrow = (diff < 0);
      }
      normalize();
      return *this;
    }
  }
}

BigInteger &BigInteger::operator*=(const BigInteger &a)
{
  BigInteger temp(*this);
  BigInteger c;
  
  if (this == &a) {
    c = a;          // make a copy to prevent clobbering it
  }

  const BigInteger &b = (this == &a) ? c : a;
  
  clear();
  if (b.sign) {
    for (unsigned int i = 0; i < b.mag.size(); i++) {
      if (b.mag[i] != 0) {
	*this += (temp * b.mag[i]);
      }
      temp <<= 1;
    }
    sign *= b.sign;
  }
  return *this;
}

BigInteger &BigInteger::operator*=(Digit a)
{
  if (a <= -BASE || a >= BASE) {
    BigInteger b(a);
    return (*this *= b);
  }

  if (isZero()) {
    return *this;
  } else if (a == 0) {
    clear();
    return *this;
  } else if (a < 0) {
    sign *= -1;
    a = -a;
  }
    
  Digit carry = 0;
  for (unsigned int i = 0; i < mag.size(); i++) {
    Digit prod = a * mag[i];
    mag[i] = (carry + prod) % BASE;
    carry = (carry + prod) / BASE;
  }
  if (carry) {
    mag.push_back(carry);
  }
  return *this;
}

BigInteger &BigInteger::operator<<=(Digit a)
{
  assert(a >= 0);
  if (sign) {
    while (a-- > 0) {
      mag.insert(mag.begin(), 0);
    }
  }
  return *this;
}

BigInteger &BigInteger::operator/=(const BigInteger &a)
{
  BigInteger temp(*this), r;
  temp.divide(a, *this, r);
  return *this;
}

BigInteger &BigInteger::operator/=(Digit a)
{
  BigInteger temp(*this);
  Digit r;
  temp.divide(a, *this, r);
  return *this;
}
  
BigInteger &BigInteger::operator%=(const BigInteger &a)
{
  BigInteger temp(*this), q;
  temp.divide(a, q, *this);
  return *this;
}

void BigInteger::divide(const BigInteger &b, BigInteger &q, 
			BigInteger &r) const
{
  // reference Knuth v.2 Algorithm D

  assert(!b.isZero());

  if (b.mag.size() == 1) {
    Digit r2;
    divide(b.sign*b.mag[0], q, r2);
    r = r2;
    return;
  } 
    
  r = *this;
  if (r.sign < 0) {
    r.sign = 1;
  }
  q.clear();
  
  int n = b.mag.size();
  int m = mag.size() - n;
  if (m >= 0) {
    BigInteger v(b);
    q.mag.resize(m+1);
    q.sign = 1;

    // D1: normalize
    Digit d = BASE / (v.mag[n-1] + 1);  // Book is wrong.  See errata on web
    r *= d;
    v *= d;
    while ((int)r.mag.size() < m+n+1) {
      r.mag.push_back(0);
    }
      
    
    // loop
    for (int j = m; j >= 0; j--) {
      // D3: calculate q2
      Digit t = r.mag[j+n] * BASE + r.mag[j+n-1];
      Digit q2 = t / v.mag[n-1];
      Digit r2 = t - q2 * v.mag[n-1];
      if (q2 == BASE || q2 * v.mag[n-2] > BASE * r2 + r.mag[j+n-2]) {
	q2--;
	r2 += v.mag[n-1];
	if (r2 < BASE && 
	    (q2 == BASE || q2 * v.mag[n-2] > BASE * r2 + r.mag[j+n-2])) {
	  q2--;
	  r2 += v.mag[n-1];
	}
      }
      
      // D4: multiply and subtract
      Digit carry, borrow, diff;
      carry = borrow = 0;
      for (int i = 0; i <= n; i++) {
	t = q2 * ((i < n) ? v.mag[i] : 0) + carry;
	carry = t / BASE;
	t %= BASE;
	diff = r.mag[j+i] - t - borrow;
	r.mag[j+i] = (diff >= 0 || i == n) ? diff : diff + BASE;
	borrow = (diff < 0);
      }

      // D5: test remainder
      q.mag[j] = q2;
      if (r.mag[n+j] < 0) {
	// D6: add back
	q.mag[j]--;
	carry = 0;
	for (int i = 0; i < n; i++) {
	  t = r.mag[j+i] + v.mag[i] + carry;
	  r.mag[j+i] = (t < BASE) ? t : t - BASE;
	  carry = (t >= BASE);
	}
	r.mag[j+n] += carry;
      }
    }
 
    q.normalize();
    r.normalize();

    // D8: unnormalize
    r /= d;
  }

  // normalize
  if (sign < 0 && b.sign > 0) {
    q.sign *= -1;
    r *= -1;
    if (!r.isZero()) {
      r += b;
      q -= 1;
    }
  } else if (sign > 0 && b.sign < 0) {
    q.sign *= -1;
  } else if (sign < 0 && b.sign < 0 && !r.isZero()) {
    r += b;
    r *= -1;
    q += 1;
  }
}

void BigInteger::divide(Digit b, BigInteger &q, Digit &r) const
{
  if (b <= -BASE || b >= BASE) {
    BigInteger bb(b), rr;
    divide(bb, q, rr);
    r = rr.toLongLong();
    return;
  }

  int bsign = 1;
  if (b < 0) {
    b *= -1;
    bsign = -1;
  }
  q.clear();

  r = 0;
  for (int i = mag.size()-1; i >= 0; i--) {
    Digit t = r * BASE + mag[i];
    if (t / b > 0) {
      q.sign = 1;
    }
    q.mag.insert(q.mag.begin(), t / b);
    r = t - q.mag[0] * b;
  }

  // normalize
  q.normalize();

  if (sign < 0 && bsign > 0) {
    q.sign *= -1;
    r *= -1;
    if (r) {
      r += b;
      q -= 1;
    }
  } else if (sign > 0 && bsign < 0) {
    q.sign *= -1;
  } else if (sign < 0 && bsign < 0 && r) {
    r = b - r;
    q += 1;
  }
}

int BigInteger::sqrt(BigInteger &root) const
{
  assert(sign >= 0);
  root.clear();
  if (sign == 0) {
    return 1;
  }

  // figure out how many digits there are
  BigInteger x, r, t2;
  r.sign = 1;
  int d = mag.size();
  
  int root_d = (d % 2) ? (d+1)/2 : d / 2;

  if (d % 2) {
    r.mag.resize(1);
    r.mag[0] = mag[--d];
  } else {
    r.mag.resize(2);
    r.mag[1] = mag[--d];
    r.mag[0] = mag[--d];
  }

  root.sign = 1;

  // figure out one digit at a time
  for (int k = root_d - 1; k >= 0; k--) {
    // invariant: result is the sqrt (integer part) of the digits processed
    // so far
    
    // look for next digit in result by binary search
    x = root * 2;
    x <<= 1;
    Digit t;

    Digit lo = 0, hi = BASE;
    while (hi - lo > 1) {
      Digit mid = (lo + hi) / 2;
      x.mag[0] = t = mid;
      t2 = x * t;
      if (t2 < r || t2 == r) {
	lo = mid;
      } else {
	hi = mid;
      }
    }
    root <<= 1;
    root.mag[0] = lo;

    // form the next r
    x.mag[0] = t = lo;
    t2 = x * t;
    r -= t2;
    r <<= 1;
    r += (d > 0) ? mag[--d] : 0;
    r <<= 1;
    r += (d > 0) ? mag[--d] : 0;
  }

  return r.isZero();
}

BigInteger operator+(const BigInteger &a, const BigInteger &b)
{
  BigInteger r(a);
  r += b;
  return r;
}

BigInteger operator-(const BigInteger &a, const BigInteger &b)
{
  BigInteger r(a);
  r -= b;
  return r;
}

BigInteger operator*(const BigInteger &a, const BigInteger &b)
{
  BigInteger r(a);
  r *= b;
  return r;
}

BigInteger operator*(const BigInteger &a, Digit b)
{
  BigInteger r(a);
  r *= b;
  return r;
}

BigInteger operator<<(const BigInteger &a, Digit b)
{
  BigInteger r(a);
  r <<= b;
  return r;
}

BigInteger operator/(const BigInteger &a, const BigInteger &b)
{
  BigInteger r(a);
  r /= b;
  return r;
}

BigInteger operator/(const BigInteger &a, Digit b)
{
  BigInteger r(a);
  r /= b;
  return r;
}

BigInteger operator%(const BigInteger &a, const BigInteger &b)
{
  BigInteger r(a);
  r %= b;
  return r;
}
  
Digit operator%(const BigInteger &a, Digit b)
{
  Digit r;
  if (b > 0 && b < BASE) {
    r = 0;
    for (int i = a.mag.size()-1; i >= 0; i--) {
      r = ((r * BASE) + a.mag[i]) % b;
    }
    if (a.sign < 0) {
      r = (b - r) % b;
    }
    return r;
  }
  
  BigInteger q;
  
  a.divide(b, q, r);
  return r;
}

BigInteger power(BigInteger x, Digit y)
{
  BigInteger result(1), sx(x);
  
  assert(y >= 0);
  while (y > 0) {
    if (y & 0x01) {
      y--;
      result *= sx;
    } else {
      sx *= sx;
      y >>= 1;
    }
  }
  return result;
}

istream &operator>>(istream &is, BigInteger &a)
{
  string s;
  char c = ' ';

  is.get(c);
    
  while (!is.eof() && isspace(c)) {
    is.get(c);
  }
  if (is.eof()) {
    if (isdigit(c)) {
      a = (int)(c - '0');
      is.clear();
    }
    return is;
  }

  if (c == '-') {
    s = "-";
  } else {
    is.unget();
    if (!isdigit(c)) {
      return is;
    }
  }

  is.get(c);
  while (!is.eof() && isdigit(c)) {
    s += c;
    is.get(c);
  }
  if (!is.eof()) {
    is.unget();
  }
  a = s;
  is.clear();
  return is;
}

ostream &operator<<(ostream &os, const BigInteger &a)
{
  return (os << a.toString());
}


int main()
{
  BigInteger a, b;

  while (cin >> a >> b && (!(a == 0) || !(b == 0))) {
    cout << "a = " << a << endl;
    cout << "b = " << b << endl;
    if (!(a < 0)) {
      if (a.sqrt(b)) {
	cout << "perfect square" << endl;
      }
      cout << "sqrt(a) = " << b << endl;
    }
  }
  return 0;
}

