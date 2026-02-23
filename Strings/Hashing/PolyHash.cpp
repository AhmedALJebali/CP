const uint64_t HashMod = (1ULL << 61) - 1;
uint64_t getRandomBase() {
    static mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
    uniform_int_distribution<uint64_t> dist(HashMod / 3, 2 * HashMod / 3);
    return dist(rng);
}
uint64_t base1 = getRandomBase(), base2 = getRandomBase();
inline uint64_t MUL(uint64_t a, uint64_t b) {
    __uint128_t t = (__uint128_t) a * b;
    uint64_t l = (uint64_t) t & HashMod;
    uint64_t h = (uint64_t) (t >> 61);
    uint64_t res = l + h;
    if (res >= HashMod) res -= HashMod;
    return res;
}

struct FastHash {
    int n;
    vector<uint64_t> pow1, pow2, inv1, inv2, pref1, pref2, suff1, suff2;

    static

    uint64_t modPow(uint64_t a, uint64_t e) {
        uint64_t res = 1;
        while (e) {
            if (e & 1) res = MUL(res, a);
            a = MUL(a, a);
            e >>= 1;
        }
        return res;
    }

    FastHash(const string &s) {
        n = s.size();
        pow1.assign(n + 1, 1);
        pow2.assign(n + 1, 1);
        inv1.assign(n + 1, 1);
        inv2.assign(n + 1, 1);
        pref1.assign(n + 1, 0);
        pref2.assign(n + 1, 0);
        suff1.assign(n + 1, 0);
        suff2.assign(n + 1, 0);
        uint64_t invBase1 = modPow(base1, HashMod - 2);
        uint64_t invBase2 = modPow(base2, HashMod - 2);

        for (int i = 1; i <= n; i++) {
            pow1[i] = MUL(pow1[i - 1], base1);
            pow2[i] = MUL(pow2[i - 1], base2);
            inv1[i] = MUL(inv1[i - 1], invBase1);
            inv2[i] = MUL(inv2[i - 1], invBase2);
        }

        // forward hash
        for (int i = 0; i < n; i++) {
            pref1[i] = (i ? pref1[i - 1] : 0) + MUL(s[i], pow1[i]);
            pref1[i] = (pref1[i] >= HashMod ? pref1[i] - HashMod : pref1[i]);
            pref2[i] = (i ? pref2[i - 1] : 0) + MUL(s[i], pow2[i]);
            pref2[i] = (pref2[i] >= HashMod ? pref2[i] - HashMod : pref2[i]);
        }

        // reverse hash
        for (int i = 0; i < n; i++) {
            suff1[i] = (i ? suff1[i - 1] : 0) + MUL(s[n - i - 1], pow1[i]);
            suff1[i] = (suff1[i] >= HashMod ? suff1[i] - HashMod : suff1[i]);
            suff2[i] = (i ? suff2[i - 1] : 0) + MUL(s[n - i - 1], pow2[i]);
            suff2[i] = (suff2[i] >= HashMod ? suff2[i] - HashMod : suff2[i]);
        }
    }

    pair<uint64_t, uint64_t> qry(int l, int r) {
        uint64_t x1 = pref1[r] - (l ? pref1[l - 1] : 0);
        if (x1 >= HashMod) x1 += HashMod;
        x1 = MUL(x1, inv1[l]);

        uint64_t x2 = pref2[r] - (l ? pref2[l - 1] : 0);
        if (x2 >= HashMod) x2 += HashMod;
        x2 = MUL(x2, inv2[l]);

        return {x1, x2};
    }

    pair<uint64_t, uint64_t> revqry(int l, int r) {
        uint64_t x1 = suff1[n - l - 1] - (r + 1 < n ? suff1[n - r - 2] : 0);
        if (x1 >= HashMod) x1 += HashMod;
        x1 = MUL(x1, inv1[n - r - 1]);

        uint64_t x2 = suff2[n - l - 1] - (r + 1 < n ? suff2[n - r - 2] : 0);
        if (x2 >= HashMod) x2 += HashMod;
        x2 = MUL(x2, inv2[n - r - 1]);

        return {x1, x2};
    }

    bool isPalindrome(int l, int r) {
        return qry(l, r) == revqry(l, r);
    }
};
