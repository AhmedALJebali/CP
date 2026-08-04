int get_cot_coprimes(int x, vector<int> &mul) {
  vector<int> p;
  while (x > 1) {
    int prime = spf[x];
    p.push_back(prime);
    while (x % prime == 0) {
      x /= prime;
    }
  }
  int nc = 0;
  int msk = 1LL << p.size();
  for (int j = 1; j < msk; j++) {
    int lcm = 1;
    for (int idx = 0; idx < p.size(); idx++) {
      if (j >> idx & 1) lcm *= p[idx];
    }
    if (__builtin_popcountll(j) & 1) nc += mul[lcm];
    else nc -= mul[lcm];
  }
  return nc;
}
