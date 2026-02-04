
> [!ABSTRACT] Context
> 
> Grid walking problems are a staple of competitive programming and discrete math. They usually involve finding the number of paths between two points on a 2D plane under specific constraints.
> 
> **Tags:** #combinatorics #math #algorithms #dynamic-programming

---

## 1. Basic Grid Walk (Right & Up)

**Problem:** Count the number of paths from $(0,0)$ to $(n,m)$ using only **Right (R)** and **Up (U)** moves.
###  The Insight

To reach $(n,m)$, you must take exactly $n$ horizontal steps and $m$ vertical steps. The total number of steps is always $n + m$. The problem is simply finding how many ways we can arrange these $n$ "R"s and $m$ "U"s in a sequence.

**The Formula:**

$$\binom{n+m}{n} \quad \text{or} \quad \binom{n+m}{m}$$

---

## 2. Walking Between Arbitrary Points

**Problem:** Move from $(x_1, y_1)$ to $(x_2, y_2)$.

> [!IMPORTANT] Constraint
> 
> This only works if $x_2 \ge x_1$ and $y_2 \ge y_1$. If you can only move Right and Up, you can never go "backwards."

**The Calculation:**

- **$\Delta x$ (Right steps):** $x_2 - x_1$
- **$\Delta y$ (Up steps):** $y_2 - y_1$
**The Formula:**

$$\binom{(x_2-x_1) + (y_2-y_1)}{x_2-x_1}$$

---

## 3. Mandatory Checkpoints

**Problem:** The path must pass through a specific coordinate $(x_c, y_c)$.

To solve this, treat the journey as two independent sub-problems. You must go from the **Start to the Checkpoint**, and then from the **Checkpoint to the End**.

**The Logic:**
$\text{Paths} = (\text{Start} \to \text{Checkpoint}) \times (\text{Checkpoint} \to \text{End})$
**The Formula:**

$$\binom{x_c+y_c}{x_c} \cdot \binom{(n-x_c)+(m-y_c)}{n-x_c}$$

---

## 4. Grid Walk with Multiple Obstacles

When dealing with multiple blocked cells $O_1, O_2, \dots, O_k$, we cannot simply subtract each one individually because we would double-subtract paths that pass through multiple obstacles.
###  The DP + Inclusion-Exclusion Strategy

To find the number of **valid paths** (those that hit zero obstacles), we calculate the number of **invalid paths** and subtract them from the total.
**1. Preparation**

- **Sort** all obstacles by their $x$-coordinates, and then by their $y$-coordinates. This ensures that if obstacle $j$ is reachable from obstacle $i$, then $i < j$.
- Let the starting point be $(0,0)$ and the target destination be $(n, m)$. Treat the **target** as the final "obstacle" ($O_{k+1}$) to simplify the calculation.

**2. Define the DP State**

Let $DP[i]$ be the number of paths from the **Start** to **Obstacle $i$** such that $O_i$ is the **very first obstacle** encountered.

**3. The Transition Formula**

To ensure $O_i$ is the _first_ obstacle hit, we take the total paths to $O_i$ and subtract paths that hit any _previous_ obstacle $j$ first:

$$DP[i] = \text{Ways}(Start \to O_i) - \sum_{j < i} \left( DP[j] \times \text{Ways}(O_j \to O_i) \right)$$

> [!NOTE] Logic Check
> 
> - $\text{Ways}(A \to B)$ is the standard $\binom{\Delta x + \Delta y}{\Delta x}$ formula.
>     
> - If $O_j$ is to the right or above $O_i$, $\text{Ways}(O_j \to O_i) = 0$.
>     
> - By multiplying $DP[j]$ (paths where $O_j$ was the _first_ hit) by the paths from $O_j$ to $O_i$, we perfectly partition the set of "bad" paths without overcounting.
>     

**4. Final Answer**

Since we treated the target $(n, m)$ as the last obstacle $O_{k+1}$:

$$\text{Valid Paths} = DP[k+1]$$

---

## 5. Fixed-Length Walk (Four Directions)

**Problem:** Move from $(x_1, y_1)$ to $(x_2, y_2)$ in **exactly** $k$ steps using $\{U, D, L, R\}$.
Let $d$ be the **Manhattan Distance**: $d = |x_1-x_2| + |y_1-y_2|$.

> [!WARNING] Validity Check
> 
> A path of exactly $k$ steps exists **if and only if**:
> 
> 1. $k \ge d$ (You have enough steps to reach the destination).
>     
> 2. $(k - d)$ is **even** (Any "extra" steps must be "undone" by a reverse move, e.g., Right then Left).
>     

---

## 6. Boundary Constraints (Reflection Principle)

**Problem:** Paths must stay within a boundary (e.g., never crossing the line $y = x$).
**The Technique:**

1. Count all paths from $A$ to $B$.
2. Any "illegal" path that touches the boundary is reflected across that boundary line.
3. The number of illegal paths from $A$ to $B$ is equal to the number of paths from the **reflected Start Point $A'$** to $B$.
