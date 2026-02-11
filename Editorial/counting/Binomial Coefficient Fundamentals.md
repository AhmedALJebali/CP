
The number of ways to choose $r$ elements from a set of $n$ elements is given by:

$$\binom{n}{r} = \frac{n!}{r!(n-r)!}$$

---

## 1. Boundary Values

$$\binom{n}{0} = \binom{n}{n} = 1$$

**Proof:**

Using the factorial formula:

$\binom{n}{0} = \frac{n!}{0!(n-0)!} = \frac{n!}{1 \cdot n!} = 1$.

**Example:**

Choosing 0 items from 5 $(\binom{5}{0})$ or all 5 items $(\binom{5}{5})$ can only be done in 1 way.

## 2. Automatic Zero (Out of Bounds)

$$\binom{n}{r} = 0 \quad \text{if } r < 0 \text{ or } r > n$$

**Intuition:**

You cannot choose more items than you have, nor can you choose a negative number of items.

**Application:**

In Dynamic Programming transitions, this allows you to avoid complex `if` conditions by treating out-of-bounds states as zero.

## 3. Symmetry Property

$$\binom{n}{r} = \binom{n}{n-r}$$

**Proof:**

$\binom{n}{n-r} = \frac{n!}{(n-r)!(n-(n-r))!} = \frac{n!}{(n-r)!r!}$, which is identical to $\binom{n}{r}$.

**Example:**

Choosing 3 people to join a committee from 10 is the same as choosing 7 people to leave out.

**Optimization:**

In code, always use $r = \min(r, n-r)$ to reduce the number of multiplications.

## 4. Pascal's Identity

$$\binom{n}{r} = \binom{n-1}{r} + \binom{n-1}{r-1}$$

**Proof (Combinatorial):**

Consider a specific element $x$ in a set of $n$ elements. To choose $r$ elements:

1. If $x$ is **not** chosen, we must choose $r$ elements from the remaining $n-1$: $\binom{n-1}{r}$.
2. If $x$ **is** chosen, we must choose $r-1$ more elements from the remaining $n-1$: $\binom{n-1}{r-1}$.
    **Application:**
    This is the recurrence used to build Pascal’s Triangle in $O(n^2)$.
    

![صورة Pascal's Triangle](https://encrypted-tbn1.gstatic.com/licensed-image?q=tbn:ANd9GcSKURvXoDmj2m-G3Ot5rBRZPwJEVPEUDJMn2ies8x6YG1WEC0MNi8LTVobvfbUkXbOrz1nT3H2fD7y3IFZwVe8_BWBMLlj-h8jiu49wMZ8G75m0Qu4)

Shutterstock

## 5. Absorption Identities

$$r \binom{n}{r} = n \binom{n-1}{r-1}$$

$$(n-r) \binom{n}{r} = n \binom{n-1}{r}$$

**Proof:**

$r \cdot \frac{n!}{r!(n-r)!} = \frac{n \cdot (n-1)!}{(r-1)!(n-r)!} = n \binom{n-1}{r-1}$.

**Application:**

Useful for simplifying summations where the index $r$ appears outside the binomial coefficient.

## 6. Sum of Binomial Coefficients

$$\sum_{r=0}^{n} \binom{n}{r} = 2^n$$

**Proof:**

By the Binomial Theorem, $(1+1)^n = \sum_{r=0}^{n} \binom{n}{r} (1)^r (1)^{n-r} = 2^n$.

**Interpretation:**

The total number of subsets of a set of size $n$ is $2^n$.

## 7. Sum of Squares

$$\sum_{r=0}^{n} \binom{n}{r}^2 = \binom{2n}{n}$$

**Intuition:**

Since $\binom{n}{r} = \binom{n}{n-r}$, the sum is $\sum \binom{n}{r}\binom{n}{n-r}$. This represents choosing $r$ elements from the first group of $n$ and $n-r$ elements from a second group of $n$, effectively choosing $n$ elements from $2n$.

## 8. Vandermonde's Identity

$$\sum_{k=0}^{r} \binom{n}{k}\binom{m}{r-k} = \binom{n+m}{r}$$

**Example:**

If you have $n$ boys and $m$ girls and want to form a committee of $r$ people, you sum the ways of picking $k$ boys and $r-k$ girls for all possible values of $k$.

## 9. Iterative Formula

$$\binom{n}{r} = \binom{n}{r-1} \cdot \frac{n-r+1}{r}$$

**Application:**

Allows computing $\binom{n}{r}$ in $O(r)$ time without calculating large factorials first, which helps prevent overflow in intermediate steps.

## 10. Growth and Maxima

The value of $\binom{n}{r}$ increases as $r$ approaches $\frac{n}{2}$.

- If $n$ is even, the maximum is $\binom{n}{n/2}$.
- If $n$ is odd, the maxima are $\binom{n}{(n-1)/2}$ and $\binom{n}{(n+1)/2}$.
    **Note:**
    These values grow exponentially; $\binom{100}{50}$ is approximately $10^{29}$, necessitating the use of Modular Arithmetic in CP.
## 11. Binomial Theorem

$$(x+y)^n = \sum_{r=0}^{n} \binom{n}{r} x^r y^{n-r}$$

**Application:**

Used to expand algebraic expressions and find coefficients of polynomials in counting problems.

## 12. Modular Inverse (When $M$ is Prime)

In Competitive Programming, we often need $\binom{n}{r} \pmod M$. If $M$ is prime, use Fermat's Little Theorem for the modular inverse:

$$inv(a) \equiv a^{M-2} \pmod M$$

**Implementation:**

1. Precompute factorials: $fact[i] = (fact[i-1] \cdot i) \pmod M$.
2. Precompute inverse factorials: $invFact[i] = inv(fact[i]) \pmod M$.
3. Result: $\binom{n}{r} = fact[n] \cdot invFact[r] \cdot invFact[n-r] \pmod M$.

---

## 13. Lucas's Theorem

Lucas's Theorem is a powerful tool used to calculate the value of $\binom{n}{r} \pmod P$ when $P$ is a **small prime** number, but $n$ and $r$ are very large (potentially up to $10^{18}$).

**Mathematical Statement:**

For a prime $P$, let the base-$P$ expansions of $n$ and $r$ be:

$$n = n_k P^k + n_{k-1} P^{k-1} + \dots + n_1 P + n_0$$

$$r = r_k P^k + r_{k-1} P^{k-1} + \dots + r_1 P + r_0$$

Then:

$$\binom{n}{r} \equiv \prod_{i=0}^{k} \binom{n_i}{r_i} \pmod P$$

**Intuition:**

Instead of dealing with massive factorials that would overflow even a 64-bit integer, Lucas's Theorem allows us to break $n$ and $r$ into their "digits" in base $P$. We then calculate the binomial coefficient for each pair of digits and multiply them together under modulo $P$.

**Application:**

- **Handling $n \geq P$:** In standard modular arithmetic (Rule 12), if $n \geq P$, then $n! \equiv 0 \pmod P$, which makes the division $\frac{n!}{r!(n-r)!}$ impossible. Lucas's Theorem bypasses this by working only with digits smaller than $P$.
    
- **Time Complexity:** The process takes $O(P + \log_P n)$. You precompute factorials up to $P$ and then perform a simple loop to extract digits.
    

**Quick Example:**

To calculate $\binom{10}{3} \pmod 3$:

1. **Base-3 expansion of 10:** $10 = (1 \cdot 3^2) + (0 \cdot 3^1) + (1 \cdot 3^0) = (101)_3$.
2. **Base-3 expansion of 3:** $3 = (0 \cdot 3^2) + (1 \cdot 3^1) + (0 \cdot 3^0) = (010)_3$.
3. **Apply Theorem:** $\binom{10}{3} \equiv \binom{1}{0} \times \binom{0}{1} \times \binom{1}{0} \pmod 3$.
4. Since $\binom{0}{1} = 0$ (Rule 2), the entire product becomes **0**.

