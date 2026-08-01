vector<int> PrimeFactors(int n) {
  vector<int>ret;
  for (int i = 2; i * i <= n; i = i + 2) {
    while (n % i == 0) {
      ret.push_back(i);
      n = n / i;
    }
  }
  if (n > 2) {
    ret.push_back(n);
  }
  return ret;
}
