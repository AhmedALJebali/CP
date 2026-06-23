const ld EPS = 1e-9;
const ld PI = acos(-1.0L);

typedef ld T;
typedef complex<T> pt;
#define x real()
#define y imag()

// =====================
// Basic vector helpers
// =====================

T dot(pt a, pt b) { return a.x * b.x + a.y * b.y; }
T cross(pt a, pt b) { return a.x * b.y - a.y * b.x; }
T sq(pt p) { return p.x * p.x + p.y * p.y; }
pt perp_ccw(pt p) { return {-p.y, p.x}; }

int sgn(T val) {
    if (val > EPS) return 1;
    if (val < -EPS) return -1;
    return 0;
}

bool samePoint(pt a, pt b) {
    return abs(a - b) <= EPS;
}

bool onSegment(pt a, pt b, pt p) {
    return sgn(cross(b - a, p - a)) == 0 && sgn(dot(a - p, b - p)) <= 0;
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
// Convex hull (Monotonic chain)
// Returns CCW hull without repeated last point
// =====================

vector<pt> convexHull(vector<pt> p, bool strict = false) {
    sort(p.begin(), p.end(), [](const pt& a, const pt& b) {
        if (sgn(a.x - b.x) != 0) return a.x < b.x;
        return a.y < b.y;
    });

    p.erase(unique(p.begin(), p.end(), [](const pt& a, const pt& b) {
        return samePoint(a, b);
    }), p.end());

    int n = (int)p.size();
    if (n <= 1) return p;

    vector<pt> lo, up;
    for (auto &pti : p) {
        while ((int)lo.size() >= 2) {
            T cr = cross(lo.back() - lo[(int)lo.size() - 2], pti - lo.back());
            if (strict ? sgn(cr) <= 0 : sgn(cr) < 0) lo.pop_back();
            else break;
        }
        lo.push_back(pti);
    }

    for (int i = n - 1; i >= 0; i--) {
        auto pti = p[i];
        while ((int)up.size() >= 2) {
            T cr = cross(up.back() - up[(int)up.size() - 2], pti - up.back());
            if (strict ? sgn(cr) <= 0 : sgn(cr) < 0) up.pop_back();
            else break;
        }
        up.push_back(pti);
    }

    lo.pop_back();
    up.pop_back();
    vector<pt> hull = lo;
    hull.insert(hull.end(), up.begin(), up.end());

    if (hull.empty() && !p.empty()) hull = p; // all points identical
    return hull;
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
    if (n == 1) return !strict && samePoint(poly[0], q);
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
