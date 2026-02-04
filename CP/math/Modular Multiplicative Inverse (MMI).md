
> [!ABSTRACT] Definition
> 
> Given an integer $a$ and modulus $m$, the modular inverse $x$ is a number such that:
> 
> $$a \cdot x \equiv 1 \pmod{m}$$
> 
> It is written as $a^{-1} \pmod m$. Effectively, multiplying by $x$ is the same as dividing by $a$ under modulo $m$

---

## 1. The Core Constraint: Existence

An inverse $a^{-1} \pmod m$ exists **if and only if** $a$ and $m$ are coprime:

$$\gcd(a, m) = 1$$
---
## 2. Why do we need it?

In modular arithmetic, **standard division is forbidden**. You cannot do $(a / b) \pmod m$ because the result of $a/b$ might be a fraction, but modular arithmetic only works with integers.

Instead, we use the identity:

$$\frac{a}{b} \pmod m \iff (a \cdot b^{-1}) \pmod m$$