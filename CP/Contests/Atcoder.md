
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
- **Bézout's Identity:** $\gcd(a, b)$ is the