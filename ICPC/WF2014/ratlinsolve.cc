// Performs guassian elimination over the rationals.
//
// Author: Darcy Best
// Date  : September 22, 2010
//
// pair<int,int> means first = numerator, second = denominator

#include <iostream>
#include <iomanip>
#include <cstdlib>
using namespace std;

#define pii pair<int,int>
const int MAX_N = 100;

pii *r_m,m_m;

void print(pii x){
  if(x.second == 1)
    cout << x.first;
  else
    cout << x.first <<  "/" << x.second;
}

void print(pii A[MAX_N][MAX_N],int m,int n){
  for(int i=0;i<m;i++){
    for(int j=0;j<n;j++){
      cout << setw(5);
      print(A[i][j]);
    }
    cout << endl;
  }
  cout << endl;
}

void read(pii& x){
  cin >> x.first;
  char ch;
  if(cin.peek() == '/')
    cin >> ch >> x.second;
  else
    x.second = 1;
}

int gcd(int a,int b){
  while (b) {
    int r = a % b;
    a = b;
    b = r;
  }
  return a;
}

pii reduce(pii a){
  if(a.first == 0){
    a.second = 1;
  } else {
    if(a.second < 0){
      a.first *= -1;
      a.second *= -1;
    }
    int g = gcd(abs(a.first),a.second);
    a.first /= g;
    a.second /= g;
  }
  return a;
}

pii operator*(pii a,pii b){
  return reduce(pii(a.first*b.first,a.second*b.second));
}

pii operator+(pii a,pii b){
  return reduce(pii(a.first*b.second+b.first*a.second,a.second*b.second));
}

void multRow(pii& x){
  x = x * m_m;
}

void addMultRow(pii& x){
  x = x + (m_m * (*r_m++));
}

int rowReduction(pii A[MAX_N][MAX_N],int rows,int cols){
  int rank = 0;
  for(int c=0;c<cols;c++){
    for(int r=rank;r<rows;r++){
      if(A[r][c].first){
	if(r != rank) // Swap rows
	  swap_ranges(A[rank],A[rank]+cols,A[r]);
	if(c == cols-1) // Inconsistent
	  return -1;
				
	// Make first entry 1
	m_m = pii(A[rank][c].second,A[rank][c].first);
	for_each(A[rank]+c+1,A[rank]+cols,multRow);
	A[rank][c] = pii(1,0);
				
	for(int i=(arb?rank+1:0);i<rows;i++)
	  if(A[i][c].first && i != rank){
	    // Make the other rows 0
	    m_m = pii(-A[i][c].first,A[i][c].second);
	    r_m = A[rank]+c+1;
	    for_each(A[i]+c+1,A[i]+cols,addMultRow);
	    A[i][c] = pii(0,1);
	  }
	rank++;
	break;
      }
    }
  }
  return rank;
}

int main(){
  int C=0;
  int T,m,n,rank;
  pii A[MAX_N][MAX_N];
  while(cin >> T && T){
    if(C++)
      cout << endl;
    cout << "Solution for Matrix System # " << T << endl;
    cin >> n >> m;
    for(int i=0;i<m;i++)
      for(int j=0;j<=n;j++)
	read(A[i][j]);
		
    if((rank = rowReduction(A,m,n+1)) < 0){
      cout << "No Solution." << endl;
    } else {
      if(rank != n){
	cout << "Infinitely many solutions containing " << n-rank << " arbitrary constants." << endl;
      } else {
	for(int i=0;i<n;i++){
	  cout << "x[" << i+1 << "] = ";print(A[i][n]); cout << endl;
	}
      }
    }
  }
  return 0;
}
