const uint64_t HashMod = (1ULL << 61) - 1;
const uint64_t seed = chrono::system_clock::now().time_since_epoch().count();
const uint64_t base = mt19937_64(seed)() % (HashMod / 3) + (HashMod / 3);

int64_t MUL(uint64_t a, uint64_t b) {
    uint64_t l1 = (uint32_t) a, h1 = a >> 32, l2 = (uint32_t) b, h2 = b >> 32;
    uint64_t l = l1 * l2, m = l1 * h2 + l2 * h1, h = h1 * h2;
    uint64_t ret = (l & HashMod) + (l >> 61) + (h << 3) + (m >> 29) + (m << 35 >> 3) + 1;
    ret = (ret & HashMod) + (ret >> 61);
    ret = (ret & HashMod) + (ret >> 61);
    return (int64_t) ret - 1;
}

struct PolyHash61 {
    static const uint64_t HashMod = (1ULL << 61) - 1;
    uint64_t base1, base2;
    int n;
    vector<uint64_t> pow1, pow2, pref1, pref2, inv1, inv2, suff1, suff2;

    static uint64_t getRandomBase() {
        static mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
        uniform_int_distribution<uint64_t> dist(HashMod / 3, 2 * HashMod / 3);
        return dist(rng);
    }

    uint64_t modPow(uint64_t a, uint64_t e) {
        uint64_t res = 1;
        while (e) {
            if (e & 1) res = MUL(res, a);
            a = MUL(a, a);
            e >>= 1;
        }
        return res;
    }

    PolyHash61(const string &s) {
        n = s.size();
        base1 = getRandomBase(),
                base2 = getRandomBase(), pow1.assign(n + 1, 1), pow2.assign(n + 1, 1), inv1.assign(n + 1, 1), inv2.
                assign(n + 1, 1), pref1.assign(n + 1, 0), pref2.assign(n + 1, 0), suff1.assign(n + 1, 0), suff2.assign(
                    n + 1, 0);

        uint64_t invBase1 = modPow(base1, HashMod - 2);
        uint64_t invBase2 = modPow(base2, HashMod - 2);

        for (int i = 1; i <= n; i++) {
            pow1[i] = MUL(pow1[i - 1], base1);
            pow2[i] = MUL(pow2[i - 1], base2);
            inv1[i] = MUL(inv1[i - 1], invBase1);
            inv2[i] = MUL(inv2[i - 1], invBase2);
        }
        // Reverse hash
        // Forward hash
        pref1[0] = s[0], pref2[0] = s[0], suff1[0] = MUL(s[0], pow1[n - 1]), suff2[0] = MUL(s[0], pow2[n - 1]);
        for (int i = 1; i < n; i++) {
            pref1[i] = (pref1[i - 1] + MUL(s[i], pow1[i])) % HashMod;
            pref2[i] = (pref2[i - 1] + MUL(s[i], pow2[i])) % HashMod;
            suff1[i] = (suff1[i - 1] + MUL(s[i], pow1[n - i - 1])) % HashMod;
            suff2[i] = (suff2[i - 1] + MUL(s[i], pow2[n - i - 1])) % HashMod;
        }
    }

    pair<uint64_t, uint64_t> getHash(int l, int r) {
        uint64_t x1 = pref1[r] - (l ? pref1[l - 1] : 0);
        uint64_t x2 = pref2[r] - (l ? pref2[l - 1] : 0);
        x1 = (MUL(x1, inv1[l]) + HashMod) % HashMod;
        x2 = (MUL(x2, inv2[l]) + HashMod) % HashMod;
        return {x1, x2};
    }

    pair<uint64_t, uint64_t> getReverseHash(int l, int r) {
        uint64_t x1 = suff1[r] - (l ? suff1[l - 1] : 0);
        uint64_t x2 = suff2[r] - (l ? suff2[l - 1] : 0);
        x1 = (MUL(x1, inv1[n - r - 1]) + HashMod) % HashMod;
        x2 = (MUL(x2, inv2[n - r - 1]) + HashMod) % HashMod;
        return {x1, x2};
    }

    bool isPalindrome(int l, int r) {
        return getHash(l, r) == getReverseHash(l, r);
    }
};