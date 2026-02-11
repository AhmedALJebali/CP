
> [!ABSTRACT] The Core Idea
> 
> The Extended Euclidean Algorithm (EEA) is an extension of the standard GCD algorithm. While the standard version only finds the $\gcd(a, b)$, the EEA also finds the **Bézout coefficients** $(x, y)$ such that:
> 
> $$ax + by = \gcd(a, b)$$
---

## 1. What does it solve?


The standard Euclidean algorithm tells us the largest number that divides both $a$ and $b$. The EEA goes further by expressing that $\gcd$ as a linear combination of $a$ and $b$.

- **Input:** Two integers $a$ and $b$.
- **Output:** $\gcd(a, b)$ and coefficients $x, y$.

> [!EXAMPLE] Walkthrough
> 
> For $a = 12, b = 42$:
> 
> 1. $\gcd(12, 42) = 6$.
>     
> 2. EEA finds $x = -3, y = 1$ such that $12(-3) + 42(1) = 6$.
>     

---

## 2. Mathematical Intuition (The Swap)

The algorithm works by using the property $\gcd(a, b) = \gcd(b, a \bmod b)$ and tracking how the coefficients change.

Suppose we have the result for the next step in the recursion:

$$b \cdot x_1 + (a \bmod b) \cdot y_1 = g$$

Since $a \bmod b = a - \lfloor a/b \rfloor \cdot b$, we substitute:

$$b \cdot x_1 + (a - \lfloor a/b \rfloor \cdot b) \cdot y_1 = g$$

$$a(y_1) + b(x_1 - \lfloor a/b \rfloor \cdot y_1) = g$$

**New Coefficients:**

- $x = y_1$
- $y = x_1 - \lfloor a/b \rfloor \cdot y_1$

![[download.png]]
---

## 3. Implementation (Recursive)

This is the most common version used in Competitive Programming due to its brevity.


```cpp
/**
 * @return gcd(a, b)
 * @param x sets the coefficient for a
 * @param y sets the coefficient for b
 */
int ExtendedEuclid(int a, int b, int &x, int &y) {  
  if (!b) {  
    x = 1, y = 0;  
    return a;  
  }  
  int x1, y1;  
  int g = ExtendedEuclid(b, a % b, x1, y1);  
  x = y1;  
  y = x1 - y1 * (a / b);  
  return g;  
}
```

# General Solutions for Diophantine Equations

> [!ABSTRACT] Beyond the First Solution
> 
> The Extended Euclidean Algorithm (EEA) provides one specific solution $(x_0, y_0)$. However, there are infinitely many integer solutions that lie on the same line.
> 
---

## 4. The General Formula for All Solutions

Once you have found a particular solution $(x_0, y_0)$ such that $Ax_0 + By_0 = D$ (where $D = \gcd(A, B)$), you can generate **every other integer solution** using the following parameter $k$:

$$\boxed{ \begin{align} x &= x_0 + k \cdot \frac{B}{D} \\ y &= y_0 - k \cdot \frac{A}{D} \end{align} } \quad \text{for any } k \in \mathbb{Z}$$

###  Why does this work?

Notice that if we substitute these back into the original equation:


$$A\left(x_0 + k \frac{B}{D}\right) + B\left(y_0 - k \frac{A}{D}\right) = (Ax_0 + By_0) + \left(k \frac{AB}{D} - k \frac{AB}{D}\right) = D + 0 = D$$
The terms involving $k$ always cancel out, proving that any integer $k$ yields a valid solution.
---

## 5. Finding the Smallest Positive Solution

In problems like **Modular Inverse** or **Resource Allocation**, you usually need the smallest non-negative value for $x$.

### The "Modulo" Trick

To transform any $x_0$ into the smallest non-negative $x$, we use the step size $S = \frac{B}{D}$:

1. Calculate the step size: $S = |B / D|$.
2. Normalize $x$:

    ```cpp
   x = (x0 % S + S) % S;
    ```

3. Calculate the corresponding $y$:
 $$y = \frac{D - Ax}{B}$$

> [!WARNING] Division by Zero
> 
> If $B=0$, the step size is undefined. Always handle the $B=0$ case separately (usually $\gcd(A,0)=A$, so $x=1, y=0$ if $c$ is a multiple of $A$).

---

## 6. Solving $Ax + By = C$ (General Case)

If the target value $C$ is not equal to $\gcd(A, B)$, follow these steps:

1. **Check Feasibility:** If $C \pmod{\gcd(A, B)} \neq 0$, there are **zero** integer solutions.
2. **Find Base Solution:** Use EEA to find $(x_{base}, y_{base})$ for $Ax + By = \gcd(A, B)$.
3. **Scale:** Multiply the base solutions by $\frac{C}{\gcd(A, B)}$ to get your particular solution:
    $$x_0 = x_{base} \cdot \frac{C}{D}, \quad y_0 = y_{base} \cdot \frac{C}{D}$$
4. **Apply General Formula:** Use the formula from Section 4 to find other solutions if needed.


