
> [!ABSTRACT] The Core Idea
> 
> The **Least Common Multiple (LCM)** of two integers $a$ and $b$ is the **smallest positive integer** that is divisible by both $a$ and $b$ without a remainder. It represents the first point where two different "rhythms" or cycles meet.
> 
> **Tags:** #number-theory #math #algorithms #lcm #competitive-programming

---

## 1. Definition & Examples

For any two integers $a$ and $b$, the LCM is the smallest $L$ such that $a|L$ and $b|L$.

| **a** | **b** | **lcm(a,b)** | **Logic**                                                                  |
| ----- | ----- | ------------ | -------------------------------------------------------------------------- |
| $4$   | $6$   | $12$         | Multiples of 4: (4, 8, **12**, 16...). Multiples of 6: (6, **12**, 18...). |
| $3$   | $5$   | $15$         | Since they are coprime, the LCM is simply $3 \times 5$.                    |
| $10$  | $20$  | $20$         | If $a$ divides $b$, then $\text{lcm}(a, b) = b$.                           |

---

## 2. The Fundamental Identity

The most efficient way to find the LCM is through its relationship with the **Greatest Common Divisor (GCD)**. For any two integers:

$$a \cdot b = \gcd(a, b) \cdot \text{lcm}(a, b)$$

This gives us the standard formula used in programming:

$$\text{lcm}(a, b) = \frac{|a \cdot b|}{\gcd(a, b)}$$

---

## 3. Prime Factorization View

If you look at the prime factors of $a$ and $b$:

- **GCD** takes the **minimum** power of each common prime.
- **LCM** takes the **maximum** power of every prime present in either number.

**Example:**

$12 = 2^2 \cdot 3^1$

$18 = 2^1 \cdot 3^2$

$\text{lcm}(12, 18) = 2^{\max(2,1)} \cdot 3^{\max(1,2)} = 2^2 \cdot 3^2 = 36$.

---

## 4. Properties of LCM

- **Commutative:** $\text{lcm}(a, b) = \text{lcm}(b, a)$
- **Associative:** $\text{lcm}(a, \text{lcm}(b, c)) = \text{lcm}(\text{lcm}(a, b), c)$
    - _Note:_ You can find the LCM of an entire array by iterating through it.
- **Distributive:** $\text{lcm}(a, \gcd(b, c)) = \gcd(\text{lcm}(a, b), \text{lcm}(a, c))$
- **Scaling:** $\text{lcm}(ka, kb) = |k| \cdot \text{lcm}(a, b)$

---

## 5. Implementation 

> [!WARNING] Critical Implementation Tip
> 
> When writing your own function, **always divide before you multiply** to prevent integer overflow.
> 
> $(a \cdot b) / g$ can overflow even if the final result fits in a `long long`.
> 
> Instead, use: $(a / g) \cdot b$.


```cpp

long long get_lcm(long long a, long long b) {
    if (a == 0 || b == 0) return 0;
    long long g = gcd(a, b);
    // Divide first to avoid overflow
    return (abs(a) / g) * abs(b);
}
```

---

##  Common CP Use Cases

1. **Cycle Synchronization:** If object A repeats every $A$ seconds and object B every $B$ seconds, they synchronize every $\text{lcm}(A, B)$ seconds.
2. **Fraction Addition:** Finding a common denominator for adding fractions.
3. **Periodic Patterns:** Solving problems involving gears, tiles, or repeating string patterns.
