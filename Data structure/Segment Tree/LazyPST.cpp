#include <bits/stdc++.h>
using namespace std;
#define int long long
#define ld long double
#define all(v) v.begin(), v.end()
#define FAST                    \
ios_base::sync_with_stdio(0); \
cin.tie(nullptr);             \
cout.tie(nullptr);
#define fileIO                      \
freopen("input.txt", "r", stdin); \
freopen("out.txt", "w", stdout)

const int N =1e5;
struct Node {
  int l,r,sum,lazy,hasLazy;
  Node(int x=0) {
    sum=x;
    l=r=lazy=hasLazy=0;
  }
} tree[N*40];
int id=0;
int upd(int node,int l,int r,int x,int lx,int rx) {
  if (rx <= l || r <= lx) return node;
  int cur_id=++id;
  tree[cur_id]=tree[node];
  if (l<=lx&& rx<=r) {
    tree[cur_id].sum+=(rx-lx)*x  ;
    tree[cur_id].lazy+=x;
    tree[cur_id].hasLazy=1;
    return cur_id;
  }
  int mid=lx+(rx-lx)/2;
  tree[cur_id].l=upd(tree[node].l,l,r,x,lx,mid);
  tree[cur_id].r=upd(tree[node].r,l,r,x,mid,rx);
  tree[cur_id].sum = tree[tree[cur_id].l].sum + tree[tree[cur_id].r].sum +(rx - lx) * tree[cur_id].lazy;
  return cur_id;
}
int get(int node,int l,int r,int lx,int rx,int cur) {
  if (rx <= l || r <= lx) return 0;
  if (l<=lx&& rx<=r) {return  tree[node].sum + (rx-lx)*cur;}
  int mid = lx + (rx - lx) / 2;
  return get(tree[node].l, l, r, lx, mid,  cur + tree[node].lazy) + get(tree[node].r, l, r, mid, rx,  cur + tree[node].lazy);
}
int t=0;
void solve() {
  int n, q;
  cin >> n >> q;
  int st = 0;
  vector<int> a(n);
  for (int i = 0; i < n; i++) {
    cin >> a[i];
    st = upd(st, i, i + 1, a[i], 0, n);
  }
  vector<int> time(q + 5, 0);
  time[0] = st;
  int t = 0;
  while (q--) {
    char typo;
    cin >> typo;

    if (typo == 'Q') {
      int l, r;
      cin >> l >> r;
      l--;
      cout << get(time[t], l, r, 0, n, 0) << "\n";
    } else if (typo == 'C') {
      int l, r, d;
      cin >> l >> r >> d;
      l--;
      time[t + 1] = upd(time[t], l, r, d, 0, n);
      t++;
    } else if (typo == 'B') {
      int back;
      cin >> back;
      t = back;

    } else {
      int l, r, tt;
      cin >> l >> r >> tt;
      l--;
      cout << get(time[tt], l, r, 0, n, 0) << "\n";
    }
  }
}


