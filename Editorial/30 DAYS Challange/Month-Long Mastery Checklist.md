
### AtCoder ABC Checklist (30 Contest Streak)

_Goal: Solve A–D in < 45 minutes for every entry._

**[ 01–10 ] — Building Momentum**

- [x] **Contest 01 & Upsolved** ([ABC433](https://atcoder.jp/contests/abc433))
- [ ] **Contest 02** (Target: ABC 414)
- [ ] **Contest 03** (Target: ABC 415)
- [ ] **Contest 04** (Target: ABC 416)
- [ ] **Contest 05** (Target: ABC 417)
- [ ] **Contest 06** (Target: ABC 418)
- [ ] **Contest 07** (Target: ABC 419)
- [ ] **Contest 08** (Target: ABC 420)
- [ ] **Contest 09** (Target: ABC 421)
- [ ] **Contest 10** (Target: ABC 422)

**[ 11–20 ] — Refining Speed**

- [ ] **Contest 11** (Target: ABC 423)
- [ ] **Contest 12** (Target: ABC 424)
- [ ] **Contest 13** (Target: ABC 425)
- [ ] **Contest 14** (Target: ABC 426)
- [ ] **Contest 15** (Target: ABC 427)
- [ ] **Contest 16** (Target: ABC 428)
- [ ] **Contest 17** (Target: ABC 429)
- [ ] **Contest 18** (Target: ABC 430)
- [ ] **Contest 19** (Target: ABC 431)
- [ ] **Contest 20** (Target: ABC 432)

**[ 21–30 ] — Mastery & Accuracy**

- [ ] **Contest 21** (Target: ABC 433)
- [ ] **Contest 22** (Target: ABC 434)
- [ ] **Contest 23** (Target: ABC 435)
- [ ] **Contest 24** (Target: ABC 436)
- [ ] **Contest 25** (Target: ABC 437)
- [ ] **Contest 26** (Target: ABC 438)
- [ ] **Contest 27** (Target: ABC 439)
- [ ] **Contest 28** (Target: ABC 440)
- [ ] **Contest 29** (Target: ABC 441)
- [ ] **Contest 30** (Target: ABC 442)

---
### Codeforces Gym Counter 

_5-hour simulations to build endurance._

- [ ] **Gym Session #1**
- [ ] **Gym Session #2**
- [ ] **Gym Session #3**
- [ ] **Gym Session #4**
---
## Week 1: Strings, Hashing & Final Polish

### Day 1: Standard Rolling Hashing

- [x] **Session Khalwsh**
- [x] **Session Guc**
- [x] Solve 5 problems Hashing**
- [x] **Atcoder Contest**
---

### ## Day 2: XOR & Multiset Hashing

- [ ] **Menoufia Hashing Sheet**
- [ ] **Ultimate Topic List Sheet**
- [ ] **Upsolve Atcoder Contest**
- [ ] **Atcoder Contest**

---

### ## Day 3: KMP (Knuth-Morris-Pratt)

_Goal: Understand the "failure function" and prefix matching._

- [ ] **The $\pi$ Array:** Write the `compute_prefix_function(S)` that builds the $\pi[i]$ array in $O(N)$.
- [ ] **Manual Trace:** Hand-trace the $\pi$ array for `abababc` to ensure you understand the "jump back" logic.
- [ ] **String Periodicity:** Use the property that if $n$ is divisible by $n - \pi[n-1]$, then $n - \pi[n-1]$ is the length of the smallest repeating unit
- [ ] **Pattern Matching:** Implement the full KMP search to find all occurrences of pattern $P$ in text $T$.

---

### ## Day 4: Z-Algorithm

_Goal: Linear time LCP (Longest Common Prefix) for all suffixes._

- [ ] **Z-Array Construction:** Implement the $O(N)$ Z-algorithm using the "box" ($[L, R]$) optimization.
- [ ] **The Concatenation Trick:** Practice the $S = P + \# + T$ construction to solve pattern matching problems.
- [ ] **Prefix-Suffix LCP:** Solve a problem that requires finding the longest prefix of $S$ that is also a suffix starting at index $i$.
- [ ] **Compare and Contrast:** Note one scenario where Z-algorithm is easier to code than KMP (usually when dealing with LCP).

---

### ## Day 5: Mixed Practice & Decision Making

_Goal: Developing the intuition to pick the right algorithm._

- [ ] **Constraint Analysis:** Practice identifying when $O(N)$ (KMP/Z) is required vs. when $O(N \log N)$ (Hash + Binary Search) is acceptable.
- [ ] **2D Rolling Hash:** (Advanced) Implement a hash for a 2D grid to find a sub-grid pattern.
- [ ] **Large Prime Optimization:** Update your templates to use `__int128` or custom modulo math to prevent overflows without losing speed.

---

### ## Day 6-7: Template Refinement

_Goal: Hardening your code for the "Heat of Battle."_

- [ ] **`StringHash` Class:** Wrap your hashing logic into a reusable class with `get_hash(l, r)` methods.
- [ ] **KMP State Machine:** Implement KMP as a transition table (Automaton). This is a lifesaver for String DP problems.
- [ ] **Stress Tester:** Write a simple script that generates random strings and compares your KMP/Hash results against a brute-force $O(N^2)$ approach to catch edge cases.
---

## Week 2: Tree Fundamentals & Techniques

_Moving from linear strings to hierarchical structures._

- [ ] **Day 8: Tree Diameter** (Two DFS or DP approach)
- [ ] **Day 9: Euler Tour Technique (ETT)** (Flattening for subtree queries)
- [ ] **Day 10: LCA (Lowest Common Ancestor)** (Binary Lifting & RMQ)
- [ ] **Day 11: Heavy-Light Decomposition (HLD)** (Path queries)
- [ ] **Day 12: Centroid Decomposition** (Divide and conquer on trees)
- [ ] **Day 13: DSU on Tree (Sack)** (Small-to-large merging)
- [ ] **Day 14: Review & Practice** (Focus on HLD/Centroid)
---

## Week 3: Graph Foundations & Connectivity

_Connectivity, traversal optimizations, and pathfinding._

- [ ] **Day 15: DFS Tree & SCCs** (Tarjan’s or Kosaraju’s)
- [ ] **Day 16: Bridges & Articulation Points** (The "Black Box" logic)
- [ ] **Day 17: Bridge Tree & Block Cut Tree** (Graph condensation)
- [ ] **Day 18: MST (Kruskal’s) & 0/1 BFS**
- [ ] **Day 19: Eulerian Path & SPFA**
- [ ] **Day 20-21: Review & Practice** (Focus on SCC/BCT problems)

---

## Week 4: Advanced Data Structures & Mo's

_Techniques for handling complex queries efficiently._

- [ ] **Day 22: SQRT Decomposition** (The manual "Block" approach)
- [ ] **Day 23: Standard Mo’s Algorithm** (Offline range queries)
- [ ] **Day 24: Rollback Mo & Mo's with DSU**
- [ ] **Day 25: Mo's on Tree** (Combining Mo with ETT)
- [ ] **Day 26: Persistent Segment Tree** (Queries over history)
- [ ] **Day 27-28: Mixed Problem Solving** (DS + Graph/Tree combined)
- [ ] **Day 29-30: Final Review** (Clean up all templates)

