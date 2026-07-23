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
