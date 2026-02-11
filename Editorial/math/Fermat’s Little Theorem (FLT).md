
> [!ABSTRACT] The Core Statement
> 
> If $p$ is a prime number and $a$ is an integer such that $p$ does not divide $a$ (i.e., $\gcd(a, p) = 1$), then:
> 
> $$a^{p-1} \equiv 1 \pmod{p}$$
> 
> **General form:** For any integer $a$:
> 
> $$a^p \equiv a \pmod{p}$$

---

##  The Proof (Combinatorial Approach)

One of the most intuitive ways to prove FLT is by using a **necklaces** argument.

### 1. The Setup

Suppose we want to create a necklace with $p$ beads, where $p$ is a prime number. We have $a$ different colors of beads available to us.

### 2. Total Arrangements

The total number of ways to arrange the beads in a straight line of length $p$ is $a^p$ (since each of the $p$ positions has $a$ choices).

### 3. Subtracting Monochromatic Lines

There are exactly $a$ arrangements where all $p$ beads are the same color (e.g., all red, all blue, etc.). We subtract these out, leaving us with:

$$a^p - a$$

These are the arrangements that use at least two different colors.

### 4. Forming the Necklaces

Now, we join the ends of these lines to form a circle (a necklace).

- For any non-monochromatic arrangement, rotating the necklace by $1, 2, \dots, p-1$ positions will produce $p$ distinct linear patterns.
- Because $p$ is **prime**, we cannot have "repeating sub-patterns" that would cause the rotations to cycle early (like a pattern of `ABAB` in a 4-bead necklace).
- Therefore, the remaining $a^p - a$ linear arrangements can be grouped into sets of exactly $p$.
### 5. Conclusion

Since the arrangements are grouped into sets of $p$, the total number must be divisible by $p$:

$$(a^p - a) \equiv 0 \pmod{p}$$

Which simplifies to:

$$a^p \equiv a \pmod{p}$$

If we divide both sides by $a$ (allowed if $\gcd(a, p) = 1$), we get the standard form:

$$a^{p-1} \equiv 1 \pmod{p}$$

---

##  Application: Modular Multiplicative Inverse

In competitive programming, we use this theorem to find the modular inverse of a number $a$ under a prime modulus $p$.

**The Logic:**

Since $a \cdot a^{p-2} = a^{p-1} \equiv 1 \pmod{p}$, it follows that:

$$a^{-1} \equiv a^{p-2} \pmod{p}$$

> [!TIP] Implementation
> 
> Use **Binary Exponentiation** to calculate $a^{p-2} \pmod{p}$ in $O(\log p)$ time.



```cpp
long long power(long long a, long long b, long long m) {
    long long res = 1;
    a %= m;
    while (b > 0) {
        if (b & 1) res = (res * a) % m;
        a = (a * a) % m;
        b >>= 1;
    }
    return res;
}

// Only works if m is prime!
long long modInverse(long long a, long long m) {
    return power(a, m - 2, m);
}
```
