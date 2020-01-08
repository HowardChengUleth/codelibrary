// max sum is in [start,end]
int vecsum(int v[], int n, int &start, int &end)
{
  int maxval = 0, max_end = 0, max_end_start, max_end_end;
  start = max_end_start = 0;       end = max_end_end = -1;
  for (int i = 0; i < n; i++) {
    if (v[i] + max_end >= 0) {  max_end = v[i] + max_end;    max_end_end = i;
    } else { max_end_start = i+1;    max_end_end = -1;    max_end = 0; }


    if (maxval < max_end) {
      start = max_end_start;   end = max_end_end;    maxval = max_end;
    } else if (maxval == max_end) {    }  /* tie-breaking here */
  }
  return maxval;
}
