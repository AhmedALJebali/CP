const ld EPS = 1e-9;
const ld PI = acos(-1.0L);

typedef ld T;
typedef complex<T> pt;
#define x real()
#define y imag()

// ==========================================
// --- 1. MINIMAL DEPENDENCIES FOR ARCS ---
// ==========================================

T sq(pt p) { return p.x * p.x + p.y * p.y; }
T dot(pt v, pt w) { return v.x * w.x + v.y * w.y; }
T cross(pt v, pt w) { return v.x * w.y - v.y * w.x; }

int sgn(T val) {
    if (val > EPS) return 1;
    if (val < -EPS) return -1;
    return 0;
}

bool samePoint(pt a, pt b) {
    return sgn(abs(a - b)) == 0;
}

pt perp_ccw(pt p) { return {-p.y, p.x}; }

T orient(pt a, pt b, pt c) { return cross(b - a, c - a); }

T angle(pt v, pt w) {
    return acos(clamp(dot(v, w) / abs(v) / abs(w), (T)-1.0, (T)1.0));
}

// Returns angle in [0, 2PI)
T orientedAngle(pt a, pt b, pt c) {
  if (orient(a,b,c) >= 0)
    return angle(b-a, c-a);
  else
    return 2*PI - angle(b-a, c-a);
}

// Minimal line struct for circle/arc intersections
struct line {
    pt v;
    T c;

    line(pt p, pt q) : v(q - p), c(cross(v, p)) {}

    T side(pt p) const { return cross(v, p) - c; }
    T sqDist(pt p) const { return side(p) * side(p) / sq(v); }
    pt proj(pt p) const { return p - perp_ccw(v) * side(p) / sq(v); }
};

// Circle-line intersection
int circleLine(pt c, T r, line l, pair<pt, pt>& out) {
    T vv = sq(l.v);
    if (sgn(vv) == 0) return 0; // degenerate line

    T dis = r * r - l.sqDist(c);
    if (sgn(dis) < 0) return 0;

    pt p = l.proj(c), dir = l.v / abs(l.v);
    T h = (sgn(dis) == 0 ? 0 : sqrt(dis));
    out = {p + dir * h, p - dir * h};
    return 1 + sgn(dis);
}

// Circle-circle intersection
int circleCircle(pt c1, T r1, pt c2, T r2, pair<pt, pt>& out) {
    pt v = c2 - c1;
    T dis = abs(v);

    if (sgn(r1) <= 0 || sgn(r2) <= 0) return 0;
    if (sgn(dis) == 0) return 0; // concentric -> none or infinite, both treated as 0 here
    if (sgn(dis - (r1 + r2)) > 0 || sgn(dis - abs(r1 - r2)) < 0) return 0;

    T cosTheta = clamp((r1 * r1 + dis * dis - r2 * r2) / (2.0L * r1 * dis), (T)-1.0, (T)1.0);
    T dP = r1 * cosTheta;
    pt p = c1 + (v / dis) * dP;
    T h = sqrt(max((T)0.0, r1 * r1 - dP * dP));
    pt perp = perp_ccw(v) / dis;

    out = {p + perp * h, p - perp * h};
    return (sgn(dis - (r1 + r2)) == 0 || sgn(dis - abs(r1 - r2)) == 0) ? 1 : 2;
}

// ==========================================
// --- 2. CIRCULAR ARCS ---
// ==========================================
// Arc is the CCW arc from a to b on circle (c, r).

T arcLength(pt c, T r, pt a, pt b) {
    T theta = orientedAngle(c, a, b);
    return r * theta;
}

T arcSectorArea(pt c, T r, pt a, pt b) {
    T theta = orientedAngle(c, a, b);
    return 0.5L * r * r * theta;
}

T arcSegmentArea(pt c, T r, pt a, pt b) {
    T theta = orientedAngle(c, a, b);
    return 0.5L * r * r * (theta - sin(theta));
}

bool isPointOnArc(pt c, T r, pt a, pt b, pt p) {
    if (sgn(abs(p - c) - r) != 0) return false;
    if (samePoint(p, a) || samePoint(p, b)) return true;

    T arc = orientedAngle(c, a, b);
    T pos = orientedAngle(c, a, p);
    return sgn(pos - arc) <= 0;
}

vector<pt> arcLineInter(pt c, T r, pt a, pt b, line l) {
    pair<pt, pt> out;
    int cnt = circleLine(c, r, l, out);
    vector<pt> res;

    if (cnt == 0) return res;
    if (isPointOnArc(c, r, a, b, out.first)) res.push_back(out.first);
    if (cnt > 1 && !samePoint(out.first, out.second) && isPointOnArc(c, r, a, b, out.second))
        res.push_back(out.second);

    return res;
}

vector<pt> arcArcInter(pt c1, T r1, pt a1, pt b1, pt c2, T r2, pt a2, pt b2) {
    pair<pt, pt> out;
    int cnt = circleCircle(c1, r1, c2, r2, out);
    vector<pt> res;

    auto add = [&](pt p) {
        if (!isPointOnArc(c1, r1, a1, b1, p)) return;
        if (!isPointOnArc(c2, r2, a2, b2, p)) return;
        for (auto &q : res) if (samePoint(q, p)) return;
        res.push_back(p);
    };

    if (cnt >= 1) add(out.first);
    if (cnt >= 2) add(out.second);

    return res;
}

T distPointArc(pt c, T r, pt a, pt b, pt p) {
    T d_pc = abs(p - c);
    if (sgn(d_pc) == 0) return r;

    pt proj = c + (p - c) * (r / d_pc);
    if (isPointOnArc(c, r, a, b, proj)) {
        return fabsl(d_pc - r);
    }
    return min(abs(p - a), abs(p - b));
}

pair<pt, pt> arcBoundingBox(pt c, T r, pt a, pt b) {
    T minX = min(a.x, b.x), maxX = max(a.x, b.x);
    T minY = min(a.y, b.y), maxY = max(a.y, b.y);

    vector<pt> extremes = {
        {c.x + r, c.y},
        {c.x, c.y + r},
        {c.x - r, c.y},
        {c.x, c.y - r}
    };

    for (auto &e : extremes) {
        if (isPointOnArc(c, r, a, b, e)) {
            minX = min(minX, e.x);
            maxX = max(maxX, e.x);
            minY = min(minY, e.y);
            maxY = max(maxY, e.y);
        }
    }

    return {{minX, minY}, {maxX, maxY}};
}
