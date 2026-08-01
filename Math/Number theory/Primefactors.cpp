vector<int> PrimeFactors(int n) {
  vector<int> ret;
  for (int i = 2; i * i <= n; i++) { 
    while (n % i == 0) {
      ret.push_back(i);
      n = n / i;
    }
  }
  if (n > 1) { 
    ret.push_back(n);
  }
  return ret;
}
