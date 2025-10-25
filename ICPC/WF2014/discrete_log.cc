// Discrete Log Solver -- O(sqrt(p))
ll discrete_log(ll p,ll b,ll n){
  map<ll,ll> M; ll jump = ceil(sqrt(p));
  for(int i=0;i<jump && i<p;i++) M[fast_exp_mod(b,i,p)] = i+1;
  for(int i=0;i<p-1;i+=jump){
    ll x = (n*fast_exp_mod(b,p-i-1,p)) % p;
    if(M.find(x) != M.end()) return (i+M[x]-1) % (p-1);
  }
  return -1;
}
