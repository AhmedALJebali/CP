


https://codeforces.com/contest/2112/problem/B // version 2

https://atcoder.jp/contests/abc375/tasks/abc375_d // version 2 string before and after


### **Black and White Tree: The Optimal Root**

**time limit per test:** 2 seconds

**memory limit per test:** 256 megabytes

You are given a tree of $n$ nodes, where every node is colored either black or white.

You will be given $q$ queries. Each query consists of a single node $u$. For each query, you are allowed to temporarily choose **any** node in the tree to be the root.

After choosing a root, your task is to count how many nodes $v$ exist that satisfy both of the following conditions:

1. Node $v$ is located in the subtree of node $u$.
2. The simple path from $u$ to $v$ (including both $u$ and $v$) contains an exactly equal number of black and white nodes.

For each query, output the **maximum** possible number of such valid nodes $v$ you can obtain by choosing the root of the tree optimally.

##### **Input**

The first line contains two integers $n$ and $q$ ($1 \le n \le 10^5$, $1 \le q \le 10^5$) — the number of nodes in the tree and the number of queries, respectively.

The second line contains $n$ integers $c_1, c_2, \dots, c_n$ ($0 \le c_i \le 1$), where $c_i = 0$ means that the $i$-th node is white, and $c_i = 1$ means that the $i$-th node is black.

Each of the next $n - 1$ lines describes an edge of the tree. The $i$-th edge is denoted by two integers $x_i$ and $y_i$, the labels of the nodes it connects ($1 \le x_i, y_i \le n$, $x_i \ne y_i$). It is guaranteed that the given edges form a valid tree.

The next $q$ lines contain the queries. The $j$-th line contains one integer $u$ ($1 \le u \le n$) — the target node for the query.

##### **Output**

Print $q$ integers — the maximum number of valid nodes $v$ for each query, printed in the order they appear in the input.

### **B2. Hungry (Hard Version)**

**New Problem Statement:**

Hendy is hungry and needs to eat $q$ meals over the next $q$ days. The shop's queue of $n$ customers is the same every day, but Hendy's location persists between days.

Hendy starts at **Home**. For each day $i$, he receives three parameters $x_i, y_i, m_i$ and must choose one of two options:

1. **Delivery:** Hendy gets his food at Home. The shop serves $m_i$ people, then delivers the food.
2. **Shop:** Hendy gets his food at the Shop. He waits for all $n$ customers to be served.

**The Dependency (Location State):**

- Traveling between Home and the Shop takes $x_i$ minutes.
- If Hendy is at Home and chooses the Shop, he must travel $x_i$ minutes. He receives his food at time $\max(x_i, P[n])$. He is now at the **Shop**.
- If Hendy is already at the Shop and chooses the Shop again, his travel time is $0$. He receives his food at time $P[n]$. He remains at the **Shop**.
- If Hendy is at the Shop and chooses Delivery, he must walk back Home while the food is being prepared/delivered. He receives his food at time $\max(x_i, P[m_i] + y_i)$. He is now at **Home**.
- If Hendy is at Home and chooses Delivery, he just waits. He receives his food at time $P[m_i] + y_i$. He remains at **Home**.

**Goal:** Find the minimum **total** wait time across all $q$ days.



### **L. The Hell Boy Returns (Modulo Madness)**

_time limit per test: 1 second_

_memory limit per test: 256 megabytes_

The Heaven Boy was too easy for you, so The Hell Boy is back with his ultimate challenge!

You are given an array $a$ consisting of $n$ positive integers, and an integer $m$. Your task is to calculate the sum of the **sums** of all non-empty subsets of array $a$ such that the sum of the elements in the subset is **strictly divisible by** $m$.

Since the answer can be extremely large, print it modulo $10^9 + 7$.

**Input**

The first line contains a single integer $t$ ($1 \le t \le 100$) — the number of test cases.

The first line of each test case contains two integers $n$ and $m$ ($1 \le n \le 1000$, $1 \le m \le 1000$) — the size of the array $a$ and the divisibility condition.

The second line of each test case contains $n$ integers $a_1, a_2, \dots, a_n$ ($1 \le a_i \le 10^9$) — the elements of the array $a$.

It is guaranteed that the sum of $n \cdot m$ over all test cases does not exceed $2 \cdot 10^6$.

**Output**

For each test case, print a single integer — the sum of the sums of all valid non-empty subsets modulo $10^9 + 7$.

---


```cpp
#include <bits/stdc++.h>
using namespace std;

#define int long long
#define FAST                    \
ios_base::sync_with_stdio(0); \
cin.tie(nullptr);

const int MOD = 1e9 + 7;
struct State {
    int count;
    int sum;
};
int n, m;
int val[1005];
State dp[1005][1005];
bool vis[1005][1005];


State rec(int i, int rem) {
    if (i == n) {
        if (rem == 0) return {1, 0};
        return {0, 0}; 
    }
    if (vis[i][rem]) return dp[i][rem];
    State leave = rec(i + 1, rem);
    State take = rec(i + 1, (rem + val[i]) % m);
    State ret = {0, 0};
    ret.count = (leave.count + take.count) % MOD;
    int take_sum = (take.sum + (val[i] % MOD) * take.count) % MOD;
    ret.sum = (leave.sum + take_sum) % MOD;
    vis[i][rem] = true;
    return dp[i][rem] = ret;
}

void solve() {
    cin >> n >> m;
    for (int i = 0; i < n; i++) {
        cin >> val[i];
    }
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= m; j++) {
            vis[i][j] = false;
        }
    }
    State ans = rec(0, 0);
    cout << ans.sum << endl;
}

signed main() {
    FAST;
    int t = 1;
    cin >> t;
    for (int i = 1; i <= t; i++) {
        solve();
    }
    return 0;
}
```

### **K. The Heaven Boy**

_time limit per test: 1 second_

_memory limit per test: 256 megabytes_

After successfully solving the multiplication challenge, you thought you could rest. However, a new task has appeared!

You are given an array $a$ consisting of $n$ positive integers. Your task is to calculate the sum of the **sums** of all non-empty subsets of array $a$.

A non-empty subset of an array $a$ is defined as a set of elements that can be obtained by deleting zero or more elements from the original array $a$, ensuring at least one element remains.

Since the answer can be extremely large, print it modulo $10^9 + 7$.

**Input**

The first line contains a single integer $t$ ($1 \le t \le 10^4$) — the number of test cases.

The first line of each test case contains an integer $n$ ($1 \le n \le 10^5$) — the size of the array $a$.

The second line of each test case contains $n$ integers $a_1, a_2, \dots, a_n$ ($1 \le a_i \le 10^9$) — the elements of the array $a$.

It is guaranteed that the sum of $n$ over all test cases does not exceed $2 \cdot 10^5$.

**Output**

For each test case, print a single integer — the sum of the sums of all non-empty subsets of array $a$ modulo $10^9 + 7$.

**Example**

**Input**

Plaintext

```
2
3
1 2 3
2
10 20
```

**Output**

Plaintext

```
24
60
```

**Note**

In the first test case, the array $a$ has $7$ non-empty subsets. The answer is calculated as follows:

- $\{1\} \rightarrow$ sum is $1$
- $\{2\} \rightarrow$ sum is $2$
- $\{3\} \rightarrow$ sum is $3$
- $\{1, 2\} \rightarrow$ sum is $3$
- $\{1, 3\} \rightarrow$ sum is $4$
- $\{2, 3\} \rightarrow$ sum is $5$
- $\{1, 2, 3\} \rightarrow$ sum is $6$

Total sum = $1 + 2 + 3 + 3 + 4 + 5 + 6 = 24$.
### **Total Visibility Over Segments**

You are given an array $A$ consisting of $N$ integers.

You will be given $Q$ queries. Each query provides two indices, $L$ and $R$ (where $L < R$).

For each query, your task is to calculate the total number of valid index pairs $(i, j)$ that satisfy **all three** of the following conditions:

1. **$i$ is inside the query range:** $L \le i \le R$
2. **$j$ is outside and to the right of the query range:** $R < j \le N$
3. **The "Clear Path" Condition:** The value at $A_j$ must be **strictly greater** than every single number located between index $i$ and index $j$.
       - _Mathematically: For every index $x$ where $i < x < j$, it must be true that $A_x < A_j$._

---
#### **Constraints**
- $3 \le N \le 2 \times 10^5$
- $1 \le Q \le 2 \times 10^5$
- $1 \le A_x \le N$ for all $1 \le x \le N$
- $1 \le L_q < R_q < N$
- All input values are integers.
---
#### **Input**

The input is given from Standard Input in the following format:
$N$ $Q$
$A_1$ $A_2$ $\dots$ $A_N$
$L_1$ $R_1$
$L_2$ $R_2$
$L_Q$ $R_Q$
#### **Output**

Print $Q$ lines. The $q$-th line should contain a single integer: the number of valid pairs $(i, j)$ for the $q$-th query.



