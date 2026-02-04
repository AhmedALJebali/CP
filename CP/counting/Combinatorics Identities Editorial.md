
> [!ABSTRACT] Overview
> 
> This note serves as a cheat sheet for binomial identities and counting principles.
> 
---

## 1. The Binomial Coefficient

The fundamental building block of counting.

**Definition:**

$$\binom{n}{k} = \frac{n!}{k!(n-k)!}$$

> [!TIP] Computation in CP
> 
> When $n$ is large, we precompute factorials and their modular inverses. For $n, k \le 10^7$ and prime $M$:
> 
> `nCr = (fact[n] * invFact[k] % M) * invFact[n-k] % M`

---

## 2. Symmetry & Pascal's Identity

### Symmetry

$$\binom{n}{k} = \binom{n}{n-k}$$

**Intuition:** Choosing $k$ items to "keep" is the same as choosing $n-k$ items to "throw away."

### Pascal's Identity

$$\binom{n}{k} = \binom{n-1}{k} + \binom{n-1}{k-1}$$

**Intuition:** Focus on one specific item. Either you **don't** pick it (choose $k$ from the remaining $n-1$) or you **do** pick it (choose $k-1$ from the remaining $n-1$).

---

## 3. Summation Identities

### Power of Two

$$\sum_{k=0}^{n} \binom{n}{k} = 2^n$$

**Intuition:** This represents the total number of subsets of a set of size $n$. Every element has 2 choices: "In" or "Out."

### Weighted Sum

$$\sum_{k=0}^{n} k \binom{n}{k} = n \cdot 2^{n-1}$$

**Intuition:** If you form every possible committee and count the total number of members across all committees, this is the result.

---

## 4. Vandermonde’s Identity

$$\sum_{k=0}^{r} \binom{n}{k} \binom{m}{r-k} = \binom{n+m}{r}$$

> [!EXAMPLE] Story
> 
> You have $n$ boys and $m$ girls. You want to pick a committee of $r$ people. This formula sums up the cases where you pick $k$ boys and $r-k$ girls.

---

## 5. Multinomial Coefficient

Used when you aren't just picking "in or out," but sorting $n$ items into multiple distinct groups of sizes $k_1, k_2, \dots, k_m$.

**Formula:**

$$\binom{n}{k_1, k_2, \dots, k_m} = \frac{n!}{k_1!k_2!\dots k_m!}$$

_Constraint: $\sum k_i = n$_

---

## 6. Stars and Bars (Balls and Urns)

One of the most frequent CP patterns.

**Problem:** Number of ways to put $k$ identical items into $n$ distinct bins.

**Non-negative integer solutions ($x_i \ge 0$):**

$$\binom{n+k-1}{k} \quad \text{or} \quad \binom{n+k-1}{n-1}$$

> [!IMPORTANT] Variation
> 
> If each bin must have **at least one** item ($x_i \ge 1$), use:
> 
> $$\binom{k-1}{n-1}$$

---

## 7. Inclusion–Exclusion Principle (PIE)

Used to count the size of the union of multiple overlapping sets.

$$\left| \bigcup_{i=1}^n A_i \right| = \sum |A_i| - \sum |A_i \cap A_j| + \sum |A_i \cap A_j \cap A_k| - \dots$$

---

## 8. Catalan Numbers

A sequence that appears in many recursive structures.

$$C_n = \frac{1}{n+1} \binom{2n}{n}$$

**Common Applications:**

1. Number of valid parenthesis strings with $n$ pairs.
    
2. Number of ways to triangulate a polygon with $n+2$ sides.
    
3. Number of binary trees with $n$ nodes.
    

---

## 📊 Summary Table

|**Identity**|**Best Used For...**|
|---|---|
|**Symmetry**|Reducing $k$ when $k > n/2$.|
|**Pascal**|DP-based combinations when $N$ is small ($\approx 5000$).|
|**Stars & Bars**|Distributing tasks, resources, or candy.|
|**Vandermonde**|Simplifying sums involving two binomials.|
|**PIE**|Problems with "at least one" or "none of" constraints.|
|**Catalan**|Balanced structures and non-crossing paths.|

---

## 💡 Implementation Checklist for CP

- [ ] **Modular Inverse:** Use Fermat's Little Theorem $a^{M-2} \pmod M$ for prime $M$.
    
- [ ] **Precomputation:** Precompute `fact[]` and `invFact[]` in $O(N)$ time.
    
- [ ] **Corner Cases:** Always handle $n < k$ (return 0) and $k < 0$ (return 0).
    
- [ ] **Lucas Theorem:** If $n, k$ are huge but $M$ is a small prime, use Lucas Theorem.
    

Would you like me to generate a **template code for precomputing nCr** in C++ or Python to add to your notes?