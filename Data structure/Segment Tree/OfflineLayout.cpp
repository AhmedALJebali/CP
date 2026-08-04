struct OfflineLayout {
  // Total number of unique elements ever inserted.
  int id_counter = 0;
 
  // Maintains the relative chronological order of all elements ever added.
  // Even if an element is "removed", it stays in this list to reserve its spot in the static layout.
  list<int> order;
 
  // Represents the current "live" state of the array.
  // Maps a current dynamic index to its node in the `order` list.
  vector<list<int>::iterator> active;
 
  // Stores the final mapped index for every unique element ID.
  vector<int> mapped_index;
 
  // Constructor: Pre-fills the structure with `initial_size` elements.
  OfflineLayout(int initial_size = 0) {
    for (int i = 0; i < initial_size; i++) {
      int id = id_counter++;
      order.push_back(id);
      active.push_back(--order.end());
    }
  }
 
  // Simulates inserting an element at dynamic index `pos`.
  // Returns a unique identifier for this specific insertion.
  int insert(int pos) {
    int new_id = id_counter++;
    if (pos == active.size()) {
      // Append to the end
      order.push_back(new_id);
      active.push_back(--order.end());
    } else {
      // Insert in the middle.
      // Note: active.insert() takes O(V) time where V is the current number of active elements.
      auto it = active[pos];
      auto new_it = order.insert(it, new_id);
      active.insert(active.begin() + pos, new_it);
    }
    return new_id;
  }
 
  // Simulates removing the element currently at dynamic index `pos`.
  // Note: It is only removed from `active`. It remains in `order`.
  void remove(int pos) {
    active.erase(active.begin() + pos);
  }
 
  // Finalizes the static layout. MUST be called after all inserts/removes
  // and BEFORE calling id(identifier).
  void build() {
    mapped_index.assign(id_counter, -1);
    int cur = 0;
    for (int id : order) {
      mapped_index[id] = cur++;
    }
  }
 
  // Returns the static 0-based array index for a given element identifier.
  int id(int identifier) {
    return mapped_index[identifier];
  }
 
  // Returns the total number of unique elements (the required size for your Segment Tree).
  int size() {
    return id_counter;
  }
};
