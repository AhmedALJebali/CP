void erase_ranges_containing(int i, set<pair<int,int> > &st) {
    auto it = st.upper_bound({i, INT_MAX});
    if (it != st.begin()) {
        --it;
    } else {
        return;
    }
    while (true) {
        int l = it->first;
        int r = it->second;
        if (l <= i && i <= r) {
            // erase safely
            auto toErase = it;
            if (it == st.begin()) {
                st.erase(toErase);
                return;
            }
            --it;
            st.erase(toErase);
        } else {
            break;
        }

        if (it == st.end()) break;
    }
}
