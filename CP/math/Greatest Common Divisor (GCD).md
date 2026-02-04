
> [!ABSTRACT]
> 
> The **Greatest Common Divisor (GCD)** is the largest positive integer that divides two or more integers without a remainder. It is the fundamental building block of number theory in competitive programming.

---

## 1. Definition & Visual Intuition

Mathematically, for integers $a$ and $b$:

$$\gcd(a, b) = \max \{ d \in \mathbb{Z}^+ : d \mid a \text{ and } d \mid b \}$$

###  The Tiling Analogy

Think of the GCD as the **largest square tile** that can perfectly cover an $a \times b$ rectangle without gaps or overlaps. For a $12 \times 18$ rectangle, the largest possible square tile is $6 \times 6$.

---

## 2. Essential Properties

These properties allow you to manipulate GCD within complex equations or optimize algorithms.

### A. Arithmetic & Transformation

- **Commutativity:** $\gcd(a, b) = \gcd(b, a)$
- **Associativity:** $\gcd(a, \gcd(b, c)) = \gcd(\gcd(a, b), c)$ (Works for entire arrays).
- **Identity:** $\gcd(a, 0) = |a|$
- **Difference Rule:** $\gcd(a, b) = \gcd(a, b - a)$
- **Scaling:** $\gcd(ka, kb) = |k| \cdot \gcd(a, b)$

### B. Relationship with LCM

For any two integers $a$ and $b$:

$$a \cdot b = \gcd(a, b) \cdot \text{lcm}(a, b)$$

### C. Advanced Properties

- **Fibonacci:** $\gcd(F_n, F_m) = F_{\gcd(n, m)}$
- **Exponents:** For $a > 1$, $\gcd(a^m - 1, a^n - 1) = a^{\gcd(m, n)} - 1$
- **Bézout's Identity:** $\gcd(a, b)$ is the smallest positive integer that can be written as $ax + by$ for some $x, y \in \mathbb{Z}$.

---

## 3. Computation: The Euclidean Algorithm

The most efficient way to compute GCD is by using the remainder instead of repeated subtraction.

$$\gcd(a, b) = \gcd(b, a \bmod b)$$

```cpp

// Recursive Manual Implementation
long long gcd(long long a, long long b) {
    return b == 0 ? a : gcd(b, a % b);
}
// Iterative Manual Implementation
long long gcd_iter(long long a, long long b) {
    while (b) {
        a %= b;
        swap(a, b);
    }
    return a;
}
```

**Complexity:** $O(\log(\min(a, b)))$. For $10^{18}$, this takes fewer than 100 steps.

---

## 4. Competitive Programming Applications

- **Simplifying Ratios:** Divide $a$ and $b$ by $\gcd(a, b)$ to get the irreducible form $a' : b'$.
- **Lattice Points:** The number of integer points on a line segment from $(0,0)$ to $(X, Y)$ is $\gcd(X, Y) + 1$.
- **Array GCD:** To find the GCD of a whole array, initialize `res = 0` and iterate: `res = gcd(res, element)`.
- **The Water Jug Problem:** You can measure $K$ liters using jugs $A$ and $B$ **if and only if** $K \% \gcd(A, B) == 0$.


| **Property**    | **Formula**                        | **CP Application**                |
| --------------- | ---------------------------------- | --------------------------------- |
| **Associative** | $\gcd(a,b,c) = \gcd(a, \gcd(b,c))$ | GCD of an array / range           |
| **Difference**  | $\gcd(a, b) = \gcd(a, b-a)$        | Range updates (Difference Arrays) |
| **Product**     | $ab = \gcd \cdot \text{lcm}$       | Finding LCM efficiently           |
| **Scaling**     | $\gcd(ka, kb) = k\gcd(a,b)$        | Factoring out constants           |
