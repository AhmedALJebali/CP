int SQ = 316;
struct Query {
  int l, r, idx;
  bool operator<(const Query &other) const {
    int b1 = l / SQ;
    int b2 = other.l / SQ;
    if (b1 != b2)
      return b1 < b2;
    if (b1 & 1)
      return r > other.r;
    else
      return r < other.r;
  }
};
 auto add = [&](int idx, int i) {
    cur += f[v[idx][i]];
    f[v[idx][0]]++;
  };
  auto erase = [&](int idx, int i) {
    f[v[idx][0]]--;
    cur -= f[v[idx][i]];
  };
 
  for (auto [l, r, idx] : qu) {
    while (R < r) add(++R, 1);
    while (L > l) add(--L, 2);
    while (R > r) erase(R--, 1);
    while (L < l) erase(L++, 2);
    ans[idx] = cur;
  }