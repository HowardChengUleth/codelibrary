// RMQ:
//   call constructRMQ to get data structure M                   O(n)
//   call getmax to get the maximum from [a..b] inclusive        O(log n)
//     returns a pair of the form (maximum value,index of maximum value)
//   call update to change a value in the array                  O(log n)
typedef int Type;             // Change this as needed
const Type oo = INT_MIN;      // Change to INT_MAX for min.

typedef pair<Type,int> pti;
const pti p_oo = make_pair(oo,-1);
int size;

void constructRMQ(Type A[MAX_N], pti M[4*MAX_N], int n) {
  size = 1; while(size < n) size <<= 1;
  for (int i=0; i < size; i++) M[size-1+i] = (i < n ? make_pair(A[i],i) : p_oo);
  for (int i=size-2; i>=0; i--) M[i] = max(M[2*i+1],M[2*i+2]);
}

pti getmax(pti M[4*MAX_N], int a, int b, int st=0,int en=size,int ind=0) {
  if (a > b || a >= en || b < st)         return p_oo;
  if ((a <= st && en <= b) || st+1 >= en) return M[ind];
  int mid = (st+en)/2;
  return max(getmax(M,a,b,st,mid,2*ind+1), getmax(M,a,b,mid,en,2*ind+2));
}

void update(Type A[MAX_N], pti M[4*MAX_N], int i, Type val){
  A[i] = val; M[i += size-1] = make_pair(val,i);
  while((i = (i-1)/2)) M[i] = max(M[2*i+1],M[2*i+2]);
}
