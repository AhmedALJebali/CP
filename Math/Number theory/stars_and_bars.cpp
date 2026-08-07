// Σ_{i=0}^{k} C(i+r,r) = C(k+r+1,r+1) (Hockey-Stick Identity)
// Use for prefix sums of binomial coefficients / Stars & Bars (e.g. x1+...+xm ≤ k).
int hockey(int k, int r) { return (k < 0 ? 0 : nCrMod(k + r + 1, r + 1)); }
// Distribute N items into K boxes, each box MUST have AT LEAST x items
int stars_and_bars_at_least_x(int n, int k, int x) {
  if (n < k * x) return 0;
  int remaining = n - (k * x);
  if (k == 0 && remaining == 0) return 1;
  if (k == 0) return 0;
  return nCrMod(remaining + k - 1, k - 1);
}
// Distribute N items into K boxes
int stars_and_bars_non_negative(int n, int k) {
  if (k == 0 && n == 0) return 1;
  if (k == 0) return 0;
  return nCrMod(n + k - 1, k - 1);
}
// Distribute N items into K boxes, each box MUST have AT LEAST x and AT MOST y items
int stars_and_bars_bounded(int n, int k, int x, int y) {
  if (n < k * x || n > k * y) return 0;
  int remaining_items = n - (k * x);
  int limit = (y - x) + 1;
  int ans = 0;
  for (int j = 0; j <= k; j++) {
    int items_left = remaining_items - (j * limit);
    if (items_left < 0) break;
    int choose_violators = nCrMod(k, j);
    int distribute_rest = nCrMod(items_left + k - 1, k - 1);
    int current_ways = (choose_violators * distribute_rest) % MOD;
    if (j % 2 == 0) {
      ans = (ans + current_ways) % MOD;
    } else {
      ans = (ans - current_ways + MOD) % MOD;
    }
  }
  return ans;
}
int count_Subseq_Sum_Equal_S(int s) {
  return modPow(2,s-1);
}

