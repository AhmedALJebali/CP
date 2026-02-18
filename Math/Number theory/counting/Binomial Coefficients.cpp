// ===================== CONSTANTS =====================
const int N = 1e6 + 5;    // Max n for factorials/modulo
const int MOD = 1e9 + 7;  // Modulo for contests

// ===================== PASCAL TRIANGLE =====================
// Small nCr without modulo (n <= 1000)
int dp[1005][1005];
void prePascal() {
    for (int n = 0; n <= 1000; n++) {
        dp[n][0] = dp[n][n] = 1;
        for (int r = 1; r < n; r++)
            dp[n][r] = dp[n-1][r-1] + dp[n-1][r];
    }
}
int nCrPascal(int n, int r) {
    if (r < 0 || r > n) return 0;
    return dp[n][r];
}

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
        fact[i] = 1LL * fact[i-1] * i % MOD;

    inv_fact[N-1] = modPow(fact[N-1], MOD-2);
    for (int i = N-2; i >= 0; i--)
        inv_fact[i] = 1LL * inv_fact[i+1] * (i+1) % MOD;
}

// nCr modulo MOD (O(1) using precomputed factorials)
int nCrMod(int n, int r) {
    if (r < 0 || r > n) return 0;
    return 1LL * fact[n] * inv_fact[r] % MOD * inv_fact[n-r] % MOD;
}

// nPr modulo MOD
int nPrMod(int n, int r) {
    if (r < 0 || r > n) return 0;
    return 1LL * fact[n] * inv_fact[n-r] % MOD;
}

// ===================== DOUBLE PRECISION METHOD =====================
// Approximate nCr using double (n <= 1e6)
int nCrDouble(int n, int r) {
    if (r < 0 || r > n) return 0;
    double res = 1;
    for (int i = 1; i <= r; i++)
        res = res * (n - r + i) / i;
    return (int)(res + 0.01);
}

// ===================== DIRECT COMPUTATION =====================
// Direct computation without modulo (careful with large n)
long long nCrDirect(long long n, long long r) {
    if (r < 0 || r > n) return 0;
    if (r > n - r) r = n - r; // symmetry
    long long res = 1;
    for (long long i = 1; i <= r; i++) {
        res = res * (n - r + i) / i;
    }
    return res;
}
