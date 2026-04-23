struct SQRT {
  int n, SQ;
  vector<int> Val, Block, lazy;

  SQRT(int n, const vector<int>& v) {
    this->n = n;
    Val = v;
    SQ = max(1LL, (int)sqrt(n));
    int numBlocks = (n + SQ - 1) / SQ;
    Block.assign(numBlocks, LLONG_MIN);
    lazy.assign(numBlocks, 0);

    for (int i = 0; i < n; i++) {
      Block[i / SQ] = max(Block[i / SQ], Val[i]);
    }
  }

  void push(int b) {
    if (lazy[b] == 0) return;
    int L = b * SQ, R = min(n - 1, L + SQ - 1);
    for (int i = L; i <= R; i++) {
      Val[i] += lazy[b];
    }
    lazy[b] = 0;
  }

  void rebuild(int b) {
    int L = b * SQ, R = min(n - 1, L + SQ - 1);
    Block[b] = LLONG_MIN;
    for (int i = L; i <= R; i++) {
      Block[b] = max(Block[b], Val[i]);
    }
  }

  int query(int l, int r) {
    int ret = LLONG_MIN;
    int bl = l / SQ, br = r / SQ;

    for (int b = bl; b <= br; b++) {
      int L = b * SQ, R = min(n - 1, L + SQ - 1);

      if (l <= L && R <= r) {
        ret = max(ret, Block[b]);
      } else {
        for (int i = max(l, L); i <= min(r, R); i++) {
          ret = max(ret, Val[i] + lazy[b]);
        }
      }
    }
    return ret;
  }

  void upd(int l, int r, int val) {
    int bl = l / SQ, br = r / SQ;
    for (int b = bl; b <= br; b++) {
      int L = b * SQ, R = min(n - 1, L + SQ - 1);
      if (l <= L && R <= r) {
        lazy[b] += val;
        Block[b] += val;
      } else {
        push(b);
        for (int i = max(l, L); i <= min(r, R); i++) {
          Val[i] += val;
        }
        rebuild(b);
      }
    }
  }
};