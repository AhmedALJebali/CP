const ld EPS = 1e-9;
const ld PI = acos(-1.0L);

typedef ld T;
typedef complex<T> pt;
#define x real()
#define y imag()

// ==========================================
// --- 1. BASIC VECTOR OPERATIONS ---
// ==========================================

T dot(pt v, pt w) { return v.x * w.x + v.y * w.y; }
T cross(pt v, pt w) { return v.x * w.y - v.y * w.x; }
pt perp_ccw(pt p) { return {-p.y, p.x}; }
T sq(pt p) { return p.x * p.x + p.y * p.y; }

int sgn(T val) {
    if (val > EPS) return 1;
    if (val < -EPS) return -1;
    return 0;
}

// ==========================================
// --- 2. LINE STRUCTURE & UTILITIES ---
// ==========================================

struct line {

    pt v; // Direction vector of the line
    T c;  // The scalar offset (c = cross(v, p) for any point p on the line)

    // The equation cross(v, p) = c defines all points p on the line.
    // Geometrically: cross(v, p) calculates the signed area of the parallelogram 
    // formed by the direction vector v and point p. This area is constant 
    // for all points p on the same line.

    line(pt v, T c) : v(v), c(c) {}
    
    // Constructing from general form ax + by = c:
    // A vector parallel to the line (ax + by = c) is (b, -a).
    // The constant c remains the same.
    line(T a, T b, T _c) : v(b, -a), c(_c) {}
    
    // Constructing from two points p and q:
    // Direction vector v is simply the difference (q - p).
    line(pt p, pt q) : v(q - p), c(cross(v, p)) {}

    T side(pt p) const { return cross(v, p) - c; }

    T dist(pt p) const { return abs(side(p)) / abs(v); }
    T sqDist(pt p) const { return side(p) * side(p) / sq(v); }

    line perpThrough(pt p) const { return {p, p + perp_ccw(v)}; }

    bool cmpProj(pt p, pt q) const { return dot(v, p) < dot(v, q); }

    line translate(pt t) const { return {v, c + cross(v, t)}; }
    line shiftLeft(T dist) const { return {v, c + dist * abs(v)}; }

    pt proj(pt p) const { return p - perp_ccw(v) * side(p) / sq(v); }
    pt refl(pt p) const { return p - perp_ccw(v) * (T)2.0L * side(p) / sq(v); }
};
struct LineKey {
    T a, b, c;
    LineKey(T a, T b, T c) : a(a), b(b), c(c) {
        normalize();
    }
    void normalize() {
        ld z = sqrt(a*a + b*b);
        a /= z; b /= z; c /= z;
        if (a < 0 || (a == 0 && b < 0)) {
            a = -a; b = -b; c = -c;
        }
    }
    bool operator<(const LineKey& o) const {
        int cmp = sgn(a - o.a);
        if (cmp != 0) return cmp == -1;
        cmp = sgn(b - o.b);
        if (cmp != 0) return cmp == -1;
        return sgn(c - o.c) == -1;
    }
};
// ==========================================
// --- 3. INTERSECTIONS & SPECIAL POINTS ---
// ==========================================

bool inter(line l1, line l2, pt &out) {
    T d = cross(l1.v, l2.v);
    if (sgn(d) == 0) return false;
    out = (l2.v * l1.c - l1.v * l2.c) / d;
    return true;
}

line bisector(line l1, line l2, bool interior) {
    assert(sgn(cross(l1.v, l2.v)) != 0);
    T s = interior ? 1.0L : -1.0L;
    return {
        l2.v / abs(l2.v) + l1.v / abs(l1.v) * s,
        l2.c / abs(l2.v) + l1.c / abs(l1.v) * s
    };
}

pt shortestPathPointOnLine(pt a, pt b, line l) {
    T sa = l.side(a), sb = l.side(b);

    if (sgn(sa) == 0 && sgn(sb) == 0) return a;
    if (sgn(sa) == 0) return a;
    if (sgn(sb) == 0) return b;

    if (sgn(sa) * sgn(sb) < 0) {
        pt out;
        inter(line(a, b), l, out);
        return out;
    }

    pt out;
    inter(line(l.refl(a), b), l, out);
    return out;
}

// ==========================================
// --- 4. RAYS ---
// ==========================================

bool rayLineInter(pt a, pt b, line l) {
    line rayAsLine(a, b);
    pt out;
    if (inter(rayAsLine, l, out)) {
        return sgn(dot(out - a, b - a)) >= 0;
    }
    // Parallel case: ray may lie on the line
    return sgn(l.side(a)) == 0;
}

T rayLine(pt a, pt b, line l) {
    if (rayLineInter(a, b, l)) return 0.0L;
    return l.dist(a);
}

