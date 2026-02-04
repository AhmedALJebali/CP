int ceil_div(int a, int b) {
  if (b < 0) a = -a, b = -b;
  if (a >= 0) return (a + b - 1) / b;
  return a / b;
}

int floor_div(int a, int b) {
  if (b < 0) a = -a, b = -b;
  if (a >= 0) return a / b;
  return -((-a + b - 1) / b);
}