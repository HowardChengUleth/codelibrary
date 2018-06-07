/*
 * Big integer implementation
 *
 * Author: Howard Cheng
 *
 * Each digit in our representation represents LOG_BASE decimal digits
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <assert.h>
#include <time.h>

/* MAX_DIGIT should be defined to ensure that there is no overflow      */
/* it should be one digit more than the specified bound (for division), */
/* or a few more for safety.                                            */
#define MAX_DIGIT 60
#define BASE 10000
#define LOG_BASE 4
#define FMT_STR "%04d"

typedef int BigNum[MAX_DIGIT];

/*
 * For all operations taking a "result" parameter, it cannot be the
 * same as one of the operands unless stated otherwise.
 *
 */

/* a = b */
static void copy(BigNum a, BigNum b);

/* a = 0 */
static void clear(BigNum a);

/* returns the number of digits in a (i.e. don't count leading 0's) */
static int digits(BigNum a);

/* a = b (a: BigNum, b: int) */
static void convert_from_int(BigNum a, int b);

/* 
 * returns integer form of the BigNum 
 *
 */
static int convert_to_int(BigNum a);

/* a == 0 */
static int is_zero(BigNum a);

/*
 * return   1 if a > b
 *          0 if a = b
 *         -1 if a < b	
 */
static int compare(BigNum a, BigNum b);

/* result = a + b,
 * result can be the same as one of the operands
 */
static void add(BigNum a, BigNum b, BigNum result);

/* result = a - b,
 * result can be the same as one of the operands
 * assumes a >= b
 */
static void subtract(BigNum a, BigNum b, BigNum result);

/* a++ */
static void inc(BigNum a);

/* a--, assumes a > 0 */
static void dec(BigNum a);

/* a <<= amount, or a = a*BASE^amount */
static void lshift(BigNum a, int amount);

/* result = a * b, where b is a single digit 
 * result can be the same as a.
 */
static void mult_digit(BigNum a, int b, BigNum result);

/* result = a * b */
static void mult(BigNum a, BigNum b, BigNum result);

/* result = x^y, y must be a non-negat integer */
static void power(BigNum x, int y, BigNum result);

/* q = a / b, r = a % b */
static void divide(BigNum a, BigNum b, BigNum q, BigNum r);

/* q = a / b, r = a % b, where b is a single digit 
 * q can be the same as a
 */
static void short_divide(BigNum a, int b, BigNum q, int *r);

/* root = floor(sqrt(a)).  Returns 1 if a is a perfect square, 0 otherwise. */
static int square_root(BigNum a, BigNum result);

/* read BigNum from stdin */
static void read_num(BigNum a);

/* write BigNum to stdout */
static void write_num(BigNum a);

int main(void)
{
  BigNum a, b, c, q, r;
  int count;
  int limit = 100000;

  read_num(a);
  while (!is_zero(a)) {
    if (square_root(a, b)) {
      printf("Perfect square, root = ");
    } else {
      printf("floor(root) = ");
    }
    write_num(b);
    printf("\n");
    read_num(a);
  }
  return 0;

  srand(time(NULL));
  for (count = 0; count < limit; count++) {
    /* generate random a and b */
    int l1, l2, i;
    clear(a);
    clear(b);
    l1 = rand() % (MAX_DIGIT-1);
    l2 = rand() % (MAX_DIGIT-1);
    for (i = 0; i < l1; i++) {
      a[i] = rand() % BASE;
    }
    b[0] = rand() % (BASE-1) + 1;
    for (i = 1; i < l2; i++) {
      b[i] = rand() % BASE;
    }
    if (count % 1000 == 0) {
      printf("count = %d\n", count);
      fflush(stdout);
    }
    divide(a, b, q, r);
    mult(b, q, c);
    add(c, r, c);
    if (compare(a, c) != 0) {
      printf("count = %d\n", count);
      printf("a = "); write_num(a); printf("\n");
      printf("b = "); write_num(b); printf("\n");
      printf("q = "); write_num(q); printf("\n");
      printf("r = "); write_num(r); printf("\n");
      printf("b*q+r = "); write_num(c); printf("\n");
      exit(1);
    }
  }
  printf("All tests passed.\n");
  return 0;
}

/* perform assignment a = b */
static void copy(BigNum a, BigNum b)
{
  int i;
  for (i = 0; i < MAX_DIGIT; i++) {
    a[i] = b[i];
  }
}

/* perform assignment a = 0 */
static void clear(BigNum a)
{
  int i;
  for (i = 0; i < MAX_DIGIT; i++) {
    a[i] = 0;
  }
}

/* returns the number of digits in a (i.e. don't count leading 0's) */
static int digits(BigNum a)
{
  int i;
  for (i = MAX_DIGIT; i > 0; i--) {
    if (a[i-1]) {
      return i;
    }
  }
  return 1;
}

/* a = b (a: BigNum, b: int) */
static void convert_from_int(BigNum a, int b)
{
  int i = 0;
  clear(a);
  while (b > 0) {
    a[i++] = b % BASE;
    b /= BASE;
  }
}

/* 
 * returns integer form of the BigNum 
 *
 */
static int convert_to_int(BigNum a)
{
  int r = 0;
  int i;

  for (i = MAX_DIGIT-1; i >= 0; i--) {
    assert(r <= (INT_MAX - a[i]) / BASE);
    r *= BASE;
    r += a[i];
  }
  return r;
}

/* perform comparison a == 0 */
static int is_zero(BigNum a)
{
  int i;
  for (i = 0; i < MAX_DIGIT; i++) {
    if (a[i]) {
      return 0;
    }
  }
  return 1;
}

/* perform comparison between a and b.
   Returns -1 if a < b
            0 if a == b
           +1 if a > b */
static int compare(BigNum a, BigNum b)
{
  int i;
  for (i = MAX_DIGIT-1; i >= 0; i--) {
    if (a[i] < b[i]) {
      return -1;
    } else if (a[i] > b[i]) {
      return 1;
    }
  }
  return 0;
}

/* performs result = a + b */
static void add(BigNum a, BigNum b, BigNum result)
{
  int sum, carry, i;
  carry = 0;
  for (i = 0; i < MAX_DIGIT; i++) {
    sum = a[i] + b[i] + carry;
    result[i] = (sum < BASE) ? sum : sum - BASE;
    carry = (sum >= BASE);
  }
  assert(carry == 0);       /* assert no overflow */
}

/* perform result = a - b.  assumes a >= b */
static void subtract(BigNum a, BigNum b, BigNum result)
{
  int diff, borrow, i;
  borrow = 0;
  for (i = 0; i < MAX_DIGIT; i++) {
    diff = a[i] - b[i] - borrow;
    result[i] = (diff >= 0) ? diff : diff + BASE;
    borrow = (diff < 0);
  }
  assert(borrow == 0);
}

/* perform a++ */
static void inc(BigNum a)
{
  BigNum one;
  convert_from_int(one, 1);
  add(a, one, a);
}

/* perform a--, assumes a > 0 */
static void dec(BigNum a)
{
  BigNum one;
  convert_from_int(one, 1);
  subtract(a, one, a);
}

/* perform a *= BASE^amount */
static void lshift(BigNum a, int amount)
{
  int i;

  if (!amount) {
    return;
  }

  for (i = MAX_DIGIT-1; i >= amount; i--) {
    a[i] = a[i-amount];
  }
  for (i = 0; i < amount; i++) {
    a[i] = 0;
  }
}

/* result = a * b, where b is a single digit 
 * result can be the same as a.
 */
static void mult_digit(BigNum a, int b, BigNum result)
{
  int i, carry;
  int prod;

  assert(0 <= b && b < BASE);
  carry = 0;
  for (i = 0; i < MAX_DIGIT; i++) {
    prod = b * a[i];
    result[i] = (carry + prod) % BASE;
    carry = (carry + prod) / BASE;
  }
  assert(carry == 0);
}

/* result = a * b */
static void mult(BigNum a, BigNum b, BigNum result)
{
  int i;
  BigNum temp1;

  clear(result);

  for (i = 0; i < MAX_DIGIT; i++) {
    if (b[i] != 0) {
      copy(temp1, a);
      lshift(temp1, i);
      mult_digit(temp1, b[i], temp1);
      add(temp1, result, result);
    }
  }
}

/* result = x^y, y must be an integer */
static void power(BigNum x, int y, BigNum result)
{
  BigNum sx, t;
  
  convert_from_int(result, 1);
  copy(sx, x);

  while (y > 0) {
    if (y & 0x01) {
      y--;
      mult(result, sx, t);
      copy(result, t);
    } else {
      y >>= 1;
      mult(sx, sx, t);
      copy(sx, t);
    }
  }
}

/* q = a / b, r = a % b */
/* reference: Knuth v.2 Algorithm D */
static void divide(BigNum a, BigNum b, BigNum q, BigNum r)
{
  BigNum v;
  int d, m, n, t, i, j, q2, r2, carry, borrow, diff;

  assert(!is_zero(b));

  for (m = MAX_DIGIT-1; m >= 0 && !a[m]; m--)
    ;
  for (n = MAX_DIGIT-1; n >= 0 && !b[n]; n--)
    ;
  m -= n++;

  if (n == 1) {
    short_divide(a, b[0], q, &r2);
    convert_from_int(r, r2);
    return;
  }

  copy(r, a);
  copy(v, b);
  clear(q);

  if (m < 0) {
    /* nothing to do */
    return;
  }

  /* normalize */
  d = BASE / (v[n-1]+1);   /* NOTE: the book is wrong.  See errata on web */
  if (d > 1) {
    mult_digit(r, d, r);
    mult_digit(v, d, v);
  }

  /* loop */
  for (j = m; j >= 0; j--) {
    t = r[j+n] * BASE + r[j+n-1];
    q2 = t / v[n-1];
    r2 = t - q2 * v[n-1];
    if (q2 == BASE || q2 * v[n-2] > BASE * r2 + r[j+n-2]) {
      q2--;
      r2 += v[n-1];
      if (r2 < BASE && (q2 == BASE || q2 * v[n-2] > BASE * r2 + r[j+n-2])) {
	q2--;
	r2 += v[n-1];
      }
    }

    carry = borrow = 0;
    for (i = 0; i <= n; i++) {
      t = q2 * v[i] + carry;
      carry = t / BASE;
      t %= BASE;
      diff = r[j+i] - t - borrow;
      r[j+i] = (diff >= 0 || i == n) ? diff : diff + BASE;
      borrow = (diff < 0);
    }

    q[j] = q2;
    if (r[n+j] < 0) {
      q[j]--;
      carry = 0;
      for (i = 0; i < n; i++) {
	t = r[j+i] + v[i] + carry;
	r[j+i] = (t < BASE) ? t : t - BASE;
	carry = (t >= BASE);
      }
      r[j+n] += carry;
    }
  }

  /* unnormalize */
  if (d > 1) {
    short_divide(r, d, r, &t);
  }
}

/* q = a / b, r = a % b, where b is a single digit 
 * q can be the same as a
 */
static void short_divide(BigNum a, int b, BigNum q, int *r)
{
  int i, t;

  assert(0 < b && b < BASE);
  *r = 0;
  for (i = MAX_DIGIT-1; i >= 0; i--) {
    t = (*r) * BASE + a[i];
    q[i] = t / b;
    *r = t - q[i] * b;
  }
}

/* root = floor(sqrt(a)).  Returns 1 if a is a perfect square, 0 otherwise. */
static int square_root(BigNum a, BigNum result)
{
  int d, root_d;
  int k;
  BigNum x, r, t, t2;
  long long lo, hi, mid;

  /* figure out how many digits there are */
  d = digits(a);
  assert(MAX_DIGIT % 2 == 0);
  if (d % 2) d++;
  root_d = d / 2;

  clear(result);
  clear(x);

  clear(r);
  r[1] = a[--d];
  r[0] = a[--d];

  /* figure out one digit at a time */
  for (k = root_d-1; k >= 0; k--) {
    /* invariant: result is the square root (integer part) of the digits 
       processed so far */

    /* look for next digit in result by binary search */
    copy(x, result);
    mult_digit(x, 2, x);
    lshift(x, 1);
    clear(t);

    lo = 0;
    hi = BASE;
    
    while (hi - lo > 1) {
      mid = (lo + hi) / 2;
      x[0] = t[0] = mid;
      mult(x, t, t2);
      if (compare(t2, r) <= 0) {
        lo = mid;
      } else {
        hi = mid;
      }
    }

    lshift(result, 1);
    result[0] = lo;

    /* form the next r */
    x[0] = t[0] = lo;
    mult(x, t, t2);
    subtract(r, t2, r);
    lshift(r, 2);
    r[1] = a[--d];
    r[0] = a[--d];
  }
  return is_zero(r);
}

/* read BigNum from stdin */
static void read_num(BigNum a)
{
  char buffer[1024];     /* increase if necessary */
  int i, j, k, t, len, limit, mult;

  t = scanf("%s", buffer);
  assert(t == 1);
  len = strlen(buffer);
  clear(a);

  if (len % LOG_BASE == 0) {
    limit = len / LOG_BASE;
  } else {
    limit = len / LOG_BASE + 1;
  }

  j = len-1;
  for (i = 0; i < limit; i++) {
    mult = 1;
    for (k = 0; k < LOG_BASE; k++) {
      if (j >= 0) {
        a[i] += (buffer[j] - '0') * mult;
      }
      j--;
      mult *= 10;
    }
  }
}

/* write BigNum to stdout */
static void write_num(BigNum a)
{
  int i, k, writing;

  if (is_zero(a)) {
    printf("0");
  } else {
    k = 0;
    writing = 0;
    for (i = MAX_DIGIT-1; i >= 0; i--) {
      assert(0 <= a[i] && a[i] < BASE);
      if (writing) {
        printf(FMT_STR, a[i]);
      } else if (a[i]) {
        printf("%d", a[i]);
        writing = 1;
      }
    }
  }
  fflush(stdout);
}



