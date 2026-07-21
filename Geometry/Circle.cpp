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
// Line
// =====================

struct line {
    pt v; 
    T c; // cross(v, p) = c

    line(pt v, T c) : v(v), c(c) {}
    line(T a, T b, T _c) : v(b, -a), c(_c) {}
    line(pt p, pt q) : v(q - p), c(cross(v, p)) {}

    T side(pt p) const { return cross(v, p) - c; }
    T dist(pt p) const { return abs(side(p)) / abs(v); }
    T sqDist(pt p) const { return side(p) * side(p) / sq(v); }

    line translate(pt t) const { return {v, c + cross(v, t)}; }
    line shiftLeft(T d) const { return {v, c + d * abs(v)}; }

    pt proj(pt p) const { return p - perp_ccw(v) * side(p) / sq(v); }
    pt refl(pt p) const { return p - perp_ccw(v) * (T)2.0L * side(p) / sq(v); }
};

bool inter(line l1, line l2, pt &out) {
    T d = cross(l1.v, l2.v);
    if (sgn(d) == 0) return false;
    out = (l2.v * l1.c - l1.v * l2.c) / d;
    return true;
}

// =====================
// Circle
// =====================

// 0 = none, 1 = tangent, 2 = secant
int circleLine(pt c, T r, line l, pair<pt, pt>& out) {
    T dis = r * r - l.sqDist(c);
    if (sgn(dis) < 0) return 0;

    pt p = l.proj(c);
    pt dir = l.v / abs(l.v);
    T h = (sgn(dis) == 0 ? 0 : sqrt(dis));

    out = {p + dir * h, p - dir * h};
    return 1 + sgn(dis);
}

// 0 = none / concentric / identical treated as 0, 1 = tangent, 2 = secant
int circleCircle(pt c1, T r1, pt c2, T r2, pair<pt, pt>& out) {
    pt v = c2 - c1;
    T d = abs(v);

    if (sgn(r1) <= 0 || sgn(r2) <= 0) return 0;
    if (sgn(d) == 0) return 0;
    if (sgn(d - (r1 + r2)) > 0 || sgn(d - abs(r1 - r2)) < 0) return 0;

    T cosTheta = clamp((r1 * r1 + d * d - r2 * r2) / (2.0L * r1 * d), (T)-1.0, (T)1.0);
    T dP = r1 * cosTheta;
    pt p = c1 + (v / d) * dP;

    T h2 = max((T)0.0, r1 * r1 - dP * dP);
    T h = sqrt(h2);
    pt perp = perp_ccw(v) / d;

    out = {p + perp * h, p - perp * h};
    return (sgn(d - (r1 + r2)) == 0 || sgn(d - abs(r1 - r2)) == 0) ? 1 : 2;
}

bool onCircle(pt c, T r, pt p) {
    return sgn(abs(p - c) - r) == 0;
}

bool inDisk(pt c, T r, pt p) {
    return sgn(abs(p - c) - r) <= 0;
}

// Area of overlap between two circles
T circleIntersectionArea(pt c1, T r1, pt c2, T r2) {
    if (sgn(r1) <= 0 || sgn(r2) <= 0) return 0.0L;

    T d = abs(c2 - c1);
    if (sgn(d - (r1 + r2)) >= 0) return 0.0L;
    if (sgn(d - abs(r1 - r2)) <= 0) return PI * min(r1, r2) * min(r1, r2);

    T a1 = clamp((r1 * r1 + d * d - r2 * r2) / (2.0L * r1 * d), (T)-1.0, (T)1.0);
    T a2 = clamp((r2 * r2 + d * d - r1 * r1) / (2.0L * r2 * d), (T)-1.0, (T)1.0);

    T ang1 = 2.0L * acos(a1);
    T ang2 = 2.0L * acos(a2);

    T area1 = 0.5L * r1 * r1 * (ang1 - sin(ang1));
    T area2 = 0.5L * r2 * r2 * (ang2 - sin(ang2));
    return area1 + area2;
}

// Circumcenter of triangle ABC
pt circumCenter(pt a, pt b, pt c) {
    b = b - a, c = c - a;
    T d = 2.0L * cross(b, c);
    if (sgn(d) == 0) return {numeric_limits<T>::quiet_NaN(), numeric_limits<T>::quiet_NaN()};
    pt ans = perp_ccw(b * dot(c, c) - c * dot(b, b)) / d;
    return a + ans;
}

// Tangents between two circles.
// inner = false -> outer tangents
// inner = true  -> inner tangents
// Returns count: 0, 1, or 2.
// Each pair in out = {touch point on first circle, touch point on second circle}
int tangents(pt o1, T r1, pt o2, T r2, bool inner, vector<pair<pt, pt>>& out) {
    if (inner) r2 = -r2;

    pt d = o2 - o1;
    T dr = r1 - r2;
    T d2 = sq(d);
    T h2 = d2 - dr * dr;

    if (sgn(d2) == 0 || sgn(h2) < 0) return 0;

    T h = sqrt(max((T)0.0, h2));

    for (T sign : {-1.0L, 1.0L}) {
        pt v = (d * dr + perp_ccw(d) * h * sign) / d2;
        out.push_back({o1 + v * r1, o2 + v * r2});
        if (sgn(h2) == 0) break;
    }

    return 1 + (sgn(h2) > 0);
}
enum CircleRelation {
  IDENTICAL,          // Same circle
  DISJOINT,           // No intersection
  EXTERNAL_TANGENT,   // One external tangent point
  INTERSECT,          // Two intersection points
  INTERNAL_TANGENT,   // One internal tangent point
  C1_INSIDE_C2,       // Circle 1 completely inside Circle 2
  C2_INSIDE_C1        // Circle 2 completely inside Circle 1
};

CircleRelation circleRelation(pt c1, ld r1, pt c2, ld r2) {
  ld d = abs(c1 - c2);
  // Same center
  if (sgn(d) == 0) {
    if (sgn(r1 - r2) == 0)
      return IDENTICAL;
    return (r1 < r2 ? C1_INSIDE_C2 : C2_INSIDE_C1);
  }
  // Separate
  if (sgn(d - (r1 + r2)) > 0)
    return DISJOINT;
  // External tangent
  if (sgn(d - (r1 + r2)) == 0)
    return EXTERNAL_TANGENT;

  ld diff = fabsl(r1 - r2);
  // One circle completely inside the other
  if (sgn(d - diff) < 0)
    return (r1 < r2 ? C1_INSIDE_C2 : C2_INSIDE_C1);
  // Internal tangent
  if (sgn(d - diff) == 0)
    return INTERNAL_TANGENT;

  // Two intersection points
  return INTERSECT;
}
ld circleSegmentArea(ld r, ld d){
    return r*r*acos(d/r)-d*sqrt(r*r-d*d);
}

// ==========================================
// --- 8. MINIMUM ENCLOSING CIRCLE ---
// ==========================================
 
pt circumCenter(pt a, pt b, pt c) {
    b = b - a, c = c - a;
    T d = 2.0L * cross(b, c);
    if (sgn(d) == 0) return {numeric_limits<T>::quiet_NaN(), numeric_limits<T>::quiet_NaN()};
    pt ans = perp_ccw(b * dot(c, c) - c * dot(b, b)) / d;
    return a + ans;
}
 
// Welzl's algorithm for Minimum Enclosing Circle in O(N) expected time.
// Returns a pair: {center_point, radius}
pair<pt, T> welzl(vector<pt> P) { // Passed by value so we can shuffle safely
    if (P.empty()) return {pt(0, 0), 0.0L};
    if (P.size() == 1) return {P[0], 0.0L};
    mt19937 gen(1337);
    shuffle(P.begin(), P.end(), gen);
    pt c = P[0];
    T r = 0;
    for (int i = 1; i < (int)P.size(); i++) {
        if (abs(P[i] - c) > r + EPS) {
            c = P[i];
            r = 0;
            for (int j = 0; j < i; j++) {
                if (abs(P[j] - c) > r + EPS) {
                    c = (P[i] + P[j]) / 2.0L;
                    r = abs(P[i] - c);
                    for (int k = 0; k < j; k++) {
                        if (abs(P[k] - c) > r + EPS) {
                            c = circumCenter(P[i], P[j], P[k]);
                            r = abs(P[k] - c);
                        }
                    }
                }
            }
        }
    }
    return {c, r};
}

