const ld EPS = 1e-9;
const ld PI = acos(-1.0L);

typedef ld T;
typedef complex<T> pt;
#define x real()
#define y imag()

// ==========================================
// --- 1. BASIC OPERATIONS ---
// ==========================================
T sq(pt p) { return p.x * p.x + p.y * p.y; }
T dot(pt v, pt w) { return v.x * w.x + v.y * w.y; } 
T cross(pt v, pt w) { return v.x * w.y - v.y * w.x; } 
int sgn(T val) { if(val > EPS) return 1; if(val < -EPS) return -1; return 0; } 
bool same(pt a, pt b) { return abs(a - b) <= EPS; }
T dist2(pt a, pt b) { return norm(a - b); }
pt perp_ccw(pt p) { return {-p.y, p.x}; }

// ==========================================
// --- 2. ANGLES & ORIENTATION ---
// ==========================================
T toDegrees(T rad) { return rad * (180.0L / PI); }
T toRadians(T deg) { return deg * (PI / 180.0L); }
T orient(pt a, pt b, pt c) { return cross(b - a, c - a); } 
T angle(pt v, pt w) { return acos(clamp(dot(v, w) / abs(v) / abs(w), (T)-1.0, (T)1.0)); }

T orientedAngle(pt a, pt b, pt c) {
  if (orient(a,b,c) >= 0)
    return angle(b-a, c-a);
  else
    return 2*PI - angle(b-a, c-a);
}

T angleTravelled(pt a, pt b, pt c) {
    T ampli = angle(b - a, c - a);
    if (sgn(orient(a, b, c)) > 0) return ampli;
    if (sgn(orient(a, b, c)) < 0) return -ampli;
    return (sgn(dot(b - a, c - a)) >= 0) ? (T)0 : PI;
}

// ==========================================
// --- 3. COLLINEARITY & SEGMENTS ---
// ==========================================
bool collinear(pt a, pt b, pt c) {
    return sgn(orient(a, b, c)) == 0;
}

bool onSegment(pt a, pt b, pt p) {
    return sgn(orient(a, b, p)) == 0 && sgn(dot(p - a, p - b)) <= 0;
}

// ==========================================
// --- 4. POLYGON BASICS ---
// ==========================================
T signedAreaPolygon(const vector<pt>& p) {
    int n = p.size(); T area = 0;
    for (int i = 0; i < n; i++) area += cross(p[i], p[(i + 1) % n]);
    return area / 2.0L;
}

T areaPolygon(const vector<pt>& p) { return fabsl(signedAreaPolygon(p)); }

T perimeterPolygon(const vector<pt>& p) {
    int n = p.size(); if (n < 2) return 0;
    T per = 0;
    for (int i = 0; i < n; i++) per += abs(p[(i + 1) % n] - p[i]);
    return per;
}

bool isConvex(const vector<pt>& p, bool strict = false) {
    int n = p.size(); if (n < 3) return false;
    int dir = 0;
    for (int i = 0; i < n; i++) {
        T cr = cross(p[(i + 1) % n] - p[i], p[(i + 2) % n] - p[(i + 1) % n]);
        int s = sgn(cr);
        if (s == 0) {
            if (strict) return false;
            continue;
        }
        if (dir == 0) dir = s;
        else if (dir != s) return false;
    }
    return true;
}

void normalizeCCW(vector<pt>& p) {
    if (sgn(signedAreaPolygon(p)) < 0) reverse(p.begin(), p.end());
}

// Rotates polygon so the lowest (then leftmost) point becomes p[0]
void reorderConvex(vector<pt>& p) {
    int pos = 0;
    for (int i = 1; i < (int)p.size(); i++) {
        if (sgn(p[i].y - p[pos].y) < 0 || (sgn(p[i].y - p[pos].y) == 0 && sgn(p[i].x - p[pos].x) < 0)) {
            pos = i;
        }
    }
    rotate(p.begin(), p.begin() + pos, p.end());
}

// ==========================================
// --- 5. CONVEX HULL (Graham Scan) ---
// ==========================================
bool cw(pt a, pt b, pt c, bool col) {
    int o = sgn(orient(a, b, c));
    return o < 0 || (col && o == 0);
}

void convex_hull(vector<pt>& a, bool include_collinear = true) {
    pt p0 = *min_element(a.begin(), a.end(), [](pt a, pt b) {
        return make_pair(a.y, a.x) < make_pair(b.y, b.x);
    });
    sort(a.begin(), a.end(), [&p0](const pt& a, const pt& b) {
        int o = sgn(orient(p0, a, b));
        if (o == 0)
            return (p0.x-a.x)*(p0.x-a.x) + (p0.y-a.y)*(p0.y-a.y)
                   < (p0.x-b.x)*(p0.x-b.x) + (p0.y-b.y)*(p0.y-b.y);
        return o < 0;
    });
    if (include_collinear) {
        int i = (int)a.size()-1;
        while (i >= 0 && collinear(p0, a[i], a.back())) i--;
        reverse(a.begin()+i+1, a.end());
    }

    vector<pt> st;
    for (int i = 0; i < (int)a.size(); i++) {
        while (st.size() > 1 && !cw(st[st.size()-2], st.back(), a[i], include_collinear))
            st.pop_back();
        if(st.empty() || a[i] != st.back())
            st.push_back(a[i]);
    }

    if (include_collinear == false && st.size() == 2 && st[0] == st[1])
        st.pop_back();

    a = st;
}

// ==========================================
// --- 6. ADVANCED POLYGON ALGORITHMS ---
// ==========================================

bool pointInConvexPolygon(const vector<pt>& poly, pt q, bool strict = true) {
    int n = poly.size();
    if (n == 0) return false;
    if (n == 1) return !strict && same(poly[0], q);
    if (n == 2) return !strict && onSegment(poly[0], poly[1], q);

    // The rest of this function assumes poly is CCW. Detect the actual
    // orientation once — O(1), since a strict convex polygon can't have
    // poly[0], poly[1], poly[2] collinear — and fold the sign into every
    // cross-product test below, so this now works for CW input too.
    T dir = (sgn(orient(poly[0], poly[1], poly[2])) >= 0) ? (T)1 : (T)-1;
    auto cx = [&](pt v, pt w) { return dir * cross(v, w); };

    if (sgn(cx(poly[1] - poly[0], q - poly[0])) < 0) return false;
    if (sgn(cx(poly[n - 1] - poly[0], q - poly[0])) > 0) return false;

    if (onSegment(poly[0], poly[1], q) || onSegment(poly[0], poly[n - 1], q)) return !strict;

    int l = 1, r = n - 1;
    while (r - l > 1) {
        int m = (l + r) / 2;
        if (sgn(cx(poly[m] - poly[0], q - poly[0])) >= 0) l = m;
        else r = m;
    }

    auto cr = cx(poly[l + 1] - poly[l], q - poly[l]);
    if (sgn(cr) < 0) return false;
    if (sgn(cr) == 0) return !strict;

    if (sgn(cx(poly[l] - poly[0], q - poly[0])) == 0) {
        if (sgn(cross(poly[1] - poly[0], poly[l] - poly[0])) == 0 ||
            sgn(cross(poly[n - 1] - poly[0], poly[l] - poly[0])) == 0) {
            return !strict;
        }
    }
    return true;
}

// Minkowski Sum of two Convex Polygons in O(N + M)
vector<pt> minkowski(vector<pt> P, vector<pt> Q) {
    reorderConvex(P);
    reorderConvex(Q);
    int n = P.size(), m = Q.size();
    P.push_back(P[0]); P.push_back(P[1]); // Padding to avoid modulo bounds
    Q.push_back(Q[0]); Q.push_back(Q[1]);
    
    vector<pt> res;
    int i = 0, j = 0;
    while (i < n || j < m) {
        res.push_back(P[i] + Q[j]);
        T cr = cross(P[i+1] - P[i], Q[j+1] - Q[j]);
        if (sgn(cr) >= 0 && i < n) i++;
        if (sgn(cr) <= 0 && j < m) j++;
    }
    return res;
}

// Diameter of Convex Polygon (Rotating Calipers) - O(N)
pair<pair<pt, pt>, T> convexDiameter(const vector<pt>& poly) {
    int n = poly.size();
    if (n < 2) return {{pt(0, 0), pt(0, 0)}, 0};
    if (n == 2) return {{poly[0], poly[1]}, abs(poly[1] - poly[0])};

    int j = 1; T best = 0; pair<pt, pt> ans = {poly[0], poly[0]};
    for (int i = 0; i < n; i++) {
        int ni = (i + 1) % n;
        // Advance j while the triangle area strictly increases
        while (sgn(cross(poly[ni] - poly[i], poly[(j + 1) % n] - poly[i]) - 
                   cross(poly[ni] - poly[i], poly[j] - poly[i])) > 0) {
            j = (j + 1) % n;
        }
        T d1 = abs(poly[i] - poly[j]);
        if (d1 > best) { best = d1; ans = {poly[i], poly[j]}; }
        
        T d2 = abs(poly[ni] - poly[j]);
        if (d2 > best) { best = d2; ans = {poly[ni], poly[j]}; }
    }
    return {ans, best};
}

// ==========================================
// --- 7. HALF-PLANE INTERSECTION ---
// ==========================================
struct Halfplane {
    pt p, pq; T angle;
    Halfplane() {}
    Halfplane(pt a, pt b) : p(a), pq(b - a) {
        angle = atan2(imag(pq), real(pq));
    }
    
    // Checks if point 'r' is strictly outside (to the right of) the half-plane
    bool out(pt r) const { return cross(pq, r - p) < -EPS; }

    bool operator<(const Halfplane& other) const {
        if (sgn(angle - other.angle) == 0) {
            // Resolve parallel planes: keep the one that is most restrictive (tightest)
            return cross(pq, other.p - p) < 0; 
        }
        return angle < other.angle;
    }

    friend pt inter(const Halfplane& a, const Halfplane& b) {
        T A = cross(b.p - a.p, b.pq) / cross(a.pq, b.pq);
        return a.p + a.pq * A;
    }
};

vector<pt> hp_intersect(vector<Halfplane> H) {
    const T INF = 1e9;
    vector<pt> box = {{INF, INF}, {-INF, INF}, {-INF, -INF}, {INF, -INF}};
    for (int i = 0; i < 4; i++) H.emplace_back(box[i], box[(i+1)%4]);

    sort(H.begin(), H.end());
    
    // Filter out redundant parallel half-planes
    vector<Halfplane> filtered;
    for (int i = 0; i < (int)H.size(); i++) {
        if (i > 0 && sgn(H[i].angle - H[i-1].angle) == 0) continue; 
        filtered.push_back(H[i]);
    }
    H = filtered;

    deque<Halfplane> dq;
    for (auto &h : H) {
        while (dq.size() > 1 && h.out(inter(dq.back(), dq[dq.size()-2]))) dq.pop_back();
        while (dq.size() > 1 && h.out(inter(dq[0], dq[1]))) dq.pop_front();
        dq.push_back(h);
    }
    while (dq.size() > 2 && dq.front().out(inter(dq.back(), dq[dq.size()-2]))) dq.pop_back();
    while (dq.size() > 2 && dq.back().out(inter(dq[0], dq[1]))) dq.pop_front();

    if (dq.size() < 3) return {};
    
    vector<pt> ret;
    for (int i = 0; i < (int)dq.size(); i++) ret.push_back(inter(dq[i], dq[(i+1)%dq.size()]));
    return ret;
}

// ==========================================
// --- 8. MISC BRUTE FORCE (O(N*M)) ---
// ==========================================
// Note: Maximum distance between two polygons. For large inputs, compute Minkowski(U, -V) instead.
T maximum_dist_from_polygon_to_polygon_brute(vector<pt>& u, vector<pt>& v) {
    T ans = 0;
    for (pt p1 : u) for (pt p2 : v) ans = max(ans, dist2(p1, p2));
    return sqrt(ans);
}


// ==========================================
// --- 9. DYNAMIC CONVEX HULL ---
// ==========================================

struct cmp {
    bool operator()(const pt &a, const pt &b) const {
        if (sgn(a.x - b.x) != 0) return sgn(a.x - b.x) < 0;
        return sgn(a.y - b.y) < 0;
    }
};

struct upper_hull {
    set<pt, cmp> st;
    long long area2 = 0; // Maintains 2 * Area of the upper envelope

    // Helper for cross product (assuming integer coordinates, cast prevents overflow)
    long long cross(pt a, pt b) {
        return 1LL * a.x * b.y - 1LL * a.y * b.x;
    }

    void add_edge(pt a, pt b) {
        area2 += cross(a, b);
    }

    void remove_edge(pt a, pt b) {
        area2 -= cross(a, b);
    }

    auto add_point(pt p) {
        auto it = st.insert(p).first;
        bool has_prev = (it != st.begin());
        bool has_next = (next(it) != st.end());

        if (has_prev && has_next) remove_edge(*prev(it), *next(it));
        if (has_prev) add_edge(*prev(it), p);
        if (has_next) add_edge(p, *next(it));

        return it;
    }

    void remove_point(set<pt, cmp>::iterator it) {
        bool has_prev = (it != st.begin());
        bool has_next = (next(it) != st.end());

        if (has_prev) remove_edge(*prev(it), *it);
        if (has_next) remove_edge(*it, *next(it));
        if (has_prev && has_next) add_edge(*prev(it), *next(it));

        st.erase(it);
    }

    // 0 = outside, 1 = strictly inside, 2 = on boundary
    int is_under(pt p) {
        auto it = st.lower_bound(p);
        if (it == st.end()) return 0;
        if (sgn(it->x - p.x) == 0) return sgn(it->y - p.y) == 0 ? 2 : 1;
        if (it == st.begin()) return 0;
        int o = sgn(orient(p, *it, *prev(it)));
        if (o > 0) return 1;
        if (o == 0) return 2;
        return 0;
    }

    void insert(pt p) {
        if (is_under(p)) return;

        auto it_x = st.lower_bound(pt(p.x, -1e18));
        while (it_x != st.end() && sgn(it_x->x - p.x) == 0) {
            auto nxt = next(it_x);
            remove_point(it_x);
            it_x = nxt;
        }

        auto it = add_point(p);

        while (next(it) != st.end() &&
               next(next(it)) != st.end() &&
               sgn(orient(*it, *next(it), *next(next(it)))) >= 0) {
            remove_point(next(it));
        }
        while (it != st.begin() &&
               prev(it) != st.begin() &&
               sgn(orient(*prev(prev(it)), *prev(it), *it)) >= 0) {
            remove_point(prev(it));
        }
    }

    void erase(pt p) {
        auto it = st.find(p);
        if (it != st.end()) {
            remove_point(it);
        }
    }
};

struct DynamicHull {
    upper_hull upper, lower;

    void insert(pt p) {
        upper.insert(p);
        lower.insert(-p);
    }

    void erase(pt p) {
        upper.erase(p);
        lower.erase(-p);
    }

    // 0 = outside, 1 = strictly inside, 2 = on boundary
    int is_inside(pt p) {
        int u = upper.is_under(p);
        int l = lower.is_under(-p);
        if (!u || !l) return 0;
        return max(u, l);
    }

    // O(1) area tracking
    long long get_area2() {
        if (upper.st.empty() && lower.st.empty()) return 0;

        long long res = upper.area2 + lower.area2;

        // Connect the endpoints of upper and lower envelopes
        if (!upper.st.empty() && !lower.st.empty()) {
            pt u_right = *upper.st.rbegin();
            pt u_left = *upper.st.begin();
            pt l_right = -(*lower.st.begin());
            pt l_left = -(*lower.st.rbegin());

            res += upper.cross(u_right, l_right);
            res += upper.cross(l_left, u_left);
        }
        return abs(res);
    }

    double get_area() {
        return get_area2() / 2.0;
    }

    // Returns hull vertices in strictly CCW order.
    vector<pt> get_hull() {
        vector<pt> up(upper.st.begin(), upper.st.end());
        if (up.size() <= 1) return up;

        vector<pt> down;
        for (auto p : lower.st) down.push_back(-p);

        vector<pt> hull = up;
        for (int i = 1; i + 1 < (int)down.size(); i++) {
            hull.push_back(down[i]);
        }
        return hull;
    }
};


// ==========================================
// --- TANGENTS FROM EXTERIOR POINT O(log N)
// ==========================================
pair<int, int> tangentsFromExteriorPoint(const vector<pt>& poly, pt q) {
    int n = poly.size();
    if (n < 3) return {0, min(1LL, n - 1)};
    auto is_up = [&](int i) {
        return sgn(cross(poly[i] - q, poly[(i + 1) % n] - q)) >= 0;
    };
    auto cmp_0 = [&](int m) {
        return sgn(cross(poly[0] - q, poly[m] - q));
    };
    auto last_true = [&](auto predicate) {
        int l = 0, r = n - 1, ans = 0;
        while (l <= r) {
            int m = l + (r - l) / 2;
            if (predicate(m)) {
                ans = m;
                l = m + 1;
            } else {
                r = m - 1;
            }
        }
        return ans;
    };

    bool u0 = is_up(0);
    int left_tangent, right_tangent;

    if (u0) {
        left_tangent = (last_true([&](int m) {
            return is_up(m) && cmp_0(m) >= 0;
        }) + 1) % n;

        right_tangent = (last_true([&](int m) {
            return !(is_up(m) && cmp_0(m) < 0);
        }) + 1) % n;
    } else {
        right_tangent = (last_true([&](int m) {
            return !is_up(m) && cmp_0(m) <= 0;
        }) + 1) % n;

        left_tangent = (last_true([&](int m) {
            return !(!is_up(m) && cmp_0(m) > 0);
        }) + 1) % n;
    }
    return {right_tangent, left_tangent};
}
// Returns the intersection points of an infinite directed line AB and a convex polygon.
// The polygon MUST be strictly convex and in Counter-Clockwise (CCW) order.
// Time Complexity: O(log N)
// Returns:
// - Empty vector if there is no intersection.
// - 1 point if the line is tangent to a single vertex.
// - 2 points if the line properly intersects the polygon (or coincides with an edge).
vector<pt> lineConvexPolygonIntersection(const vector<pt>& poly, pt A, pt B) {
    int n = poly.size();
    if (n < 3) return {};
    auto getExtremeVertex = [&](pt dir) {
        auto cmp = [&](int i, int j) {
            return sgn(dot(poly[i], dir) - dot(poly[j], dir));
        };
        auto is_up = [&](int i) {
            return cmp((i + 1) % n, i) >= 0;
        };
        int l = 0, r = n - 1;
        bool u0 = is_up(0);
        while (l <= r) {
            int m = l + (r - l) / 2;
            bool um = is_up(m);
            if (um == u0) {
                if (um) {
                    if (cmp(m, 0) >= 0) l = m + 1;
                    else r = m - 1;
                } else {
                    if (cmp(m, 0) <= 0) l = m + 1;
                    else r = m - 1;
                }
            } else {
                if (um) l = m + 1;
                else r = m - 1;
            }
        }
        return l % n;
    };
    pt N = perp_ccw(B - A);
    int max_idx = getExtremeVertex(N);
    int min_idx = getExtremeVertex(-N);
    auto eval = [&](int i) { return cross(B - A, poly[i] - A); };
    T max_val = eval(max_idx);
    T min_val = eval(min_idx);
    if (sgn(max_val) < 0 || sgn(min_val) > 0) return {};
    auto get_crossing = [&](int start, int end) {
        int L = (end - start + n) % n;
        int l = 0, r = L - 1;
        int ans = 0;
        T v_start = eval(start);
        while (l <= r) {
            int m = l + (r - l) / 2;
            int idx = (start + m) % n;
            T v_mid = eval(idx);
            if (sgn(v_mid) == 0) {
                ans = m;
                break;
            }
            if (sgn(v_mid) == sgn(v_start)) {
                ans = m;
                l = m + 1;
            } else {
                r = m - 1;
            }
        }
        return (start + ans) % n;
    };
    vector<pt> res;
    auto add_pt = [&](pt p) {
        for (pt existing : res) {
            if (same(existing, p)) return;
        }
        res.push_back(p);
    };
    auto intersect_edge = [&](int i) {
        pt C = poly[i], D = poly[(i + 1) % n];
        T vC = eval(i), vD = eval((i + 1) % n);
        if (sgn(vC) == 0) add_pt(C);
        else if (sgn(vD) == 0) add_pt(D);
        else {
            // Find intersection point using proportion of distances
            pt inter = C + (D - C) * (vC / (vC - vD));
            add_pt(inter);
        }
    };
    int cross1 = get_crossing(max_idx, min_idx);
    intersect_edge(cross1);
    int cross2 = get_crossing(min_idx, max_idx);
    intersect_edge(cross2);
    return res;
}


// ==========================================
// --- 10. MINKOWSKI DIFFERENCE & DISTANCES ---
// ==========================================

// Minkowski Difference of two Convex Polygons in O(N + M)
vector<pt> minkowski_diff(vector<pt> P, vector<pt> Q) {
    vector<pt> negQ;
    negQ.reserve(Q.size());
    for (const pt& p : Q) {
        negQ.push_back(-p); // Reflect Q across the origin
    }
    return minkowski(P, negQ);
}

// Minimum and Maximum Distance between two Convex Polygons in O(N + M)
// Returns a pair: {min_distance, max_distance}
pair<T, T> convexPolygonsDistances(vector<pt> P, vector<pt> Q) {
    vector<pt> MD = minkowski_diff(P, Q);
    T max_dist = 0;
    for (const pt& p : MD) {
        max_dist = max(max_dist, abs(p));
    }
    T min_dist = 1e18; // Large value representing infinity
    if (pointInConvexPolygon(MD, pt(0, 0), false)) {
        min_dist = 0;
    } else {
        int n = MD.size();
        for (int i = 0; i < n; i++) {
            pt a = MD[i];
            pt b = MD[(i + 1) % n];
            if (same(a, b)) {
                min_dist = min(min_dist, abs(a));
            } else {
                T t = clamp(dot(-a, b - a) / dist2(a, b), (T)0.0, (T)1.0);
                pt proj = a + (b - a) * t;
                min_dist = min(min_dist, abs(proj));
            }
        }
    }
    return {min_dist, max_dist};
}

// Finds the maximum distance between any two points in a set in O(N log N).
// Returns a pair containing the two farthest points and their Euclidean distance.
pair<pair<pt, pt>, T> pointSetDiameter(vector<pt> pts) {
    int n = pts.size();
    if (n < 2) {
        return {{pt(0, 0), pt(0, 0)}, (T)0};
    }
    if (n == 2) {
        return {{pts[0], pts[1]}, abs(pts[1] - pts[0])};
    }
    convex_hull(pts, false); 
    return convexDiameter(pts);
}
T maxDistanceInPointSet(vector<pt> pts) {
    return pointSetDiameter(pts).second;
}
// Returns: {min_width, max_width}
pair<T, T> convexPolygonWidthExtremes(const vector<pt>& poly) {
    int n = poly.size();
    if (n < 2) return {0, 0};
    if (n == 2) return {0, abs(poly[1] - poly[0])};

    T min_width = 1e18;
    int j = 1;
    
    // Find Minimum Width (Edge to opposite vertex)
    for (int i = 0; i < n; i++) {
        pt v = poly[(i + 1) % n] - poly[i];
        while (sgn(cross(v, poly[(j + 1) % n] - poly[j])) >= 0) {
            j = (j + 1) % n;
        }
        T current_width = cross(v, poly[j] - poly[i]) / abs(v);
        min_width = min(min_width, current_width);
    }
    
    // Maximum width is precisely the diameter of the polygon
    T max_width = convexDiameter(poly).second; 
    
    return {min_width, max_width};
}

pair<T, T> convexPolygonBoundingBoxAreaExtremes(const vector<pt>& poly) {
    int n = poly.size();
    if (n < 3) return {0, 0};
    
    T min_area = 1e18, max_area = 0;
    int top = 1, right = 1, left = 1;
    
    for (int i = 0; i < n; i++) {
        pt v = poly[(i + 1) % n] - poly[i];
        
        while (sgn(cross(v, poly[(top + 1) % n] - poly[top])) >= 0) 
            top = (top + 1) % n;
            
        while (sgn(dot(v, poly[(right + 1) % n] - poly[right])) >= 0) 
            right = (right + 1) % n;
            
        if (i == 0) left = top; 
        while (sgn(dot(-v, poly[(left + 1) % n] - poly[left])) >= 0) 
            left = (left + 1) % n;
            
        T height_scaled = cross(v, poly[top] - poly[i]);
        T width_scaled = dot(v, poly[right] - poly[i]) - dot(v, poly[left] - poly[i]);
        
        T current_area = (height_scaled * width_scaled) / dist2(poly[(i + 1) % n], poly[i]);
        
        min_area = min(min_area, current_area);
        max_area = max(max_area, current_area);
    }
    
    return {min_area, max_area};
}

// 3. MINIMUM & MAXIMUM AREA TRIANGLE
pair<T, T> convexPolygonTriangleAreaExtremes(const vector<pt>& poly) {
    int n = poly.size();
    if (n < 3) return {0, 0};
    // Part 1: Minimum Area Triangle (always 3 adjacent points on a convex hull)
    T min_area = 1e18;
    for (int i = 0; i < n; i++) {
        T cur = cross(poly[(i + 1) % n] - poly[i], poly[(i + 2) % n] - poly[i]);
        min_area = min(min_area, abs(cur));
    }
    // Part 2: Maximum Area Triangle (3-pointer Rotating Calipers)
    T max_area = 0;
    int j = 1, k = 2;
    for (int i = 0; i < n; i++) {
        if (i == j) j = (j + 1) % n;
        if (j == k) k = (k + 1) % n;
        while (true) {
            while (sgn(cross(poly[j] - poly[i], poly[(k + 1) % n] - poly[i]) - 
                       cross(poly[j] - poly[i], poly[k] - poly[i])) >= 0) {
                k = (k + 1) % n;
            }
            if (sgn(cross(poly[(j + 1) % n] - poly[i], poly[k] - poly[i]) - 
                    cross(poly[j] - poly[i], poly[k] - poly[i])) >= 0) {
                j = (j + 1) % n;
            } else {
                break;
            }
        }
        max_area = max(max_area, abs(cross(poly[j] - poly[i], poly[k] - poly[i])));
    }
    
    return {min_area / 2.0L, max_area / 2.0L};
}
