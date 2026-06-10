const ld EPS = 1e-9;
const ld PI = acos(-1.0L);

typedef ld T;
typedef complex<T> pt;
#define x real()
#define y imag()

// --- 1. BASIC MATH & VECTORS ---
T sq(pt p) { return p.x * p.x + p.y * p.y; } // Squared magnitude |v|^2
T dot(pt v, pt w) { return v.x * w.x + v.y * w.y; } // >0 acute, 0 right, <0 obtuse
T cross(pt v, pt w) { return v.x * w.y - v.y * w.x; } // >0 CCW, <0 CW. Mag = Area
pt vect(T r, T th) { return {r * cos(th), r * sin(th)}; } // Polar to Cartesian
int sgn(T val) { if(val > EPS) return 1; if(val < -EPS) return -1; return 0; } // Float safe
bool isPerp(pt v, pt w) { return fabs(dot(v, w)) < EPS; } // Perpendicular check
pt gt() { T xx, yy; cin >> xx >> yy; return pt(xx, yy); } // Fast read

// --- 2. TRANSFORMATIONS ---
pt perp_ccw(pt p) { return {-p.y, p.x}; } // Rotate 90 deg counter-clockwise
pt perp_cw(pt p) { return {p.y, -p.x}; }  // Rotate 90 deg clockwise
pt translate(pt v, pt p) { return p + v; }
pt scale(pt c, ld factor, pt p) { return c + (p - c) * factor; }
pt rot(pt p, pt c, ld a) { return c + (p - c) * pt(cos(a), sin(a)); } // Complex mult is 2D rot
// Maps point r from coordinate system (p, q) to (fp, fq) using complex division
pt linearTransfo(pt p, pt q, pt r, pt fp, pt fq) { return fp + (r - p) * (fq - fp) / (q - p); }

// --- 3. ANGLES ---
T toDegrees(T rad) { return rad * (180.0 / PI); }
T toRadians(T deg) { return deg * (PI / 180.0); }
T orient(pt a, pt b, pt c) { return cross(b - a, c - a); } // Orientation of C relative to AB
T angle(pt v, pt w) { return acos(clamp(dot(v, w) / abs(v) / abs(w), (T)-1.0, (T)1.0)); }

// Returns angle [0, 2PI) moving strictly Counter-Clockwise from AB to AC
T orientedAngle(pt a, pt b, pt c) {
    ld ampli = angle(b - a, c - a);
    // When collinear and same direction ampli=0, orient<=0 would give 2PI. Fix: return 0.
    if (sgn(ampli) == 0) return 0;
    return sgn(orient(a, b, c)) > 0 ? ampli : 2 * PI - ampli;
}

// Returns shortest rotational path (-PI, PI]. + is CCW (Left), - is CW (Right)
T angleTravelled(pt a, pt b, pt c) {
    ld ampli = angle(b - a, c - a);
    if (sgn(orient(a, b, c)) > 0) return ampli;
    if (sgn(orient(a, b, c)) < 0) return -ampli;
    // Collinear case: same direction => 0, opposite directions => PI
    return (sgn(dot(b - a, c - a)) >= 0) ? (T)0 : PI;
}

// Checks if ray AP is strictly between rays AB and AC
bool inAngle(pt a, pt b, pt c, pt p) {
    // Normalize: ensure ABC is CCW
    if (sgn(orient(a, b, c)) < 0) swap(b, c);
    // P is inside angle ABC iff it is CCW from AB and CW from AC
    return sgn(orient(a, b, p)) > 0 && sgn(orient(a, c, p)) < 0;
}

// --- 4. LINES ---
struct line {
    pt v; T c;
    line(pt v, T c) : v(v), c(c) {} // From direction vector v and offset c
    line(T a, T b, T _c) : v(b, -a), c(_c) {} // From equation ax + by = c
    line(pt p, pt q) : v(q - p), c(cross(v, p)) {} // From two points P and Q

    T side(pt p) { return cross(v, p) - c; } // >0 left, =0 on line, <0 right
    T dist(pt p) { return abs(side(p)) / abs(v); } // Perpendicular distance
    T sqDist(pt p) { return side(p) * side(p) / (T)sq(v); }
    line prepThrough(pt p) { return {p, p + perp_ccw(v)}; }
    bool cmpProj(pt p, pt q) { return dot(v, p) < dot(v, q); }
    line translate(pt t) { return {v, c + cross(v, t)}; }
    line shiftLeft(T dist) { return {v, c + dist * abs(v)}; }
    pt proj(pt p) { return p - perp_ccw(v) * side(p) / sq(v); } // Drop perpendicular
    pt refl(pt p) { return p - perp_ccw(v) * (T)2.0 * side(p) / sq(v); } // Reflect point
};

// Intersection of 2 lines using Cramer's rule. Returns false if parallel.
bool inter(line l1, line l2, pt &out) {
    T d = cross(l1.v, l2.v);
    if (sgn(d) == 0) return false;
    out = (l2.v * l1.c - l1.v * l2.c) / d;
    return true;
}

// Bisector of angle. true = interior angle, false = exterior angle
line bisector(line l1, line l2, bool interior) {
    assert(sgn(cross(l1.v, l2.v)) != 0); // Lines must not be parallel
    ld sign = interior ? 1 : -1;
    return {l2.v / abs(l2.v) + l1.v / abs(l1.v) * sign, l2.c / abs(l2.v) + l1.c / abs(l1.v) * sign};
}

// Heron's shortest path: point on line l minimizing distance to A + distance to B
pt shortestPathPointOnLine(pt a, pt b, line l) {
    if (sgn(l.side(a)) == 0 && sgn(l.side(b)) == 0) return a; // Both on line: any point works, return A
    if (sgn(l.side(a)) * sgn(l.side(b)) < 0) { // Opposite sides, direct connect
        pt out; inter(line(a, b), l, out); return out;
    }
    pt out; inter(line(l.refl(a), b), l, out); return out; // Same side, reflect and connect
}

// --- 5. SEGMENTS & RAYS ---
// Thales's Theorem: angle at p is obtuse/right if dot <= 0
bool inDisk(pt a, pt b, pt p) { return sgn(dot(a - p, b - p)) <= 0; }
bool onSegment(pt a, pt b, pt c) { return sgn(orient(a, b, c)) == 0 && inDisk(a, b, c); }

// True if segments AB and CD intersect at a single point strictly inside both
bool properInter(pt a, pt b, pt c, pt d, pt &out) {
    T oa = orient(c, d, a), ob = orient(c, d, b), oc = orient(a, b, c), od = orient(a, b, d);
    if (sgn(oa) * sgn(ob) < 0 && sgn(oc) * sgn(od) < 0) {
        out = (a * ob - b * oa) / (ob - oa);
        return true;
    }
    return false;
}

// Returns all unique intersection points of segments AB and CD (handles collinear overlap)
set<pair<ld, ld>> inters(pt a, pt b, pt c, pt d) {
    set<pair<ld, ld>> s; pt out;

    if (properInter(a, b, c, d, out)) return {{out.x, out.y}};

    // Check all four endpoint-on-segment cases (covers shared endpoints and collinear overlap)
    if (onSegment(c, d, a)) s.insert({a.x, a.y});
    if (onSegment(c, d, b)) s.insert({b.x, b.y});
    if (onSegment(a, b, c)) s.insert({c.x, c.y});
    if (onSegment(a, b, d)) s.insert({d.x, d.y});
    return s;
}

pt closestPointOnSegment(pt a, pt b, pt p) {
    if (sgn(abs(b - a)) == 0) return a; // Degenerate segment: both endpoints coincide
    pt ab = b - a;
    T t = dot(p - a, ab) / sq(ab); // Projection scalar
    if (sgn(t) <= 0) return a;     // P is behind A
    if (sgn(t - 1) >= 0) return b; // P is past B
    return a + ab * t;             // P is alongside AB
}

ld segPoint(pt a, pt b, pt p) { return abs(p - closestPointOnSegment(a, b, p)); }

// Shortest distance between two line segments AB and CD
ld segSeg(pt a, pt b, pt c, pt d) {
    pt dummy;
    if (properInter(a, b, c, d, dummy)) return 0; // Intersecting segments distance is 0
    return min({segPoint(a, b, c), segPoint(a, b, d), segPoint(c, d, a), segPoint(c, d, b)});
}

ld pointRay(pt a, pt b, pt p) {
    if (sgn(dot(p - a, b - a)) <= 0) return sqrt(dot(p - a, p - a)); // P behind ray origin
    return abs(cross(b - a, p - a)) / sqrt(dot(b - a, b - a));       // Perpendicular drop
}

// --- 6. POLYGONS & AREA ---
ld areaTriangle(pt a, pt b, pt c) { return abs(cross(b - a, c - a)) / 2.0; }

// Shoelace formula. Returns signed area: positive if CCW, negative if CW.
ld signedAreaPolygon(vector<pt> p) {
    ld area = 0.0;
    for (int i = 0, n = p.size(); i < n; i++) area += cross(p[i], p[(i + 1) % n]);
    return area / 2.0;
}

// Shoelace formula. Always returns positive area regardless of orientation.
ld areaPolygon(vector<pt> p) { return fabsl(signedAreaPolygon(p)); }

bool above(pt a, pt p) { return p.y >= a.y; }
bool crossesRay(pt a, pt p, pt q) { return (above(a, q) - above(a, p)) * sgn(orient(a, p, q)) > 0; }

// Ray-casting algorithm. Even crossings = outside, Odd crossings = inside.
// Only correct for simple (non-self-intersecting) polygons.
bool inPolygon(vector<pt> p, pt a, bool strict = true) {
    int numCrossings = 0;
    for (int i = 0, n = p.size(); i < n; i++) {
        if (onSegment(p[i], p[(i + 1) % n], a)) return !strict;
        numCrossings += crossesRay(a, p[i], p[(i + 1) % n]);
    }
    return numCrossings & 1;
}

// --- 7. PYRAMIDS & 3D ---
T getRegularPolygonArea(int n, T L) { return (n < 3 || L <= 0) ? 0.0 : (n * L * L) / (4.0 * tan(PI / n)); }
T getCircumradiusR(int n, T L) { return (n < 3 || L <= 0) ? 0.0 : L / (2.0 * sin((PI / n))); }
T getPyramidHeight(T lateral_edge, T R) { return (lateral_edge <= R || R < 0) ? 0.0 : sqrt((lateral_edge * lateral_edge) - (R * R)); }
T getPyramidVolume(T A, T H) { return (A <= 0 || H <= 0) ? 0.0 : (A * H) / 3.0; }

// Regular pyramid where ALL edges (base and lateral) have the same length s.
// V = (A*h)/3 where A = base area of n-gon with side s, h = height from base to apex.
T getRegularPyramidVolume(int n, T s) {
    T baseArea = getRegularPolygonArea(n, s);
    T circumradius = getCircumradiusR(n, s);
    T height = getPyramidHeight(s, circumradius);
    return getPyramidVolume(baseArea, height);
}

// --- 8. CIRCLES ---
// Pythagorean theorem. Returns 0 (outside), 1 (tangent), or 2 (secant)
int circleLine(pt c, T r, line l, pair<pt, pt>& out) {
    T dis = r * r - l.sqDist(c);
    if (sgn(dis) < 0) return 0;
    pt p = l.proj(c), dir = l.v / abs(l.v);
    T h = (!sgn(dis)) ? 0 : sqrt(dis); // Half-chord length
    out = {p + dir * h, p - dir * h};
    return 1 + sgn(dis);
}

// Uses Law of Cosines & Projections. Returns 0 (none/identical), 1 (tangent), or 2 (secant).
// Requires: r1 > 0, r2 > 0 (point-circles are guarded and return 0).
int circleCircle(pt c1, T r1, pt c2, T r2, pair<pt, pt>& out) {
    pt v = c2 - c1;
    T dis = abs(v);
    if (sgn(r1) <= 0 || sgn(r2) <= 0) return 0; // Degenerate: point-circle has no intersection
    // Concentric circles: identical (infinite intersections) or one inside the other
    if (sgn(dis) == 0) return 0;
    if (sgn(dis - (r1 + r2)) > 0 || sgn(dis - abs(r1 - r2)) < 0) return 0;
    T cosTheta = clamp((r1 * r1 + dis * dis - r2 * r2) / (2.0 * r1 * dis), (T)-1.0, (T)1.0);
    T d_P = r1 * cosTheta; // Distance to projection
    pt p = c1 + (v / dis) * d_P; // Chord midpoint
    T h_sq = max((T)0, r1 * r1 - d_P * d_P); // Clamp to avoid sqrt of negative
    T h = sqrt(h_sq);
    pt perp = perp_ccw(v) / dis; // Perpendicular direction
    out = {p + perp * h, p - perp * h};
    return (sgn(dis - (r1 + r2)) == 0 || sgn(dis - abs(r1 - r2)) == 0) ? 1 : 2;
}

// Calculates area of two overlapping circular segments.
// Requires: r1 > 0, r2 > 0 (point-circles are guarded and return 0).
T circleIntersectionArea(pt c1, T r1, pt c2, T r2) {
    if (sgn(r1) <= 0 || sgn(r2) <= 0) return 0.0; // Degenerate: point-circle has zero area
    T d = abs(c2 - c1);
    if (sgn(d - (r1 + r2)) >= 0) return 0.0; // Disjoint
    if (sgn(d - abs(r1 - r2)) <= 0) return PI * min(r1, r2) * min(r1, r2); // Strictly inside

    // Central angles using Law of Cosines
    T cos_alpha1 = clamp((r1 * r1 + d * d - r2 * r2) / (2.0 * r1 * d), (T)-1.0, (T)1.0);
    T cos_alpha2 = clamp((r2 * r2 + d * d - r1 * r1) / (2.0 * r2 * d), (T)-1.0, (T)1.0);
    T alpha1 = 2.0 * acos(cos_alpha1);
    T alpha2 = 2.0 * acos(cos_alpha2);

    // Area = Area of sector - Area of triangle
    T area1 = 0.5 * r1 * r1 * (alpha1 - sin(alpha1));
    T area2 = 0.5 * r2 * r2 * (alpha2 - sin(alpha2));
    return area1 + area2;
}
