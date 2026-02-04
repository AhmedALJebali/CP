
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

