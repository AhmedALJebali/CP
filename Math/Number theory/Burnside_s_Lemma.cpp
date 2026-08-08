// ==========================================
// 0. Core Math Helpers
// ==========================================
int power(int base, int exp) {
    int res = 1; base %= MOD; if (base < 0) base += MOD;
    while (exp > 0) {
        if (exp & 1) res = (res * base) % MOD;
        base = (base * base) % MOD; exp >>= 1;
    }
    return res;
}
int modInverse(int n) { n %= MOD; if (n < 0) n += MOD; return power(n, MOD - 2); }

int get_phi(int n) {
    int res = n;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            while (n % i == 0) n /= i;
            res -= res / i;
        }
    }
    if (n > 1) res -= res / n;
    return res;
}

// Mobius function mu(n): 0 if n has a squared prime factor, else (-1)^k
// for k distinct prime factors. Needed for aperiodic-necklace / Lyndon
// word counting.
int get_mobius(int n) {
    int res = 1;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            n /= i;
            if (n % i == 0) return 0; // squared factor
            res = -res;
        }
    }
    if (n > 1) res = -res;
    return res;
}

int cp_gcd(int a, int b) { return b == 0 ? a : cp_gcd(b, a % b); }

// nCr mod p (small n, r) via direct product -- used by exact-count Polya.
int nCr_mod(int n, int r) {
    if (r < 0 || r > n) return 0;
    r = min(r, n - r);
    int num = 1, den = 1;
    for (int i = 0; i < r; i++) {
        num = (num * ((n - i) % MOD)) % MOD;
        den = (den * ((i + 1) % MOD)) % MOD;
    }
    return (num * modInverse(den)) % MOD;
}



// ==========================================
// 1D: Cycles and Rings (rotations / dihedral)
// ==========================================
// Number of necklaces of length N using C colors, up to ROTATION only.
int solve_1D_Necklace(int N, int C) {
    int ans = 0;
    for (int d = 1; d * d <= N; d++) {
        if (N % d == 0) {
            ans = (ans + power(C, d) * (get_phi(N / d) % MOD)) % MOD;
            int d2 = N / d;
            if (d2 != d) {
                ans = (ans + power(C, d2) * (get_phi(N / d2) % MOD)) % MOD;
            }
        }
    }
    return (ans * modInverse(N)) % MOD;
}
// Number of bracelets of length N using C colors, up to ROTATION + REFLECTION
int solve_1D_Bracelet(int N, int C) {
    int ans = (solve_1D_Necklace(N, C) * (N % MOD)) % MOD; // sum over rotations
    int MOD_phi = MOD - 1;
    int reflection_ans = 0;

    if (N % 2 == 1) {
        int E = ((N + 1) / 2) % MOD_phi;
        reflection_ans = (N % MOD) * power(C, E) % MOD;
    } else {
        int E1 = ((N + 2) / 2) % MOD_phi;
        int E2 = (N / 2) % MOD_phi;
        int half_N = (N / 2) % MOD;
        reflection_ans = (half_N * power(C, E1)) % MOD;
        reflection_ans = (reflection_ans + half_N * power(C, E2)) % MOD;
    }
    ans = (ans + reflection_ans) % MOD;
    return (ans * modInverse((2 * (N % MOD)) % MOD)) % MOD;
}
// Number of BINARY necklaces (C=2)
int count_binary_necklaces(int N) { return solve_1D_Necklace(N, 2); }
// it counts only necklaces whose minimal period is exactly N
int count_aperiodic_necklaces(int N, int C) {
    int ans = 0;
    for (int d = 1; d * d <= N; d++) {
        if (N % d == 0) {
            int mu_d = get_mobius(d);
            if (mu_d != 0) ans = (ans + mu_d * power(C, N / d) % MOD + MOD) % MOD;
            int d2 = N / d;
            if (d2 != d) {
                int mu_d2 = get_mobius(d2);
                if (mu_d2 != 0) ans = (ans + mu_d2 * power(C, N / d2) % MOD + MOD) % MOD;
            }
        }
    }
    return (ans * modInverse(N)) % MOD;
}
// Returns the number of distinct N-bead binary necklaces that contain exactly k beads of the first color
int count_binary_necklaces_exact_k(int N, int k) {
    int g = cp_gcd(N, k);
    int ans = 0;
    for (int d = 1; d * d <= g; d++) {
        if (g % d == 0) {
            ans = (ans + (get_phi(d) % MOD) * nCr_mod(N / d, k / d)) % MOD;
            int d2 = g / d;
            if (d2 != d) ans = (ans + (get_phi(d2) % MOD) * nCr_mod(N / d2, k / d2)) % MOD;
        }
    }
    return (ans * modInverse(N)) % MOD;
}
// Returns the number of valid ways to color an N-vertex circular ring using C colors such that no two adjacent vertices share the same color
int count_cycle_proper_colorings(int N, int C) {
    int term = power(C - 1, N);
    int sign = (N % 2 == 0) ? 1 : -1;
    int ans = (term + sign * ((C - 1) % MOD) % MOD + MOD) % MOD;
    return ans;
}
// Returns the number of valid ways to color a straight path (line) using C colors such that no two adjacent vertices share the same color
int count_path_proper_colorings(int N, int C) {
    if (N <= 0) return 1;
    return (C % MOD) * power(C - 1, N - 1) % MOD;
}
// Returns the 0-based starting index of the lexicographically smallest cyclic rotation of the given string
int least_rotation(const string& s) {
    string t = s + s;
    int n = t.size(), i = 0, j = 1, k = 0;
    while (i < s.size() && j < s.size() && k < n) {
        char a = t[i + k], b = t[j + k];
        if (a == b) { k++; continue; }
        if (a > b) i += k + 1; else j += k + 1;
        if (i == j) j++;
        k = 0;
    }
    return min(i, j);
}
// Returns the unique sequence of Lyndon words into which the given string can be factorized.
vector<string> duval_lyndon_factorization(const string& s) {
    int n = s.size(), i = 0;
    vector<string> factors;
    while (i < n) {
        int j = i + 1, k = i;
        while (j < n && s[k] <= s[j]) {
            if (s[k] < s[j]) k = i; else k++;
            j++;
        }
        while (i <= k) { factors.push_back(s.substr(i, j - k)); i += j - k; }
    }
    return factors;
}

// ==========================================
// 2D: Grids and Matrices
// ==========================================

// Grid rotations only (C4 if square, else {id, 180}).
int solve_2D_Grid_Rotations(int N, int M, int C) {
    int MOD_phi = MOD - 1;
    unsigned int S = (unsigned int)N * M;

    if (N == M) {
        int E1 = S % MOD_phi;
        int E2 = ((S + 1) / 2) % MOD_phi;
        int E3 = ((S + 3) / 4) % MOD_phi;
        int ans = (power(C, E1) + power(C, E2) + 2 * power(C, E3)) % MOD;
        return (ans * modInverse(4)) % MOD;
    } else {
        int E1 = S % MOD_phi;
        int E2 = ((S + 1) / 2) % MOD_phi;
        int ans = (power(C, E1) + power(C, E2)) % MOD;
        return (ans * modInverse(2)) % MOD;
    }
}
