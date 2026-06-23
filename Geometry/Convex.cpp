const ld EPS = 1e-9;
const ld PI = acos(-1.0L);

typedef ld T;
typedef complex<T> pt;
#define x real()
#define y imag()

// =====================
// Basic operations
// =====================
T dot(pt v, pt w) { return v.x * w.x + v.y * w.y; } // >0 acute, 0 right, <0 obtuse
T cross(pt v, pt w) { return v.x * w.y - v.y * w.x; } // >0 CCW, <0 CW. Mag = Area
int sgn(T val) { if(val > EPS) return 1; if(val < -EPS) return -1; return 0; } // Float safe

// --- 3. ANGLES ---
T toDegrees(T rad) { return rad * (180.0 / PI); }
T toRadians(T deg) { return deg * (PI / 180.0); }
T orient(pt a, pt b, pt c) { return cross(b - a, c - a); } // Orientation of C relative to AB
T angle(pt v, pt w) { return acos(clamp(dot(v, w) / abs(v) / abs(w), (T)-1.0, (T)1.0)); }

// Returns angle [0, 2PI) moving strictly Counter-Clockwise from AB to AC
T orientedAngle(pt a, pt b, pt c) {
    T ampli = angle(b - a, c - a);
    // When collinear and same direction ampli=0, orient<=0 would give 2PI. Fix: return 0.
    if (sgn(ampli) == 0) return 0;
    return sgn(orient(a, b, c)) > 0 ? ampli : 2 * PI - ampli;
}

// Returns shortest rotational path (-PI, PI]. + is CCW (Left), - is CW (Right)
T angleTravelled(pt a, pt b, pt c) {
    T ampli = angle(b - a, c - a);
    if (sgn(orient(a, b, c)) > 0) return ampli;
    if (sgn(orient(a, b, c)) < 0) return -ampli;
    // Collinear case: same direction => 0, opposite directions => PI
    return (sgn(dot(b - a, c - a)) >= 0) ? (T)0 : PI;
}

bool same(pt a, pt b) {
    return abs(a - b) <= EPS;
}

T dist2(pt a, pt b) {
    return norm(a - b);
}

// =====================
// Collinearity / segment
// =====================

bool collinear(pt a, pt b, pt c) {
    return sgn(orient(a,b,c)) == 0;
}

bool onSegment(pt a, pt b, pt p) {
    return sgn(orient(a,b,p)) == 0 &&
           sgn(dot(p-a, p-b)) <= 0;
}
// =====================
// Convex polygon basics
// =====================

T signedAreaPolygon(const vector<pt>& p) {
    int n = (int)p.size();
    T area = 0;
    for (int i = 0; i < n; i++) {
        area += cross(p[i], p[(i + 1) % n]);
    }
    return area / 2.0L;
}

T areaPolygon(const vector<pt>& p) {
    return fabsl(signedAreaPolygon(p));
}

T perimeterPolygon(const vector<pt>& p) {
    int n = (int)p.size();
    if (n < 2) return 0;
    T per = 0;
    for (int i = 0; i < n; i++) {
        per += abs(p[(i + 1) % n] - p[i]);
    }
    return per;
}

bool isConvex(const vector<pt>& p, bool strict = false) {
    int n = (int)p.size();
    if (n < 3) return false;

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

// Ensure polygon is CCW
void normalizeCCW(vector<pt>& p) {
    if (sgn(signedAreaPolygon(p)) < 0) reverse(p.begin(), p.end());
}

// Rotate so lowest (then leftmost) point becomes p[0]
void reorderConvex(vector<pt>& p) {
    int pos = 0;
    for (int i = 1; i < (int)p.size(); i++) {
        if (sgn(p[i].y - p[pos].y) < 0 ||
            (sgn(p[i].y - p[pos].y) == 0 && sgn(p[i].x - p[pos].x) < 0)) {
            pos = i;
            }
    }
    rotate(p.begin(), p.begin() + pos, p.end());
}

// =====================
// Convex hull (Andrew style)
// =====================

bool cw(pt a, pt b, pt c, bool col) {
    int o = sgn(orient(a,b,c));
    return o < 0 || (col && o == 0);
}

void convex_hull(vector<pt>& a, bool include_collinear = false) {
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


// =====================
// reorder polygon
// =====================
void reorder_polygon(vector<pt>& P) {
    int pos = 0;
    for (int i = 1; i < (int)P.size(); i++) {
        if (imag(P[i]) < imag(P[pos]) ||
           (imag(P[i]) == imag(P[pos]) && real(P[i]) < real(P[pos])))
            pos = i;
    }
    rotate(P.begin(), P.begin() + pos, P.end());
}

// =====================
// Minkowski sum (FIXED)
// =====================

vector<pt> minkowski(vector<pt> P, vector<pt> Q) {
    reorder_polygon(P);
    reorder_polygon(Q);

    int n = P.size(), m = Q.size();
    P.push_back(P[0]);
    Q.push_back(Q[0]);

    vector<pt> res;
    int i = 0, j = 0;

    while (i < n || j < m) {
        res.push_back(P[i] + Q[j]);

        T cr = cross(P[i+1] - P[i], Q[j+1] - Q[j]);

        if (j == m || (i < n && cr >= 0)) i++;
        if (i == n || (j < m && cr <= 0)) j++;
    }

    return res;
}

// =====================
// Point in convex polygon (O(log n))
// poly must be CCW and without repeated last point
// strict = true  -> boundary is outside
// strict = false -> boundary is inside
// =====================

bool pointInConvexPolygon(vector<pt> poly, pt q, bool strict = true) {
    int n = (int)poly.size();
    if (n == 0) return false;
    if (n == 1) return !strict && !sgn(abs(poly[0]- q));
    if (n == 2) return onSegment(poly[0], poly[1], q) && !strict;

    if (sgn(cross(poly[1] - poly[0], q - poly[0])) < 0) return false;
    if (sgn(cross(poly[n - 1] - poly[0], q - poly[0])) > 0) return false;

    if (onSegment(poly[0], poly[1], q) || onSegment(poly[0], poly[n - 1], q))
        return !strict;
    int l = 1, r = n - 1;
    while (r - l > 1) {
        int m = (l + r) / 2;
        if (sgn(cross(poly[m] - poly[0], q - poly[0])) >= 0) l = m;
        else r = m;
    }

    T cr = cross(poly[l + 1] - poly[l], q - poly[l]);
    if (sgn(cr) < 0) return false;
    if (sgn(cr) == 0) return !strict;
    return true;
}
// =====================
// Diameter of convex polygon (Rotating calipers)
// Returns farthest pair and their distance
// poly must be CCW and convex
// =====================

pair<pair<pt, pt>, T> convexDiameter(const vector<pt>& poly) {
    int n = (int)poly.size();
    if (n == 0) return {{pt(0, 0), pt(0, 0)}, 0};
    if (n == 1) return {{poly[0], poly[0]}, 0};
    if (n == 2) return {{poly[0], poly[1]}, abs(poly[1] - poly[0])};

    int j = 1;
    T best = 0;
    pair<pt, pt> ans = {poly[0], poly[0]};

    for (int i = 0; i < n; i++) {
        int ni = (i + 1) % n;
        while (abs(cross(poly[ni] - poly[i], poly[(j + 1) % n] - poly[j])) >
               abs(cross(poly[ni] - poly[i], poly[j] - poly[i]))) {
            j = (j + 1) % n;
               }

        T d1 = abs(poly[i] - poly[j]);
        if (d1 > best) best = d1, ans = {poly[i], poly[j]};

        T d2 = abs(poly[ni] - poly[j]);
        if (d2 > best) best = d2, ans = {poly[ni], poly[j]};
    }

    return {ans, best};
}

// =====================
// Halfplane
// =====================

struct Halfplane {
    pt p, pq;
    T angle;

    Halfplane() {}
    Halfplane(pt a, pt b) : p(a), pq(b-a) {
        angle = atan2(imag(pq), real(pq));
    }

    bool out(pt r) {
        return cross(pq, r - p) < -EPS;
    }

    bool operator<(const Halfplane& other) const {
        return angle < other.angle;
    }

    friend pt inter(const Halfplane& a, const Halfplane& b) {
        T A = cross(b.p - a.p, b.pq) / cross(a.pq, b.pq);
        return a.p + a.pq * A;
    }
};

// =====================
// Half-plane intersection
// =====================

vector<pt> hp_intersect(vector<Halfplane> H) {
    const T INF = 1e9;

    vector<pt> box = {
        {INF, INF}, {-INF, INF},
        {-INF, -INF}, {INF, -INF}
    };

    for (int i = 0; i < 4; i++)
        H.emplace_back(box[i], box[(i+1)%4]);

    sort(H.begin(), H.end());

    deque<Halfplane> dq;

    for (auto &h : H) {
        while (dq.size() > 1 && h.out(inter(dq[dq.size()-1], dq[dq.size()-2])))
            dq.pop_back();

        while (dq.size() > 1 && h.out(inter(dq[0], dq[1])))
            dq.pop_front();

        dq.push_back(h);
    }

    while (dq.size() > 2 &&
           dq.front().out(inter(dq[dq.size()-1], dq[dq.size()-2])))
        dq.pop_back();

    while (dq.size() > 2 &&
           dq.back().out(inter(dq[0], dq[1])))
        dq.pop_front();

    if (dq.size() < 3) return {};

    vector<pt> ret;
    for (int i = 0; i < (int)dq.size(); i++)
        ret.push_back(inter(dq[i], dq[(i+1)%dq.size()]));

    return ret;
}

// =====================
// antipodal pairs
// =====================

vector<pair<int,int>> all_anti_podal(int n, vector<pt>& p) {
    vector<pair<int,int>> res;
    vector<bool> vis(n,false);

    auto nx = [&](int i){ return (i+1)%n; };
    auto pv = [&](int i){ return (i-1+n)%n; };

    for (int i = 0, j = 0; i < n; i++) {
        pt base = p[nx(i)] - p[i];

        while (j == i || j == nx(i) ||
              sgn(cross(base, p[nx(j)] - p[j])) ==
              sgn(cross(base, p[j] - p[pv(j)]))) {
            j = nx(j);
        }

        if (vis[i]) continue;
        vis[i] = true;

        res.push_back({i,j});
        res.push_back({nx(i),j});
    }

    return res;
}

// =====================
// max distance polygon-polygon
// =====================

T maximum_dist_from_polygon_to_polygon(vector<pt>& u, vector<pt>& v) {
    int n = u.size(), m = v.size();
    T ans = 0;

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            ans = max(ans, dist2(u[i], v[j]));

    return sqrt(ans);
}
