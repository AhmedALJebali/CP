const ld EPS = 1e-9;
const ld PI  = acos(-1.0L);

typedef ld T;
typedef complex<T> pt;
#define x real()
#define y imag()

// =====================
// Basic vector helpers
// =====================

T sq(pt p) { return p.x * p.x + p.y * p.y; }
T dot(pt a, pt b) { return a.x * b.x + a.y * b.y; }
T cross(pt a, pt b) { return a.x * b.y - a.y * b.x; }
pt perp_ccw(pt p) { return {-p.y, p.x}; }

int sgn(T val) {
    if (val > EPS) return 1;
    if (val < -EPS) return -1;
    return 0;
}

bool samePoint(pt a, pt b) {
    return abs(a - b) <= EPS;
}

// =====================
// Minimal line helpers
// =====================

struct line {
    pt v;
    T c; // cross(v, p) = c

    line(pt p, pt q) : v(q - p), c(cross(v, p)) {}

    T side(pt p) const { return cross(v, p) - c; }
};

bool inter(line l1, line l2, pt &out) {
    T d = cross(l1.v, l2.v);
    if (sgn(d) == 0) return false;
    out = (l2.v * l1.c - l1.v * l2.c) / d;
    return true;
}

// =====================
// Polygon area / perimeter
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

// =====================
// Polygon orientation / convexity
// =====================

bool isCCW(const vector<pt>& p) {
    return sgn(signedAreaPolygon(p)) > 0;
}

bool isConvex(const vector<pt>& p) {
    int n = (int)p.size();
    if (n < 3) return false;

    int dir = 0;
    for (int i = 0; i < n; i++) {
        T cr = cross(p[(i + 1) % n] - p[i], p[(i + 2) % n] - p[(i + 1) % n]);
        int s = sgn(cr);
        if (s == 0) continue;
        if (dir == 0) dir = s;
        else if (dir != s) return false;
    }
    return true;
}

// =====================
// Point on segment / polygon
// =====================

bool inDisk(pt a, pt b, pt p) {
    return sgn(dot(a - p, b - p)) <= 0;
}

bool onSegment(pt a, pt b, pt p) {
    return sgn(cross(b - a, p - a)) == 0 && inDisk(a, b, p);
}

bool above(pt a, pt p) {
    return p.y >= a.y;
}

bool crossesRay(pt a, pt p, pt q) {
    return (above(a, q) - above(a, p)) * sgn(cross(p - a, q - a)) > 0;
}

// strict = true  -> boundary is outside
// strict = false -> boundary is inside
bool inPolygon(const vector<pt>& p, pt a, bool strict = true) {
    int n = (int)p.size();
    int cnt = 0;
    for (int i = 0; i < n; i++) {
        pt u = p[i], v = p[(i + 1) % n];
        if (onSegment(u, v, a)) return !strict;
        cnt += crossesRay(a, u, v);
    }
    return cnt & 1;
}

// Winding number: 0 outside, non-zero inside, -1 on boundary
int windingNumber(const vector<pt>& p, pt a) {
    int wn = 0;
    int n = (int)p.size();

    for (int i = 0; i < n; i++) {
        pt u = p[i], v = p[(i + 1) % n];

        if (onSegment(u, v, a)) return -1;

        if (sgn(u.y - a.y) <= 0) {
            if (sgn(v.y - a.y) > 0 && sgn(cross(v - u, a - u)) > 0) wn++;
        } else {
            if (sgn(v.y - a.y) <= 0 && sgn(cross(v - u, a - u)) < 0) wn--;
        }
    }
    return wn;
}

// =====================
// Polygon centroid
// =====================

pt polygonCentroid(const vector<pt>& p) {
    int n = (int)p.size();
    pt c(0, 0);
    T A2 = 0; // 2 * signed area

    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        T cr = cross(p[i], p[j]);
        c += (p[i] + p[j]) * cr;
        A2 += cr;
    }

    if (sgn(A2) == 0) return {numeric_limits<T>::quiet_NaN(),
                              numeric_limits<T>::quiet_NaN()};

    return c / (3.0L * A2);
}
// Returns the number of boundary lattice points on the segment AB.
// Does NOT count point A itself to avoid double-counting when iterating over a polygon.
int segmentBoundaryPoints(pt a, pt b) {
    // llround safely converts ld to exact integers, preventing truncation errors
    int dx = abs(llround(a.x) - llround(b.x));
    int dy = abs(llround(a.y) - llround(b.y));
    return gcd(dx, dy);
}
// Returns the total number of boundary lattice points (B) of a polygon
int polygonBoundaryPoints(const vector<pt>& p) {
    int B = 0;
    int n = p.size();
    for (int i = 0; i < n; i++) {
        B += segmentBoundaryPoints(p[i], p[(i + 1) % n]);
    }
    return B;
}
// Calculates EXACT double area, bypassing the float division in areaPolygon()
int polygonDoubleAreaExact(const vector<pt>& p) {
    int doubleA = 0;
    int n = p.size();
    for (int i = 0; i < n; i++) {
        // Rounding coordinates guarantees exact 64-bit integer cross products
        int x1 = llround(p[i].x), y1 = llround(p[i].y);
        int x2 = llround(p[(i + 1) % n].x), y2 = llround(p[(i + 1) % n].y);
        doubleA += (x1 * y2) - (x2 * y1);
    }
    return abs(doubleA);
}
// Returns the number of strictly interior lattice points (I) using Pick's Theorem.
// Formula: A = I + B/2 - 1  ==>  2A = 2I + B - 2  ==>  I = (2A - B + 2) / 2
int polygonInteriorPoints(const vector<pt>& p) {
    int doubleA = polygonDoubleAreaExact(p);
    int B = polygonBoundaryPoints(p);
    return (doubleA - B + 2) / 2;
}

// =====================
// Polygon cut by directed line AB
// Keeps left side (including the line)
// =====================
vector<pt> polygonCut(const vector<pt>& p, pt a, pt b) {
    vector<pt> q;
    int n = (int)p.size();
    line cut(a, b);

    for (int i = 0; i < n; i++) {
        pt cur = p[i], nxt = p[(i + 1) % n];
        T s1 = cut.side(cur), s2 = cut.side(nxt);

        if (sgn(s1) >= 0) q.push_back(cur);

        if (sgn(s1) * sgn(s2) < 0) {
            pt interPt;
            inter(line(cur, nxt), cut, interPt);
            q.push_back(interPt);
        }
    }
    return q;
}
pt centroid(const vector<pt> &p) {
    int n = (int)p.size();
    pt c(0, 0);
    T sum = 0;
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        T cr = cross(p[i], p[j]);
        sum += cr;
        c += (p[i] + p[j]) * cr;
    }
    if (abs(sum) < EPS) return {0, 0}; 
    return c / (3.0L * sum);
}
// ==========================================
// --- ROTATING CALIPERS ---
// ==========================================

// 1. Maximum distance from any point on the perimeter to another point (Diameter)
T polygonDiameter(const vector<pt> &p) {
    int n = (int)p.size();
    if (n <= 1) return 0.0L;
    if (n == 2) return abs(p[0] - p[1]);

    T ans = 0;
    int i = 0, j = 1;
    while (i < n) {
        // Advance j while the cross product of edges is >= 0
        while (sgn(cross(p[(i + 1) % n] - p[i], p[(j + 1) % n] - p[j])) >= 0) {
            ans = max(ans, sq(p[i] - p[j]));
            j = (j + 1) % n;
        }
        ans = max(ans, sq(p[i] - p[j]));
        i++;
    }
    return sqrt(max((T)0.0, ans));
}

// 2. Minimum distance between two parallel lines enclosing the polygon (Width)
T polygonWidth(const vector<pt> &p) {
    int n = (int)p.size();
    if (n <= 2) return 0.0L;

    T ans = numeric_limits<T>::infinity();
    int i = 0, j = 1;
    while (i < n) {
        while (sgn(cross(p[(i + 1) % n] - p[i], p[(j + 1) % n] - p[j])) >= 0) {
            j = (j + 1) % n;
        }
        // Distance from point p[j] to line(p[i], p[i+1])
        line l(p[i], p[(i + 1) % n]);
        T dist = abs(l.side(p[j])) / abs(l.v);
        ans = min(ans, dist);
        i++;
    }
    return ans;
}

// 3. Minimum enclosing rectangle (Returns Perimeter)
T minimumEnclosingRectanglePerimeter(const vector<pt> &p) {
    int n = (int)p.size();
    if (n <= 2) return perimeterPolygon(p); // Reuses your existing function

    int mndot = 0;
    pt initial_edge = p[1] - p[0];
    T tmp = dot(initial_edge, p[0]);

    for (int i = 1; i < n; i++) {
        if (sgn(dot(initial_edge, p[i]) - tmp) <= 0) {
            tmp = dot(initial_edge, p[i]);
            mndot = i;
        }
    }

    T ans = numeric_limits<T>::infinity();
    int i = 0, j = 1, mxdot = 1;

    while (i < n) {
        pt cur = p[(i + 1) % n] - p[i];
        T cur_len = abs(cur);

        while (sgn(cross(cur, p[(j + 1) % n] - p[j])) >= 0) {
            j = (j + 1) % n;
        }
        while (sgn(dot(p[(mxdot + 1) % n], cur) - dot(p[mxdot], cur)) >= 0) {
            mxdot = (mxdot + 1) % n;
        }
        while (sgn(dot(p[(mndot + 1) % n], cur) - dot(p[mndot], cur)) <= 0) {
            mndot = (mndot + 1) % n;
        }

        // Height: distance from top caliper point to the base line
        line l(p[i], p[(i + 1) % n]);
        T height = abs(l.side(p[j])) / cur_len;

        // Width: projection difference between right and left caliper points
        T width = (dot(p[mxdot], cur) - dot(p[mndot], cur)) / cur_len;

        ans = min(ans, 2.0L * (width + height));
        i++;
    }
    return ans;
}
// ==========================================
// --- 4. RADIAL SORTING ---
// ==========================================
void sortClockwise(vector<pt>& pts, pt center) {
    auto get_half = [&](pt p) {
        if (sgn(p.y - center.y) < 0 || (sgn(p.y - center.y) == 0 && sgn(p.x - center.x) > 0)) return 0;
        return 1;
    };
    sort(pts.begin(), pts.end(), [&](pt a, pt b) {
        int half_a = get_half(a), half_b = get_half(b);
        if (half_a != half_b) return half_a < half_b;
        T cr = cross(a - center, b - center);
        if (sgn(cr) != 0) return sgn(cr) < 0; 
        return sgn(sq(a - center) - sq(b - center)) < 0;
    });
}

void sortCounterClockwise(vector<pt>& pts, pt center) {
    auto get_half = [&](pt p) {
        if (sgn(p.y - center.y) > 0 || (sgn(p.y - center.y) == 0 && sgn(p.x - center.x) > 0)) return 0;
        return 1;
    };
    sort(pts.begin(), pts.end(), [&](pt a, pt b) {
        int half_a = get_half(a), half_b = get_half(b);
        if (half_a != half_b) return half_a < half_b;
        T cr = cross(a - center, b - center);
        if (sgn(cr) != 0) return sgn(cr) > 0; 
        return sgn(sq(a - center) - sq(b - center)) < 0;
    });
}
// Calculates the total area covered by a set of rectangles.
// rects: a vector of rectangles, where each rectangle is {x1, y1, x2, y2}
// (x1, y1) is the bottom-left corner and (x2, y2) is the top-right corner.
T areaUnionOfRectangles(const vector<array<T, 4>>& rects) {
    if (rects.empty()) return 0.0;
    struct Event {
        T x1, y1, y2;
        int type;
        bool operator<(const Event& other) const {
            if (abs(x1 - other.x1) > EPS)
                return x1 < other.x1;
            // Process insertions (+1) before removals (-1) at the same x-coordinate
            return type > other.type;
        }
    };
    vector<Event> events;
    vector<T> Y;
    // 1. Setup events and y-coordinates
    for (const auto& r : rects) {
        events.push_back({r[0], r[1], r[3], 1});
        events.push_back({r[2], r[1], r[3], -1});
        Y.push_back(r[1]);
        Y.push_back(r[3]);
    }
    sort(all(events));
    sort(all(Y));
    // Coordinate compression with EPS tolerance for floating point deduplication
    Y.erase(unique(all(Y), [](T a, T b) {
        return abs(a - b) <= EPS;
    }), Y.end());
    int m = Y.size();
    if (m < 2) return 0.0;
    // Segment tree arrays
    vector<int> cnt(4 * m, 0);   // Count remains integer
    vector<T> len(4 * m, 0.0);   // Segment lengths are now floating-point (T)
    // 2. Segment tree update function
    auto update = [&](auto& self, int node, int l, int r, int ql, int qr, int val) -> void {
        if (ql <= l && r <= qr) {
            cnt[node] += val;
        } else {
            int mid = l + (r - l) / 2;
            if (ql < mid) self(self, 2 * node, l, mid, ql, qr, val);
            if (qr > mid) self(self, 2 * node + 1, mid, r, ql, qr, val);
        }
        // Calculate covered length dynamically
        if (cnt[node] > 0) {
            len[node] = Y[r] - Y[l];
        } else {
            if (r - l == 1) len[node] = 0.0; // leaf node
            else len[node] = len[2 * node] + len[2 * node + 1];
        }
    };
    // 3. Sweep line process
    T total_area = 0.0;
    T last_x = events[0].x1;
    for (const auto& ev : events) {
        total_area += (ev.x1 - last_x) * len[1];
        last_x = ev.x1;
        // Find compressed coordinates using EPS to avoid precision misses
        auto get_idx = [&](T val) {
            auto it = lower_bound(all(Y), val - EPS);
            return (int)distance(Y.begin(), it);
        };
        int y1_idx = get_idx(ev.y1);
        int y2_idx = get_idx(ev.y2);
 
        if (y1_idx < y2_idx) {
            update(update, 1, 0, m - 1, y1_idx, y2_idx, ev.type);
        }
    }
    return total_area;
}

