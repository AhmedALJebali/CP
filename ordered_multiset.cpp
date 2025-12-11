#include <ext/pb_ds/assoc_container.hpp>
using namespace __gnu_pbds;
typedef tree<
    pair<int,int>,
    null_type,
    less<pair<int,int> >,
    rb_tree_tag,
    tree_order_statistics_node_update
> ordered_multiset;

ordered_multiset st;
int timer = 0;

void insert_value(int x) {
    st.insert({x, timer++});
}

int count_range(int l, int r) {
    return st.order_of_key({r + 1, 0}) - st.order_of_key({l, 0});
}
