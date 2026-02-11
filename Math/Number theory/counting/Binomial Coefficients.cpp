int dp[N][N];
// Pascal Triangle
void pre() {
  for (int n = 0; n < N; n++) {
    dp[n][0] = dp[n][n] = 1;
    for (int r = 1; r < n; r++) {
      dp[n][r] = dp[n - 1][r - 1] + dp[n - 1][r];
    }
  }
}

int f[N], inv[N], finv[N];

void pre1() {
  f[0] = 1;
  for (int n = 1; n < N; n++)f[n] = f[n - 1] * n % MOD;
  inv[1] = 1;
  for (int i = 2; i < N; i++) {
    inv[i] = (-(1LL * MOD / i) * inv[MOD % i]) % MOD;
    inv[i] = (inv[i] + MOD) % MOD;
  }
  finv[0] = 1;
  for (int i = 1; i < N; i++) finv[i] = 1LL * inv[i] * finv[i - 1] % MOD;
}

int ncr(int n, int r) {
  if (n < r || n < 0 || r < 0) return 0;
  return 1LL * f[n] * finv[n - r] % MOD * finv[r] % MOD;
}
///////////////////////////////////////////////// nCr

int C(int n, int k) {
    int res = 1;
    for (int i = n - k + 1; i <= n; ++i) // n! / (n - k)!
        res *= i;
    for (int i = 2; i <= k; ++i)// / k!
        res /= i;
    return res;
}

///////////////////////////////////////////////// nCr

int C(int n, int k) { // O(1e6)
    double res = 1;
    for (int i = 1; i <= k; ++i)
        res = res * (n - k + i) / i;
    return (int)(res + 0.01);
}
int fact[N], inv_fact[N];

// fast exponentiation
int modPow(int a, int e, int m) {
    int res = 1;
    a %= m;
    while (e) {
        if (e & 1) res = (res * a) % m;
        a = (a * a) % m;
        e >>= 1;
    }
    return res;
}

// precompute factorials
void precompute() {
    fact[0] = 1;
    for (int i = 1; i < N; i++)
        fact[i] = fact[i-1] * i % MOD;
    
    inv_fact[N-1] = modPow(fact[N-1], MOD-2, MOD);
    for (int i = N-2; i >= 0; i--)
        inv_fact[i] = inv_fact[i+1] * (i+1) % MOD;
}

// nCr modulo MOD
int nCr(int n, int r) {
    if (r < 0 || r > n) return 0;
    return fact[n] * inv_fact[r] % MOD * inv_fact[n-r] % MOD;
}