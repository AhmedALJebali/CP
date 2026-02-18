
## 1. Base Precomputation

To achieve constant-time range queries, precompute the following values based on a base $B$ and a large prime $MOD$:

- **Powers:** $p[i] = B^i \pmod{MOD}$
- **Inverse Powers:** $inv[i] = B^{-i} \pmod{MOD}$ (Calculated using Fermat's Little Theorem as $B^{MOD-2} \pmod{MOD}$)
- **Prefix Hashes:** Cumulative values stored in an array to allow range subtraction.
---
## 2. Two Styles of Hashing

### Style 1: Descending Power (Standard)

The first character of the string is assigned the highest power. This is the traditional "polynomial" representation.

$$pref[i] = s_0 B^{i-1} + s_1 B^{i-2} + \dots + s_{i-1} B^0$$

**Building Prefix Array:**

```cpp
pref[0] = 0;
for (int i = 0; i < n; i++) {
    pref[i+1] = (1LL * pref[i] * base + s[i]) % MOD;
}
```

**Querying Substring $[l, r]$:**

$$H(l, r) = (pref[r+1] - pref[l] \cdot B^{r-l+1}) \pmod{MOD}$$

```cpp
long long getHash(int l, int r) {
    return (pref[r+1] - 1LL * pref[l] * p[r-l+1] % MOD + MOD) % MOD;
}
```

- **Pros:** Does not require modular inverse.
- **Cons:** Requires multiplication by a power of $B$ during every query to align the subtracted prefix.

### Style 2: Ascending Power

The power is directly tied to the index of the character.

$$pref[i] = s_0 B^0 + s_1 B^1 + \dots + s_{i-1} B^{i-1}$$

**Querying Substring $[l, r]$:**

$$H(l, r) = (pref[r+1] - pref[l]) \cdot B^{-l} \pmod{MOD}$$

```cpp
long long getHash(int l, int r) {
    long long res = (pref[r+1] - pref[l] + MOD) % MOD;
    return 1LL * res * inv[l] % MOD;
}
```

- **Pros:** Highly intuitive for reverse hashing and palindrome checks.
- **Cons:** Requires precomputed modular inverses.

---
## 4. Rolling Window (Rabin-Karp)

For a fixed-length window $k$, the hash is updated in $O(1)$ as the window slides.

**Descending Power Update:**

```cpp
// Remove exiting character
cur = (cur - 1LL * s[i-k] * p[k-1] % MOD + MOD) % MOD;
// Shift and add entering character
cur = (1LL * cur * base + s[i]) % MOD;
```

**Ascending Power Update:**

```cpp
// Remove exiting character
cur = (cur - s[i-k] + MOD) % MOD;
// Shift powers down
cur = 1LL * cur * inv[1] % MOD;
// Add entering character at the highest power
cur = (cur + 1LL * s[i] * p[k-1]) % MOD;
```

---

## 5. Set / Multiset Hashing

To compare collections where order does not matter, use a summation-based hash. Assign each unique element a large, random value or a weighted contribution.

```cpp
pair<long long, long long> setHash(const vector<int> &v) {
    long long h1 = 0, h2 = 0;
    for (int x : v) {
        h1 = (h1 + 1LL * x * base1) % MOD1;
        h2 = (h2 + 1LL * x * base2) % MOD2;
    }
    return {h1, h2};
}
```

**Properties:**

- **Order-Independent:** The result is the same regardless of how elements are arranged.
- **Multisets:** Naturally handles frequency (e.g., `{1, 2, 2}` will produce a different hash than `{1, 2}`).
- **Additive:** You can add or remove elements from the set in $O(1)$ by adding or subtracting their weighted values