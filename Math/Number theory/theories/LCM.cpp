int lcm(int a,int b) {
  return (abs(a) / gcd(a, b)) * abs(b);
}