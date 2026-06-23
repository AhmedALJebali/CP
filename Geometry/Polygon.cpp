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
