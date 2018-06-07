// Java template for using BigInteger class.
//
// Note that there is also a similar BigDecimal class which may be useful.
//
// Name of the file must be NAME.java where NAME is the class name.
//
// To compile:
//
// javac NAME.java
//
// To run:
//
// java NAME
//
//
// Note: in Java, all non-native types (including arrays) need to be
// allocated by new.  Multidimensional arrays can be allocated in one
// call.  See below.
//

// for importing IO routines
import java.io.*;
import java.util.Scanner;
import java.math.BigInteger;

class bignumber {

  // this is main
  public static void main(String argv[])
  {
    // A scanner can be used to read many different types
    Scanner sc = new Scanner(System.in);

    // checking whether there is a next token can be done before
    // actually reading it and fail (vs. I/O model in C++)
    while (sc.hasNextInt()) {
      int N = sc.nextInt();
      int K = sc.nextInt();

      // here is how to allocate two dimensional arrays
      BigInteger binom[][] = new BigInteger[N+1][N+1];
      for (int n = 0; n <= N; n++) {
	// here is how you construct from an integer. 
	binom[n][0] = binom[n][n] = BigInteger.valueOf(1);
	for (int k = 1; k < n; k++) {
	  binom[n][k] = binom[n-1][k-1].add(binom[n-1][k]);
	}
      }

    
      // to print something, use System.out.println().
      // Arguments are strings (in double quotes), and most data types
      // can be converted to strings and concatenated.
      //
      // Call it with no argument to produce a blank line, or use print()
      // to print without a trailing end-of-line
      System.out.println("C(" + N + "," + K + ") = " + binom[N][K]);
    }

    /*

    Here are a bunch of other things you can do with BigIntegers.
    Assuming a, b, c, d are BigIntegers, n is an int

    a = BigInteger.ZERO;               a = 0
    a = BigInteger.ONE;                a = 1
    a = new BigInteger("FF", 16);      a = 255
    a = sc.nextBigInteger();           cin >> a
    s = a.toString();                  convert to string representation
    s = a.toString(base);              convert to string representation in
                                       given base

    x = a.intValue();                  convert to smaller types, but may
    x = a.longValue();                 lose precision
    x = a.floatValue();
    x = a.doubleValue();

    a = b.abs();                       a = |b|
    n = a.signum();                    n = 0, +1, -1 depending on sign of a

    a = b.negate();                    a = -b;
    a = b.add(c);                      a = b+c
    a = b.subtract(c);                 a = b-c
    a = b.multiply(c);                 a = b*c
    a = b.divide(c);                   a = b/c
    a = b.remainder(c);                a = b%c
    a = b.mod(c);                      a = b%c, but c must be positive
                                                and a >= 0

    if (a.compareTo(b))                -1 if a < b
                                       0 if a == b
                                       1 if a > b
    if (a.equals(b))                   true iff a == b

    a = b.min(c);                      a = min(b, c)
    a = b.max(c);                      a = max(b, c)
    
    a = b.pow(n);                      a = pow(b, n)
    a = b.modpow(n, c);                a = pow(b, n) mod c, c > 0
    
    a = b.gcd(c);                      a = gcd(|b|, |c|)
    a = b.modInverse(c);               a = b^(-1) mod c

    n = a.bitLength();                 number of bits in 2's complement
                                       representation, minus the sign bit

    if (a.isProbablePrime(n))          whether a is prime, with error
                                       of (1/2)^n
    
    */

    
    System.exit(0);
  }
}