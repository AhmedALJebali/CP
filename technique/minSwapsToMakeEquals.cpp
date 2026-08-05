int minSwapsToMakeEquals(const vector<int>& a, const vector<int>& b) {
  if (a.size() != b.size()) {
    return -1;
  }
  int n = a.size();
  map<int, queue<int>> target_pos;
  for (int i = 0; i < n; ++i) {
    target_pos[b[i]].push(i);
  }
  vector<int> arr(n);
  for (int i = 0; i < n; ++i) {
    if (target_pos[a[i]].empty()) {
      return -1;
    }
    arr[i] = target_pos[a[i]].front();
    target_pos[a[i]].pop();
  }
  vector<bool> visited(n, false);
  int swaps = 0;
  for (int i = 0; i < n; ++i) {
    if (visited[i] || arr[i] == i) {
      continue;
    }
    int cycle_size = 0;
    int j = i;
    while (!visited[j]) {
      visited[j] = true;
      j = arr[j];
      cycle_size++;
    }
    if (cycle_size > 1) {
      swaps += (cycle_size - 1);
    }
  }

  return swaps;
}
