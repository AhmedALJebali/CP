// Returns the number of derangements of 'n' items (permutations where no element remains in its original position).
int getDerangement(int n) {
  if (n == 0) return 1;
  if (n == 1) return 0;
  int d0 = 1, d1 = 0,ret = 0;
  for (int i = 2; i <= n; i++) {
    ret = (i - 1) * (d1 + d0);
    d0 = d1 , d1 = ret;
  }
  return ret;
}
