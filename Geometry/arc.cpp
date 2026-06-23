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
int sgn(T val) { if(val > EPS) return 1; if(val < -EPS) return -1; return 0; }
bool samePoint(pt a, pt b) { return abs(a - b) <= EPS; }
pt perp_ccw(pt p) { return {-p.y, p.x}; } 

T orient(pt a, pt b, pt c) { return cross(b - a, c - a); }
T angle(pt v, pt w) { return acos(clamp(dot(v, w) / abs(v) / abs(w), (T)-1.0, (T)1.0)); }

T orientedAngle(pt a, pt b, pt c) {
    T ampli = angle(b - a, c - a);
    if (sgn(ampli) == 0) return 0;
    return sgn(orient(a, b, c)) > 0 ? ampli : 2 * PI - ampli;
}

// Minimal line struct for circle/arc intersections
struct line {
    pt v; T c;
    line(pt p, pt q) : v(q - p), c(cross(v, p)) {} 
    T side(pt p) { return cross(v, p) - c; } 
    T sqDist(pt p) { return side(p) * side(p) / (T)sq(v); }
    pt proj(pt p) { return p - perp_ccw(v) * side(p) / sq(v); } 
};

// Circle-Line Intersection (Required for Arc-Line Intersection)
int circleLine(pt c, T r, line l, pair<pt, pt>& out) {
    T dis = r * r - l.sqDist(c);
    if (sgn(dis) < 0) return 0;
    pt p = l.proj(c), dir = l.v / abs(l.v);
    T h = (!sgn(dis)) ? 0 : sqrt(dis); 
    out = {p + dir * h, p - dir * h};
    return 1 + sgn(dis);
}

// Circle-Circle Intersection (Required for Arc-Arc Intersection)
int circleCircle(pt c1, T r1, pt c2, T r2, pair<pt, pt>& out) {
    pt v = c2 - c1; T dis = abs(v);
    if (sgn(r1) <= 0 || sgn(r2) <= 0 || sgn(dis) == 0) return 0; 
    if (sgn(dis - (r1 + r2)) > 0 || sgn(dis - abs(r1 - r2)) < 0) return 0;
    T cosTheta = clamp((r1 * r1 + dis * dis - r2 * r2) / (2.0 * r1 * dis), (T)-1.0, (T)1.0);
    T d_P = r1 * cosTheta; 
    pt p = c1 + (v / dis) * d_P; 
    T h = sqrt(max((T)0.0, r1 * r1 - d_P * d_P)); 
    pt perp = perp_ccw(v) / dis; 
    out = {p + perp * h, p - perp * h};
    return (sgn(dis - (r1 + r2)) == 0 || sgn(dis - abs(r1 - r2)) == 0) ? 1 : 2;
}


// ==========================================
// --- 2. CIRCULAR ARCS ---
// ==========================================
// Note: Arcs are defined by center 'c', radius 'r', and go strictly CCW from 'a' to 'b'.

T arcLength(pt c, T r, pt a, pt b) { return r * orientedAngle(c, a, b); }
T arcSectorArea(pt c, T r, pt a, pt b) { return 0.5L * r * r * orientedAngle(c, a, b); }
T arcSegmentArea(pt c, T r, pt a, pt b) {
    T theta = orientedAngle(c, a, b);
    return 0.5L * r * r * (theta - sin(theta));
}

bool isPointOnArc(pt c, T r, pt a, pt b, pt p) {
    if (sgn(abs(p - c) - r) != 0) return false;
    // Explicitly check endpoints to prevent 2*PI wrap-around precision errors
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
    if (sgn(abs(p - c)) == 0) return r;
    pt proj = c + (p - c) * (r / abs(p - c));
    if (isPointOnArc(c, r, a, b, proj)) return fabsl(abs(p - c) - r);
    return min(abs(p - a), abs(p - b));
}

pair<pt, pt> arcBoundingBox(pt c, T r, pt a, pt b) {
    T minX = min(a.x, b.x), maxX = max(a.x, b.x);
    T minY = min(a.y, b.y), maxY = max(a.y, b.y);
    vector<pt> extremes = {{c.x + r, c.y}, {c.x, c.y + r}, {c.x - r, c.y}, {c.x, c.y - r}};
    for (auto &e : extremes) {
        if (isPointOnArc(c, r, a, b, e)) {
            minX = min(minX, e.x); maxX = max(maxX, e.x);
            minY = min(minY, e.y); maxY = max(maxY, e.y);
        }
    }
    return {{minX, minY}, {maxX, maxY}};
}
