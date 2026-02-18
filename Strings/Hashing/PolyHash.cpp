
struct PolyHash {
    static const int mod1 = 1000000007;
    static const int mod2 = 1000000009;

    int base;
    int n;
    vector<int> pow1, pow2;
    vector<int> inv1, inv2;
    vector<int> pref1, pref2;
    vector<int> suff1, suff2;

    static int getRandomBase() {
        static mt19937 rng(
            chrono::steady_clock::now().time_since_epoch().count()
        );
        uniform_int_distribution<int> dist(256, 1000000);
        return dist(rng);
    }

    int modPow(long long a, int b, int mod) {
        long long res = 1;
        while (b) {
            if (b & 1) res = (res * a) % mod;
            a = (a * a) % mod;
            b >>= 1;
        }
        return (int) res;
    }

    PolyHash(string &s) {
        base = getRandomBase();
        n = s.size();
        pow1.assign(n + 1, 0);
        pow2.assign(n + 1, 0);
        inv1.assign(n + 1, 0);
        inv2.assign(n + 1, 0);
        pref1.assign(n + 1, 0);
        pref2.assign(n + 1, 0);
        suff1.assign(n + 1, 0);
        suff2.assign(n + 1, 0);
        int invBase1 = modPow(base, mod1 - 2, mod1);
        int invBase2 = modPow(base, mod2 - 2, mod2);
        pow1[0] = pow2[0] = 1;
        inv1[0] = inv2[0] = 1;
        for (int i = 1; i <= n; i++) {
            pow1[i] = 1LL * pow1[i - 1] * base % mod1;
            pow2[i] = 1LL * pow2[i - 1] * base % mod2;
            inv1[i] = 1LL * inv1[i - 1] * invBase1 % mod1;
            inv2[i] = 1LL * inv2[i - 1] * invBase2 % mod2;
        }
        // Reverse hash (descending powers)
        // Forward hash (ascending powers)
        pref1[0] = s[0], pref2[0] = s[0];
        suff1[0] = 1LL * s[0] * pow1[n - 1] % mod1;
        suff2[0] = 1LL * s[0] * pow2[n - 1] % mod2;
        for (int i = 1; i < n; i++) {
            pref1[i] = (pref1[i - 1] + 1LL * s[i] * pow1[i]) % mod1;
            pref2[i] = (pref2[i - 1] + 1LL * s[i] * pow2[i]) % mod2;

            suff1[i] = (suff1[i - 1] + 1LL * s[i] * pow1[n - i - 1]) % mod1;
            suff2[i] = (suff2[i - 1] + 1LL * s[i] * pow2[n - i - 1]) % mod2;
        }
    }

    // a*0 + b*1
    pair<int,int> getHash(int l, int r) {
        int x1 = pref1[r] - (l ? pref1[l - 1] : 0);
        int x2 = pref2[r] - (l ? pref2[l - 1] : 0);
        return {
            ((x1 * inv1[l]) % mod1 + mod1) % mod1,
            ((x2 * inv2[l]) % mod2 + mod2) % mod2
        };
    }

    // a*1 + b*0
    pair<int,int> getReverseHash(int l, int r) {
        int x1 = suff1[r] - (l ? suff1[l - 1] : 0);
        int x2 = suff2[r] - (l ? suff2[l - 1] : 0);
        return {
            ((x1 * inv1[n - r - 1]) % mod1 + mod1) % mod1,
            ((x2 * inv2[n - r - 1]) % mod2 + mod2) % mod2
        };
    }

    bool isPalindrome(int l, int r) {
        return getHash(l, r) == getReverseHash(l, r);
    }
};