## 1. The Mathematical Definition

The $n$-th Catalan number, denoted as $C_n$, is calculated using the central binomial coefficient:

$$C_n = \frac{1}{n+1} \binom{2n}{n} = \frac{(2n)!}{(n+1)! \, n!}$$

### The First Few Numbers:

Starting from $n=0$, the sequence begins:

**1, 1, 2, 5, 14, 42, 132, 429, 1430, ...**

---

## 2. The Recurrence Relation

Catalan numbers are naturally recursive. To find $C_{n+1}$, you can sum the products of earlier terms:

$$C_{n+1} = \sum_{i=0}^{n} C_i \cdot C_{n-i}$$

> **The Logic:** This formula represents splitting a structure into two smaller sub-structures (like a left branch and a right branch of a tree).

---

## 3. Classic Examples (The "Where" and "How")

Catalan numbers solve problems that require **non-crossing** or **balanced** structures.

### A. Valid Parentheses

How many ways can you arrange $n$ pairs of parentheses so they are properly matched?

- **For $n=3$ ($C_3=5$):**
    1. `((()))`
    2. `(()())`
    3. `(())()`
    4. `()(())`
    5. `()()()`


### B. Binary Search Trees (BST)

How many unique Binary Search Trees can be created using $n$ distinct keys?

- **Example:** For 3 nodes ($A, B, C$), there are exactly **5** different tree shapes.

### C. Polygon Triangulation

How many ways can you divide a convex polygon with $n+2$ sides into triangles by drawing non-intersecting diagonals?

- **For a Square ($n=2$):** 2 ways (splitting via either diagonal).
- **For a Pentagon ($n=3$):** 5 ways.
### D. Dyck Paths

How many paths exist from $(0,0)$ to $(2n, 0)$ that stay above the x-axis and use only "up" and "down" steps? (Or paths from $(0,0)$ to $(n,n)$ that never cross the diagonal $y=x$.

- **Result:** $C_n$ paths.

---

## 4. Worked Example: Calculating $C_3$

Let’s calculate the 3rd Catalan number step-by-step using the formula:

1. **Identify $n$:** $n = 3$
2. **Plug into formula:** $C_3 = \frac{1}{3+1} \binom{2 \times 3}{3}$
3. **Calculate the Binomial Coefficient:** $\binom{6}{3} = \frac{6 \times 5 \times 4}{3 \times 2 \times 1} = 20$
4. **Final Division:** $C_3 = \frac{1}{4} \times 20 = \mathbf{5}$

---
##  Dynamic Programming Approach 
This approach uses the **Recurrence Relation**:

$$C_{n} = \sum_{i=0}^{n-1} C_i \cdot C_{n-1-i}$$

It is highly reliable because it only uses addition and multiplication, avoiding precision issues found in division.

C++

```cpp


/**
 * Calculates the n-th Catalan number using Dynamic Programming.
 * Time Complexity: O(n^2)
 * Space Complexity: O(n)
 */
unsigned long long catalanDP(int n) {
vector<unsigned long long> catalan(n + 1, 0);
    // Base cases
    catalan[0] = 1;
    if (n > 0) catalan[1] = 1;
    for (int i = 2; i <= n; i++) {
     for (int j = 0; j < i; j++) {
    catalan[i] += catalan[j] * catalan[i - 1 - j];
     }
    }
    return catalan[n];
}
```

---

## 2. Iterative Formula Approach ($O(n)$)

This is significantly faster for large $n$. It uses the reduced recurrence relation:

$$C_n = \frac{4n - 2}{n + 1} C_{n-1}$$

By multiplying first and then dividing, we can maintain integer accuracy.

```cpp
/**
 * Calculates the n-th Catalan number using the efficient iterative formula.
 * Time Complexity: O(n)
 * Space Complexity: O(1)
 */
unsigned long long catalanIterative(int n) {
    if (n <= 1) return 1;

    unsigned long long res = 1;
    for (int i = 1; i <= n; ++i) {
        // Using the formula: C(i) = C(i-1) * (4*i - 2) / (i + 1)
        res = res * (4 * i - 2) / (i + 1);
    }
    return res;
}
```

---
