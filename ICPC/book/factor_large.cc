// To factor large numbers (x >= 2^40):
//  - Check all primes up to CUBE_ROOT(x) via trial division
//     -- At this point, x has AT MOST 2 unknown prime divisors
//  - Check if remaining value is perfect square (ll(sqrt(x))*ll(sqrt(x)) == x)
//  - Check if remaining value is prime (is_probable_prime(x,20))
//  - Find a prime divisor (using q=pollardRho(x))
//     -- q and x/q are the factors

typedef long long int ll;

// Helper functions...
ll q_mod(ll x,ll m){ return (x >= m) ? x-m : x; }
ll mult_mod(ll x,ll y,ll m){ // Use int128
  ll r = 0;
  while(y){
    if(y % 2) r = q_mod(r+x,m);
    y >>= 1; x = q_mod(x << 1,m);
  } return r;
}

ll F(ll x,ll n,ll c){ x=mult_mod(x,x,n)-c; return (x < 0 ? x + n : x); }

// Returns one (not necessarily prime) factor of n.
//  Works best on semi-primes (where n = p*q for distinct primes).
// Does not work well on perfect powers -- check for those separately.
ll pollardRho(ll n){
  ll i,c,b,x,y,z,g;
  for(g=0,c=3; g % n == 0 ;c++)
    for(g=b=x=y=z=1 ; g==1 ; b *= 2,g = gcd(z,n),z = 1, y = x)
      for(i=0;i<b;i++){ x = F(x,n,c); z = mult_mod(z,abs(x-y),n); }
  return g;
}

// Works for any long long. Do some trial division. Pick an appropriate val:
const val[] = {2,7,61};                          // n <= 2^32
const val[] = {2,13,23,1662803};                 // n <= 10^12
const val[] = {2,3,5,7,11,13,17,19,23,29,31,37}; // n <= 2^64
bool is_prime(ll n){
  if(n < 2) return false;
  for(int i=0;i<NUM_SMALL_PRIMES;i++) if(n % pr[i] == 0) return n == pr[i];
  
  ll s = __builtin_ctzll(n-1), d = (n-1) >> s;
  for(int i=0;i<NUM_ENTRIES_IN_VAL;i++){
    if(val[i] >= n) break;
    ll x = fast_exp_mod(val[i],d,n); // Use int128 in here
    if(x == 1 || x == n-1) continue;
    for(ll r=1;r<s;r++) if((x = mult_mod(x,x,n)) == n-1) goto nextPr;
    return false;
    nextPr:;
  }
  return true;
}
