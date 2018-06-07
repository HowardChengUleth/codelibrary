/*
 * Binary Search
 *
 * Author: Howard Cheng
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

#include <stdio.h>
#include <assert.h>

int bin_search(int *A, int n, int x, int *index)
{
   int l, u, m;

   if (n <= 0 || x < A[0]) {
      *index = 0;
      return 0;
   }
   if (A[n-1] < x) {
      *index = n;
      return 0;
   }
   if (x == A[n-1]) {
      *index = n-1;
      return 1;
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
      *index = l;
      return 1;
   } else {
      *index = u;
      return 0;
   }
}

void insert(int *A, int n, int x, int index)
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

   n = 0;
   while (scanf("%d", &x) == 1 && n < 10000) {
      if (!bin_search(A, n, x, &index)) {
         n++;
         insert(A, n, x, index);
      }
      printf("List:");
      for (i = 0; i < n; i++) {
         printf(" %d", A[i]);
         if (i == index) {
            printf("*");
         }
      }
      printf("\n");
   }
   return 0;
}

