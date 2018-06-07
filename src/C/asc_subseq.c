/*
 * Longest Ascending Subsequence
 *
 * Author: Howard Cheng
 * Reference:
 *   Gries, D.  The Science of Programming
 *
 * Given an array of size n, asc_seq returns the length of the longest
 * ascending subsequence, as well as one of the subsequences in S.
 * sasc_seq returns the length of the longest strictly ascending
 * subsequence.  It runs in O(n log n) time.
 *
 * Note: If we want to find do the same things with descending
 * subsequences, just reverse the array before calling the routines.
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int asc_seq(int *A, int n, int *S)
{
  int *m, *seq, i, k, j, h, e, start;
  m = malloc((n+1) * sizeof(int));
  seq = malloc(n * sizeof(int));
  assert(m && seq);

  for (i = 0; i < n; i++) {
    seq[i] = -1;
  }
  i = k = 1;
  m[1] = start = 0;
  for (; i < n; i++) {
    if (A[i] >= A[m[k]]) {
      m[k+1] = i;
      seq[i] = m[k];
      start = i;
      k++;
    } else if (A[i] < A[m[1]]) {
      m[1] = i;
    } else {
      assert(A[m[1]] <= A[i] && A[i] < A[m[k]]);
      h = 1;
      j = k;
      while (h != j-1) {
	e = (h+j)/2;
	if (A[m[e]] <= A[i]) {
	  h = e;
	} else {
	  j = e;
	}
      }
      seq[i] = m[h];
      m[j] = i;
    }
  }

  for (i = k-1; i >= 0; i--) {
    S[i] = A[start];
    start = seq[start];
  }

  free(m);
  free(seq);
  return k;
}

int sasc_seq(int *A, int n, int *S)
{
  int *m, *seq, i, k, j, h, e, start;
  m = malloc((n+1) * sizeof(int));
  seq = malloc(n * sizeof(int));
  assert(m && seq);

  for (i = 0; i < n; i++) {
    seq[i] = -1;
  }
  i = k = 1;
  m[1] = start = 0;
  for (; i < n; i++) {
    if (A[i] > A[m[k]]) {
      m[k+1] = i;
      seq[i] = m[k];
      start = i;
      k++;
    } else if (A[i] < A[m[1]]) {
      m[1] = i;
    } else if (A[i] < A[m[k]]) {
      h = 1;
      j = k;
      while (h != j-1) {
	assert(A[m[h]] <= A[i] && A[i] < A[m[j]]);
	e = (h+j)/2;
	if (A[m[e]] <= A[i]) {
	  h = e;
	} else {
	  j = e;
	}
      }
      if (A[i] > A[m[h]]) {
	seq[i] = m[h];
	m[j] = i;
      }
    }
  }

  for (i = k-1; i >= 0; i--) {
    S[i] = A[start];
    start = seq[start];
  }

  free(m);
  free(seq);
  return k;
}



int main(void)
{
  int *A, *S, n, i, k;

  while (scanf("%d", &n) == 1 && n > 0) {
    A = malloc(n*sizeof(int));
    S = malloc(n*sizeof(int));
    for (i = 0; i < n; i++) {
      scanf("%d", &A[i]);
    }
    k = asc_seq(A, n, S);
    printf("length = %d\n", k);
    for (i = 0; i < k; i++) {
      printf("%d ", S[i]);
    }
    printf("\n");

    k = sasc_seq(A, n, S);
    printf("length = %d\n", k);
    for (i = 0; i < k; i++) {
      printf("%d ", S[i]);
    }
    printf("\n");
    free(A);
    free(S);
  }
  return 0;
}
