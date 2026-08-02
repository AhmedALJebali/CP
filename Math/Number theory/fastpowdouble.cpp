// logb(x)=y
// x= b**y

ld fastPowdouble(ld base, ld exponent) {
  int intPart = (int)exponent;
  ld fracPart = exponent - intPart;
  ld intResult = 1.0;
  ld b = base;
  int n = intPart;
  while (n > 0) {
    if (n % 2 == 1) intResult *= b;
    b *= b;
    n /= 2;
  }
  return intResult * exp(fracPart * log(base));
}
