template<int MOD>
struct PowerTable {
    vector<int> pow, inv;
    int base;

    // Generate a random base in a safe range
    static int getRandomBase() {
        static mt19937 rng(
            chrono::steady_clock::now().time_since_epoch().count()
        );
        uniform_int_distribution<int> dist(256, 1000000); // safe primes
        return dist(rng);
    }

    // Modular exponentiation
    int modPow(int a, int b) {
        int res = 1;
        while (b) {
            if (b & 1) res = (1LL * res * a) % MOD;
            a = (1LL * a * a) % MOD;
            b >>= 1;
        }
        return res;
    }

    // Constructor: precompute powers and inverses up to size n
    PowerTable(int n) {
        base = getRandomBase(); // automatically choose base
        pow.assign(n + 1, 1);
        inv.assign(n + 1, 1);
        int invBase = modPow(base, MOD - 2); // base^-1 mod MOD

        for (int i = 1; i <= n; i++) {
            pow[i] = (1LL * pow[i - 1] * base) % MOD;
            inv[i] = (1LL * inv[i - 1] * invBase) % MOD;
        }
    }

    // Get base^k
    int getPow(int k) const { return pow[k]; }

    // Get base^-k
    int getInv(int k) const { return inv[k]; }
};
