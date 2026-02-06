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

## 6. Summary Comparison

| **Method**          | **Complexity** | **Best For**                     | **Exact?**            |
| ------------------- | -------------- | -------------------------------- | --------------------- |
| **Iterative (DP)**  | $O(n)$         | Small $n$ ($< 10^7$)             | Yes                   |
| **Binet's Formula** | $O(1)$         | Approximation / Math proofs      | No (Precision issues) |
| **Matrix Exp**      | $O(\log n)$    | Large $n$, arbitrary recurrences | Yes                   |
| **Fast Doubling**   | $O(\log n)$    | Maximum speed in CP              | Yes                   |

---

### Zeckendorf's Theorem (Bonus)

Every positive integer can be uniquely represented as a sum of **non-consecutive** Fibonacci numbers ($F_n$ where $n \geq 2$).

_Example:_ $100 = 89 + 8 + 3$.

This theorem is the basis for **Fibonacci Coding**, a universal code that represents integers into binary based on Fibonacci positions.

Would you like me to provide a Python implementation or dive deeper into the proof of the Pisano Period?