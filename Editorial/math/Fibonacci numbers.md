## 1. The Foundation: Recursion and Identities

The sequence begins with $F_0 = 0, F_1 = 1$, and for $n \geq 2$:

$$F_n = F_{n-1} + F_{n-2}$$

Beyond the definition, several identities are vital for solving complex CP problems:

- **Sum of first $n$ terms:** $\sum_{i=0}^{n} F_i = F_{n+2} - 1$.
- **Cassini's Identity:** $F_{n-1}F_{n+1} - F_n^2 = (-1)^n$. This is often used to prove properties of Fibonacci-based geometric puzzles.
- **GCD Property:** $\gcd(F_m, F_n) = F_{\gcd(m, n)}$. This is a rare property that links term indices directly to their values.

---

## 2. Binet’s Formula: The Golden Connection

Binet’s Formula provides the "closed-form" solution to the recurrence:

$$F_n = \frac{\varphi^n - \psi^n}{\sqrt{5}}$$

Where $\varphi = \frac{1 + \sqrt{5}}{2}$ (The Golden Ratio) and $\psi = \frac{1 - \sqrt{5}}{2}$.

> **The Precision Trap:** > While Binet’s formula is $O(1)$ in theory, it is dangerous in practice. Floating-point precision (doubles) will fail for large $n$ (usually around $n > 70$). Furthermore, you cannot apply a **Modulo** to Binet's formula easily because of the $\sqrt{5}$ and division. It remains a theoretical masterpiece, not a coding tool.

---

## 3. Matrix Exponentiation: The Bridge to $O(\log n)$

To understand why we can calculate the $10^{18}$-th Fibonacci number, we must look at the matrix form:

$$\begin{pmatrix} F_{n+1} \\ F_n \end{pmatrix} = \begin{pmatrix} 1 & 1 \\ 1 & 0 \end{pmatrix} \begin{pmatrix} F_n \\ F_{n-1} \end{pmatrix}$$

By induction, this leads to:

$$\begin{pmatrix} F_{n+1} & F_n \\ F_n & F_{n-1} \end{pmatrix} = \begin{pmatrix} 1 & 1 \\ 1 & 0 \end{pmatrix}^n$$

Since we can perform **Binary Exponentiation** on matrices, we can find the $n$-th power in $O(2^3 \log n)$ steps.

```cpp
int fib(int n) {
    if (n == 0) return 0;

    const int MOD = 1e9 + 7;

    struct Matrix {
        int a, b, c, d;
    };

    auto multiply = [&](Matrix x, Matrix y) {
        return Matrix{
            (x.a * y.a + x.b * y.c) % MOD,
            (x.a * y.b + x.b * y.d) % MOD,
            (x.c * y.a + x.d * y.c) % MOD,
            (x.c * y.b + x.d * y.d) % MOD
        };
    };

    auto power = [&](Matrix base, long long exp) {
        Matrix result{1, 0, 0, 1}; // identity
        while (exp > 0) {
            if (exp & 1) result = multiply(result, base);
            base = multiply(base, base);
            exp >>= 1;
        }
        return result;
    };

    Matrix M{1, 1, 1, 0};
    Matrix Mn = power(M, n);

    return Mn.b; // F_n
}

```
---

## 4. Fast Doubling

Fast Doubling is essentially an optimized version of matrix exponentiation. It uses these two specific identities to jump through the sequence:

1. $F_{2k} = F_k(2F_{k+1} - F_k)$
2. $F_{2k+1} = F_{k+1}^2 + F_k^2$

```cpp
const long long MOD = 1e9 + 7;

// Returns {Fn, Fn+1}
std::pair<long long, long long> fast_doubling(long long n) {
    if (n == 0) return {0, 1};

    auto [a, b] = fast_doubling(n / 2); // a = Fk, b = Fk+1

    long long c = (a * (2 * b % MOD - a + MOD) % MOD) % MOD; // F2k
    long long d = (a * a % MOD + b * b % MOD) % MOD;         // F2k+1

    if (n % 2 == 0)
        return {c, d};
    else
        return {d, (c + d) % MOD};
}
```

---

## 5. Advanced Theory: Pisano Periods

In many problems, you are asked for $F_n \pmod m$. The sequence of Fibonacci numbers modulo $m$ is **periodic**. This period is called the **Pisano Period**, denoted as $\pi(m)$.

- $\pi(10) = 60$.
- $\pi(m)$ always starts with $0, 1$.
- Knowing the Pisano period allows you to reduce a massive $n$ using $F_n \pmod m = F_{n \pmod{\pi(m)}} \pmod m$.
---

## The Tiling Interpretation (The most visual)

The most common combinatorial interpretation of $F_{n+1}$ is the number of ways to tile a **$1 \times n$ board** using two types of tiles:

- **Squares** ($1 \times 1$)
- **Dominoes** ($1 \times 2$)
### Why it works:

To find the number of ways to tile a board of length $n$:

1. If the last tile is a **square**, you must have tiled a board of length $n-1$ in $F_{n}$ ways.
2. If the last tile is a **domino**, you must have tiled a board of length $n-2$ in $F_{n-1}$ ways.
    This leads directly to the recurrence $F_{n+1} = F_n + F_{n-1}$.

---

## Compositions and "No-Consecutive" Problems

Fibonacci numbers naturally count restricted arrangements:

- **No Consecutive 1s:** The number of binary strings of length $n$ that do **not** contain "11" is exactly $F_{n+2}$.
- **Staircase Climbing:** If you can take 1 or 2 steps at a time, the number of ways to reach the $n$-th step is $F_{n+1}$.
- **Subset Sums:** The number of subsets of $\{1, 2, \dots, n\}$ that do not contain any two consecutive integers is $F_{n+2}$.

---

## The Connection to Binomial Coefficients

Fibonacci numbers can be expressed as a "diagonal sum" of **Pascal’s Triangle**. This links the additive nature of Fibonacci back to the selection nature of Combinatorics:

$$F_{n+1} = \sum_{k=0}^{\lfloor n/2 \rfloor} \binom{n-k}{k}$$

> **The Insight:** This identity effectively says that to build a board of length $n$, you choose $k$ positions for dominoes out of the $n-k$ total tiles used.

