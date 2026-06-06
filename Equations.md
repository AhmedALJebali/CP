
##  LERP & Optimization

### 1. Linear Interpolation (LERP) Formulas

These equations help you find a point $P$ that sits $t\%$ of the way between $A$ and $B$.

| **Name**          | **Equation**             | **Mental Model**                                                 |
| ----------------- | ------------------------ | ---------------------------------------------------------------- |
| **Weighted Form** | $$P(t) = (1 - t)A + tB$$ | $A$ and $B$ pulling on $P$ with different strengths.             |
| **Offset Form**   | $$P(t) = A + t(B - A)$$  | Start at $A$, then move a fraction ($t$) of the distance to $B$. |

> **Pro-Tip:** In CP, $t$ is often calculated as $t = \frac{\text{current\_time}}{\text{total\_duration}}$ or $t = \frac{i}{N-1}$ when distributing $N$ points.

---

### 2. The Argmin Identity (Optimization)

When you need to find the **index** or **value** of $t$ that minimizes a function, use the squared version to simplify your life.

#### The Core Rule:

$$\text{argmin } |f(t)| \iff \text{argmin } \big( f(t) \big)^2$$

**Why this is a "Cheat Code" in CP:**

- **No Square Roots:** Computing $\sqrt{x^2 + y^2}$ is computationally expensive and introduces floating-point errors. Comparing $x^2 + y^2$ directly is 100% precise with `long long`.
- **Avoiding Absolute Values:** $|ax + by + c|$ is annoying to differentiate or use in equations because of the "V" shape. $(ax + by + c)^2$ is a smooth parabola, making it perfect for **Ternary Search**.

---To solidify your cheatsheet, here are the **Standard Quadratic Equations** you need. In competitive programming, you usually don't get the equation handed to you—you have to build it from the geometry.

---

## 3. The Quadratic Standard Form

For any quadratic cost or distance function:

$$f(t) = at^2 + bt + c$$

|**Objective**|**Formula**|**Logic**|
|---|---|---|
|**Optimal Input**|$$t = -\frac{b}{2a}$$|The value of $t$ where the min/max occurs (**argmin**).|
|**Optimal Value**|$$f(t) = c - \frac{b^2}{4a}$$|The actual minimum/maximum value reached.|

---

## Quadratic Function in Vector Form

### 1. General Form

When expressing the squared distance (or a similar metric) as a function of time $t$, the equation follows the standard quadratic structure:

$$g(t) = at^2 + bt + c$$



### 2. Finding the Minimum (Vertex)

To find the time $t$ at which the function reaches its **minimum value** (e.g., the moment of closest approach):

**Standard Formula:**

$$t_{min} = -\frac{b}{2a}$$

## A Comprehensive Guide: Extracting Number Properties Using Prime Factorization


Many mathematical algorithms rely on one fundamental step: **Prime Factorization**.

Any integer $N$ can be expressed as a product of prime numbers raised to specific powers. The standard mathematical representation is:

$$N = p_1^{e_1} \cdot p_2^{e_2} \cdots p_k^{e_k}$$

_(where $p$ represents a prime factor, and $e$ represents its exponent or the number of times it is repeated)._

Once you break down a number into this format, it becomes an open book. You can easily extract several of its properties, as explained below.

---

### 1. Number of Divisors

**The Core Concept:**

Any divisor of $N$ must be built from the exact same prime factors, but with exponents that can range from $0$ up to the original exponent $e$.

- For the first prime factor $p_1$: You have $(e_1 + 1)$ choices for the exponent (from $0$ to $e_1$).
- For the second prime factor $p_2$: You have $(e_2 + 1)$ choices.

**The Formula:**

To find the total number of possible divisors, you multiply the number of choices available for each prime factor (using the counting principle):

$$\text{Total Divisors} = (e_1 + 1) \cdot (e_2 + 1) \cdots (e_k + 1)$$

> **Practical Example (The Number 12):**
> 
> - **Factorization:** $12 = 2^2 \cdot 3^1$
> - **Choices for Factor 2:** The exponent can be $0, 1,$ or $2$ (3 choices).
> - **Choices for Factor 3:** The exponent can be $0$ or $1$ (2 choices).
> - **Result:** $3 \times 2 = 6$ total divisors. (These are: 1, 2, 3, 4, 6, and 12).

---

### 2. Sum of Divisors

**The Core Concept:**

If you want to add all these divisors together instead of just counting them, you sum all the possible combinations for each prime factor inside a bracket, and then multiply the brackets. When these brackets are mathematically expanded, they yield the exact sum of all possible divisors.

**The Formula:**

$$\text{Sum} = (1 + p_1 + p_1^2 + \dots + p_1^{e_1}) \cdot (1 + p_2 + \dots + p_2^{e_2}) \cdots$$

**Calculation Speed-Up:**

Each bracket represents a "Geometric Series." Instead of using a loop to sum the values inside each bracket step-by-step, you can use the geometric series formula to calculate it instantly in $O(1)$ time:

$$\text{Sum of one bracket} = \frac{p^{e+1} - 1}{p - 1}$$

---

### 3. Product of Divisors

**The Core Concept:**

Divisors naturally come in pairs. Every divisor $x$ has a complementary divisor $\frac{N}{x}$, and multiplying this pair together always equals the original number $N$.

_(For example, the divisors of 12: $1 \times 12 = 12$, $2 \times 6 = 12$, and $3 \times 4 = 12$)._

Since every pair of divisors produces $N$, multiplying all divisors together is essentially multiplying $N$ by itself for half the total number of divisors.

**The Formula:**

$$\text{Product} = N^{\frac{d}{2}}$$

_(where $d$ is the total number of divisors we calculated in Step 1)._

---

### 4. Count of Coprime Numbers (Euler's Totient Function)

**The Core Concept:**

Often denoted as $\phi(N)$, this function calculates how many integers from $1$ to $N$ are "coprime" (or relatively prime) to $N$. A number is coprime to $N$ if they do not share any common divisors other than 1 (i.e., their Greatest Common Divisor is 1).

Interestingly, this formula depends entirely on the distinct prime factors themselves, completely ignoring their exponents.

**The Formula:**

$$\phi(N) = N \cdot \left(1 - \frac{1}{p_1}\right) \cdot \left(1 - \frac{1}{p_2}\right) \cdots \left(1 - \frac{1}{p_k}\right)$$