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
