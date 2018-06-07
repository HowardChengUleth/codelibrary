/*
 * Binary Search
 *
 * Author: Howard Cheng
 *
 * Note: you may wish to use the STL functions lower_bound and upper_bound
 * instead.
 *
 * Given a sorted array A of size n, it tries to find an item x in the
 * the array using binary search.  The function returns non-zero if
 * x is found, and zero otherwise.  Furthermore, if it is found, then
 * A[index] = x.  If it is not found, then index is the place x should
 * be inserted into A.
 *
 * ie.  A[i] <= x           for 0 <= i < index
 *              x < A[i]    for index <= i < n
 *
 * This routine is written for integer arrays, but can be adapted to
 * other types by changing the comparison operator.
 *
 * There is also an insert routine here that will insert the element into
 * the right place after the array has been reallocated (if necessary) to
 * store n+1 elements.
 *
 */

#include <iostream>
#include <cassert>

using namespace std;

bool bin_search(const int A[], int n, int x, int &index)
{
   int l, u, m;

   if (n <= 0 || x < A[0]) {  // check the first element, but only if it exists
      index = 0;
      return false;
   }
   if (A[n-1] < x) {
      index = n;
      return false;
   }
   if (x == A[n-1]) {
      index = n-1;
      return true;
   }
   l = 0;
   u = n-1;
   while (l+1 < u) {
      assert(A[l] <= x && x < A[u]);
      m = (l+u)/2;
      if (A[m] <= x) {
         l = m;
      } else {
         u = m;
      }
   }
   if (A[l] == x) {
      index = l;
      return true;
   } else {
      index = u;
      return false;
   }
}

void insert(int A[], int n, int x, int index)
{
   int i;
   for (i = n-1; i >= index+1; i--) {
      A[i] = A[i-1];
   }
   A[index] = x;
}


int main(void)
{
   int A[10000];
   int n, i, x, index;

   // implements binary insertion sort, but only keeps the unique elements
   n = 0;
   while (cin >> x && n < 10000) {
      if (!bin_search(A, n, x, index)) {
         n++;
         insert(A, n, x, index);
      }
      cout << "List:";
      for (i = 0; i < n; i++) {
	cout << " " << A[i];
	if (i == index) {
	  cout << "*";       // show which one is just inserted
	}
      }
      cout << endl;
   }
   return 0;
}

