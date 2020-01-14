// Josephus Problem (0-based) -- Kill the k'th person first
//   1. Determine the survivor           -- O(n)
//       -- Do not include inner for-loop (j)
//       -- A[i] is the survivor with i people, killing every k'th.
//   2. Determine the full killing order -- O(n^2)
//       -- Include inner for-loop (j)
//       -- A[i] is the i'th person who is killed (A[n] is survivor)
void josephus(int A[], int n,int k){
  A[1] = 0;
  for (int i = 2; i <= n; i++) { A[i] = (A[i-1]+(k%i))%i;
    for (int j = 1; j < i; j++) A[j] = (A[j]+(k%i))%i;
  }
}
