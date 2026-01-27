
## Introduction

A **Segment Tree** is a **powerful data structure** used to handle queries on arrays efficiently, especially when you have **frequent range queries** and **point updates**.

**Example problem:**

- You have an array of numbers, and you want to **compute the sum of any subarray quickly**, or **update a value in the array**.

Without a Segment Tree, each query could take (O(n)). With a Segment Tree, both updates and range queries can be done in (O(\log n)).

---
## Core Idea

1. Think of the array as a **binary tree**:
    - Each **leaf node** represents a single element of the array.
    - Each **internal node** represents the **merged result** of its children (sum, max, min, etc.).
2. **Merge operation**:
    - Each node knows how to merge the results of its children.
    - Example: sum, maximum, or minimum.
3. **Range queries:
    - Traverse the tree and combine only nodes that fully or partially cover the range.
    - Merge the results to get the final answer.
4. **Point updates**:
    - Update the leaf node.
    - Recalculate the values of its ancestors to reflect the change.

---

## Segment Tree in C++

Here’s a generic implementation in C++ for range sum queries:

```cpp
struct Node {
    int val;
    Node() { val = 0; }
    Node(int x) { val = x; }
    void change(int x) { val = x; }
};

struct segment_tree {
    int tree_size;
    vector<Node> segData;

    segment_tree(int n) {
        tree_size = 1;
        while (tree_size < n) tree_size *= 2;
        segData.assign(2 * tree_size, Node());
    }

    Node merge(Node &left, Node &right) {
        Node res;
        res.val = left.val + right.val; // merge operation: sum
        return res;
    }

    void init(vector<int> &a, int node, int lx, int rx) {
        if (rx - lx == 1) {
            if (lx < (int)a.size())
                segData[node] = Node(a[lx]);
            return;
        }
        int mid = (lx + rx) / 2;
        init(a, 2*node+1, lx, mid);
        init(a, 2*node+2, mid, rx);
        segData[node] = merge(segData[2*node+1], segData[2*node+2]);
    }

    void init(vector<int> &a) { init(a, 0, 0, tree_size); }

    Node get(int l, int r, int node, int lx, int rx) {
        if (lx >= r || rx <= l) return Node();
        if (lx >= l && rx <= r) return segData[node];
        int mid = (lx + rx) / 2;
        Node left = get(l, r, 2*node+1, lx, mid);
        Node right = get(l, r, 2*node+2, mid, rx);
        return merge(left, right);
    }

    int get(int l, int r) { return get(l, r, 0, 0, tree_size).val; }

    void update(int idx, int val, int node, int lx, int rx) {
        if (rx - lx == 1) {
            segData[node].change(val);
            return;
        }
        int mid = (lx + rx) / 2;
        if (idx < mid)
            update(idx, val, 2*node+1, lx, mid);
        else
            update(idx, val, 2*node+2, mid, rx);
        segData[node] = merge(segData[2*node+1], segData[2*node+2]);
    }

    void update(int idx, int val) { update(idx, val, 0, 0, tree_size); }
};
```

---

## How It Works

1. **Initialization**:

```cpp
vector<int> a = {1, 2, 3, 4, 5};
segment_tree st(a.size());
st.init(a);
```

2. **Range query** (sum of elements from index 1 to 4):

```cpp
cout << st.get(1, 4); // Output: 2 + 3 + 4 = 9
```

3. **Point update** (change element at index 2 to 10):

```cpp
st.update(2, 10);
cout << st.get(1, 4); // Output: 2 + 10 + 4 = 16
```

