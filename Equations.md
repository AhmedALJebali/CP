
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
