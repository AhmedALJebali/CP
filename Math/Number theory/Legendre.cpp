/* 
 * Legendre's Formula + Prime Factorization
 * Finds the maximum power of X that perfectly divides K! (How many times X can be formed from K!)
 * Time Complexity: O(sqrt(X) + log p(K)) -> Easily passes massive constraints (e.g., K <= 10^18).
 * 
 * Core Idea: 
 * 1. Break X into its prime factors.
 * 2. For each prime factor, see how many we NEED to build one X.
 * 3. See how many we HAVE in K! using Legendre's Formula.
 * 4. The prime factor that gives the smallest ratio (HAVE / NEED) is our bottleneck.
 */

// Calculates the highest power of a PRIME number 'p' that divides k! using Legendre's Formula
int get_power_in_factorial(int k, int p) {
  int cnt = 0;
  while (k > 0) {
    cnt += k / p;
    k /= p;
  }
  return cnt;
}
// Calculates the highest power of ANY number 'x' (prime or composite) that divides k!
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
pair<int, vector<int>> extract_single_prime(int L, int R, int p) {
  int n = R - L + 1;
  vector<int> seq(n);
  for (int i = 0; i < n; i++) {
    seq[i] = L + i;
  }
  int tot = 0;
  int st = (p - (L % p)) % p;
  for (int i = st; i < n; i += p) {
    while (seq[i] % p == 0) {
      tot++;
      seq[i] /= p; 
    }
  }
  return {tot, seq};
}
