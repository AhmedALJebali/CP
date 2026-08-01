// Legendre's Formula + Prime Factorization: Finds the max power of X that perfectly divides K!
// Time Complexity: O(sqrt(X) + log(K)) -> Easily passes massive constraints (e.g., K <= 10^18).
int get_power_in_factorial(int k, int p) {
  int cnt = 0;
  while (k > 0) {
    cnt += k / p;
    k /= p;
  }
  return cnt;
}
int get_power_of_x_in_factorial(int k, int x) {
  if (x <= 1) return 0;
  int mn = LLONG_MAX;
  for (int i = 2; i * i <= x; ++i) {
    if (x % i == 0) {
      int cntx = 0;
      while (x % i == 0) {
        cntx++;
        x /= i;
      }
      int cntk = get_power_in_factorial(k, i);
      mn = min(mn, cntk / cntx);
    }
  }
  if (x > 1) {
    int cntk = get_power_in_factorial(k, x);
    mn = min(mn, cntk / 1);
  }
  if (mn == LLONG_MAX) return 0;
  return mn;
}
