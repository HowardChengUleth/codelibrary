void db(int t,int p,int k,int n,vector<int>& seq,vector<int>& a){
  if(t > n){ if(n % p == 0) for(int j=1;j<=p;j++) seq.push_back(a[j]);
  } else {
    a[t]=a[t-p]; db(t+1,p,k,n,seq,a);
    for(int j=a[t-p]+1;j<k;j++) a[t]=j, db(t+1,t,k,n,seq,a);
  }
}

// PIN numbers without OK.
vector<int> de_bruijn(int k,int n){ // Alphabet size = k, subsequence length = n
  vector<int> a(k*n,0),seq; seq.reserve(pow(k,n));
  db(1,1,k,n,seq,a); return seq;
}
