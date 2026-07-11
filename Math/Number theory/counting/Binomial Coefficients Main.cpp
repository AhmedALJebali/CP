// ===================== CONSTANTS =====================
const int N = 1e6 + 5; // Max n for factorials/modulo
const int MOD = 1e9 + 7; // Modulo for contests


// ===================== FACTORIALS & INVERSE =====================
int fact[N], inv_fact[N];

// Fast exponentiation modulo m
int modPow(int a, int e, int m = MOD) {
    int res = 1;
    a %= m;
    while (e) {
        if (e & 1) res = (1LL * res * a) % m;
        a = (1LL * a * a) % m;
        e >>= 1;
    }
    return res;
}

// Precompute factorials and inverse factorials modulo MOD
void preFactorial() {
    fact[0] = 1;
    for (int i = 1; i < N; i++)
        fact[i] = 1LL * fact[i - 1] * i % MOD;
    inv_fact[N - 1] = modPow(fact[N - 1], MOD - 2);
    for (int i = N - 2; i >= 0; i--)
        inv_fact[i] = 1LL * inv_fact[i + 1] * (i + 1) % MOD;
}

// nCr modulo MOD (O(1) using precomputed factorials)
int nCrMod(int n, int r) {
    if (r < 0 || r > n) return 0;
    return 1LL * fact[n] * inv_fact[r] % MOD * inv_fact[n - r] % MOD;
}

// nPr modulo MOD
int nPrMod(int n, int r) {
    if (r < 0 || r > n) return 0;
    return 1LL * fact[n] * inv_fact[n - r] % MOD;
}
int nCr_large_n(int n, int r) {
  if (r < 0 || r > n) return 0;
  int num = 1;
  for (int i = 0; i < r; i++) {
    num = (num * ((n - i) % MOD)) % MOD;
  }
  return (num * inv_fact[r]) % MOD;
}
int count_permutations(const vector<int>& box_capacities, int k) {
  vector<int> dp(k + 1, 0);
  dp[0] = 1;
  for (int capacity : box_capacities) {
    vector<int> next_dp(k + 1, 0);
    for (int j = 0; j <= k; ++j) {
      int limit = min(capacity, j);
      for (int x = 0; x <= limit; ++x) {
        next_dp[j] += dp[j - x] * nCrMod(j,x);
      }
    }
    dp = next_dp;
  }
  return dp[k];
}
