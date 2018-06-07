//
// Josephus Problem
//
// Author: Darcy Best
// Date  : September 4, 2010
//
// The Josephus problem:
//   A group of n people are in a circle, and you start by killing
//     person f. Then, you kill every kth person until only one person
//     is left.
//
// Two implementations are given here (Note that neither depend on k):
//   1. Determine the survivor           -- O(n)
//   2. Determine the full killing order -- O(n^2)
//
// If there are 17 people, with every 5th person killed (killing the 
//    1st person first), the kill order is:
//     1,6,11,16,5,12,2,9,17,10,4,15,14,3,8,13,7 (survivor = 7)
//
// NOTE: This is 1-based, not 0-based.

#include <iostream>
using namespace std;

const int MAX_N = 100;

int survivor(int n,int f,int k){
  return (n==1 ? 1 : (survivor(n-1,k,k) + (f-1)) % n + 1);
}

void killOrder(int n,int f,int k,int A[]){
  if(n == 0) return;
  A[0] = 0;
  killOrder(n-1,k,k,A+1);
  for(int i=0;i<n;i++)
    A[i] = (A[i] + (f-1)) % n + 1;
}

int main(){
  int n,f,k,kOrder[MAX_N];
  while(cin >> n >> f >> k && (n || f || k)){
    killOrder(n,f,k,kOrder);
    for(int i=0;i<n;i++)
      cout << kOrder[i] << endl;
		
    cout << "Survivor: " << survivor(n,f,k) << endl;
  }
  return 0;
}
