
> [!ABSTRACT] The Core Idea
> 
> For any two integers $a$ and $b$ (where at least one is non-zero), there exist integers $x$ and $y$ such that:
> 
> $$ax + by = \gcd(a, b)$$
> 
> These integers $(x, y)$ are called **Bézout coefficients**.
---

## 1. What does it actually mean?

Bézout's Identity tells us that the greatest common divisor ($\gcd$) of two numbers is the **smallest positive integer** that can be written as a "linear combination" of those two numbers.

**Example:**

Let $a = 12$ and $b = 42$. $\gcd(12, 42) = 6$.

According to Bézout, we can find integers $x$ and $y$ such that:

$$12x + 42y = 6$$

One solution: $x = -3, y = 1 \implies 12(-3) + 42(1) = -36 + 42 = 6$

> [!SUCCESS] Takeaway
> 
> Any multiple of the $\gcd$ can be expressed as $ax + by$. Any number that is **not** a multiple of the $\gcd$ cannot be formed.

---

## 2. When does a solution exist?

For a modular inverse or any equation $ax + by = c$:

1. A solution exists **if** $c$ is divisible by $\gcd(a, b)$.
2. For modular inverse ($ax \equiv 1 \pmod m$), this requires:

$$\gcd(a, m) = 1$$
---

## 3. Connection to Modular Inverse

A modular inverse is a number $x$ such that:

$$a \cdot x \equiv 1 \pmod m$$

Rewriting this:

$$a \cdot x - m \cdot y = 1$$

This is exactly Bézout's Identity ($ax + by = d$) with $d = 1$.
