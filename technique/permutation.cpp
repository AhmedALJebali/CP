vector<int> nthPerm(int len, int nth) {
  vector<int> identity(len), perm(len);
  for (int i = 0; i < len; ++i) {
    identity[i] = i;
  }
  for (int i = len - 1; i >= 0; --i) {
    int p = nth / Fact[i];
    perm[len - 1 - i] = identity[p];
    identity.erase(identity.begin() + p);
    nth %= Fact[i];
  }
  return perm;
}
// Given a permutation, what is its index?
int PermToIndex(vector<int> perm) {
  int idx = 0;
  int n = perm.size();
  for (int i = 0; i < n; ++i) {
    idx += Fact[n - i - 1] * perm[i];
    for (int j = i + 1; j < n; j++) {
      perm[j] -= perm[j] > perm[i];
    }
  }
  return idx;
}
using perm = std::vector<int>;
perm multiply(const perm& A, const perm& B) {
  int n = A.size();
  perm C(n);
  for (int i = 0; i < n; ++i) {
    C[i] = B[A[i]]; 
  }
  return C;
}

perm pow(perm inp, perm apply, int k) {
  if (k == 0) {
    return inp;
  }
  while (k > 0) {
    if (k % 2 == 1) {
      inp = multiply(inp, apply);
    }
    apply = multiply(apply, apply);
    k /= 2;
  }
  return inp;
}
perm pow_cycle(perm inp, const perm& apply, int k) {
  int n = apply.size();
  perm apply_k(n);
  vector<bool> visited(n, false);
  for (int i = 0; i < n; ++i) {
    if (!visited[i]) {
      vector<int> cycle;
      int current = i;
      while (!visited[current]) {
        visited[current] = true;
        cycle.push_back(current);
        current = apply[current];
      }
      int L = cycle.size();
      int steps = k % L; 
      for (int j = 0; j < L; ++j) {
        apply_k[cycle[j]] = cycle[(j + steps) % L];
      }
    }
  }
  return multiply(inp, apply_k);
}
perm inverse(const perm& p) {
    int n = p.size();
    perm inv(n);
    for (int i = 0; i < n; i++)
        inv[p[i]] = i;
    return inv;
}
vector<vector<int>> getCycles(const perm& p) {
    int n = p.size();
    vector<bool> vis(n);
    vector<vector<int>> cycles;
    for (int i = 0; i < n; i++) {
        if (vis[i]) continue;
        vector<int> cyc;
        int u = i;
        while (!vis[u]) {
            vis[u] = true;
            cyc.push_back(u);
            u = p[u];
        }
        cycles.push_back(cyc);
    }
    return cycles;
}
bool isEvenPermutation(const perm& p) {
    int n = p.size();
    vector<bool> vis(n);
    int cycles = 0;
    for (int i = 0; i < n; i++) {
        if (vis[i]) continue;
        cycles++;
        int u = i;
        while (!vis[u]) {
            vis[u] = true;
            u = p[u];
        }
    }
    return ((n - cycles) % 2 == 0);
}
// The winner is predetermined by the initial minimum swaps needed to sort the array (Total Elements - Number of Cycles).
int minSwapsToSort(vector<int> arr) {
  int n = arr.size();
  vector<int> sorted_arr = arr;
  sort(sorted_arr.begin(), sorted_arr.end());
  map<int, int> correct_pos;
  for (int i = 0; i < n; ++i) {
    correct_pos[sorted_arr[i]] = i;
  }
  vector<bool> visited(n, false);
  int swaps = 0;
  for (int i = 0; i < n; ++i) {
    if (visited[i] || correct_pos[arr[i]] == i) {
      continue;
    }
    int cycle_length = 0;
    int current = i;
    while (!visited[current]) {
      visited[current] = true;
      current = correct_pos[arr[current]]; 
      cycle_length++;
    }
    if (cycle_length > 0) {
      swaps += (cycle_length - 1);
    }
  }
  return swaps;
}
// Permutation Order is the minimum number of applications required to return an entire array back to its original state, calculated by finding the Least Common Multiple (LCM) of all individual cycle lengths.
int getPermutationOrder(const perm& p) {
  int n = p.size();
  vector<bool> visited(n, false);
  int permutation_order = 1;
  for (int i = 0; i < n; ++i) {
    if (!visited[i]) {
      int cycle_length = 0;
      int current = i;
      while (!visited[current]) {
        visited[current] = true;
        current = p[current];
        cycle_length++;
      }
      permutation_order = lcm(permutation_order, cycle_length);
    }
  }
  return permutation_order;
}
// The number of permutations of n distinct elements that consist of exactly k disjoint cycles.
int stirlingFirstOptimized(int n, int k) {
  if (k < 0 || k > n) return 0;
  vector<int> dp(k + 1, 0);
  dp[0] = 1;
  for (int i = 1; i <= n; i++) {
    int upto = min(i, k);
    for (int j = upto; j >= 1; j--) {
      dp[j] = (i - 1) * dp[j] + dp[j - 1];
    }
    dp[0] = 0;
  }
  return dp[k];
}
// Compute permutations with specific cycle lengths
vector<int> specific_cycle_lengths(int max_n, const vector<int>& allowed_lengths) {
  vector<int> f(max_n + 1, 0);
  f[0] = 1; 
  for (int n = 1; n <= max_n; n++) {
    int tot = 0;
    for (int p : allowed_lengths) {
      if (p > n) continue; 
      int ways = (nPrMod(n - 1, p - 1) * f[n - p]) % MOD;
      tot = (tot + ways) % MOD;
    }
    f[n] = tot;
  }
  return f;
}
vector<int> specific_cycle_lengths_with_k_cycles_all_n(int n, int k, const vector<int>& allowed_lengths, bool k_or_less) {
  vector<int> res(n + 1, 0);
  if (k < 0) return res;
  if (k == 0) {
    res[0] = 1;
    return res;
  }
  vector<int> prev_dp(n + 1, 0);
  prev_dp[0] = 1;
  if (k_or_less) {
    res[0] = 1; 
  }
  for (int j = 1; j <= k; j++) {
    vector<int> curr_dp(n + 1, 0);
    for (int i = 1; i <= n; i++) {
      int tot = 0;
      for (int p : allowed_lengths) {
        if (p > i) continue;
        int ways = (1LL * nPrMod(i - 1, p - 1) * prev_dp[i - p]) % MOD;
        tot = (tot + ways) % MOD;
      }
      curr_dp[i] = tot;
    }
    prev_dp = curr_dp;
    if (k_or_less) {
      for (int i = 1; i <= n; i++) {
        res[i] = (res[i] + prev_dp[i]) % MOD;
      }
    }
  }
  if (!k_or_less) {
    res = prev_dp;
  }
  return res;
}
vector<int> applyPermutation(const vector<int>& a, const perm& p) {
    int n = a.size();
    vector<T> res(n);
    for (int i = 0; i < n; i++)
        res[p[i]] = a[i];
    return res;
}
vector<int> undoPermutation(const vector<int>& a,const perm& p) {
    return applyPermutation(a, inverse(p));
}
// Counts permutations where p[i] != i for all i
int countDerangements(int n) {
    if (n == 0) return 1;
    if (n == 1) return 0;
    vector<long long> dp(n + 1);
    dp[0] = 1;
    dp[1] = 0;
    for (int i = 2; i <= n; ++i) {
        dp[i] = (i - 1) * (dp[i - 1] + dp[i - 2]); 
    }
    return dp[n];
}
