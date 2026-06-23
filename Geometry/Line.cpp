const ld EPS = 1e-9;
typedef ld T;
typedef complex<T> pt;
#define x real()
#define y imag()

// ======================
// Basic math
// ======================

T sq(pt p) { return p.x * p.x + p.y * p.y; }
T dot(pt v, pt w) { return v.x * w.x + v.y * w.y; }
T cross(pt v, pt w) { return v.x * w.y - v.y * w.x; }
pt perp_ccw(pt p) { return {-p.y, p.x}; }

int sgn(T val) {
    if (val > EPS) return 1;
    if (val < -EPS) return -1;
    return 0;
}

bool samePoint(pt a, pt b) {
    return abs(a - b) <= EPS;
}

// ======================
// Line
// ======================

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

bool inter(line l1, line l2, pt &out) {
    T d = cross(l1.v, l2.v);
    if (sgn(d) == 0) return false;
    out = (l2.v * l1.c - l1.v * l2.c) / d;
    return true;
}
// Bisector of angle. true = interior angle, false = exterior angle
line bisector(line l1, line l2, bool interior) {
    assert(sgn(cross(l1.v, l2.v)) != 0); // Lines must not be parallel
    T sign = interior ? 1 : -1;
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



// ======================
// Segment helpers
// ======================

bool inDisk(pt a, pt b, pt p) {
    return sgn(dot(a - p, b - p)) <= 0;
}

bool onSegment(pt a, pt b, pt p) {
    return sgn(cross(b - a, p - a)) == 0 && inDisk(a, b, p);
}

// Strictly inside both segments
bool properInter(pt a, pt b, pt c, pt d, pt &out) {
    T oa = cross(d - c, a - c);
    T ob = cross(d - c, b - c);
    T oc = cross(b - a, c - a);
    T od = cross(b - a, d - a);

    if (sgn(oa) * sgn(ob) < 0 && sgn(oc) * sgn(od) < 0) {
        out = (a * ob - b * oa) / (ob - oa);
        return true;
    }
    return false;
}

// All intersection points of two segments (0/1/2 points)
vector<pt> segInter(pt a, pt b, pt c, pt d) {
    vector<pt> res;
    pt out;

    auto add = [&](pt p) {
        for (auto &q : res) if (samePoint(p, q)) return;
        res.push_back(p);
    };

    if (properInter(a, b, c, d, out)) {
        add(out);
        return res;
    }

    if (onSegment(c, d, a)) add(a);
    if (onSegment(c, d, b)) add(b);
    if (onSegment(a, b, c)) add(c);
    if (onSegment(a, b, d)) add(d);

    return res;
}

pt closestPointOnSegment(pt a, pt b, pt p) {
    if (sgn(abs(b - a)) == 0) return a; // degenerate segment
    pt ab = b - a;
    T t = dot(p - a, ab) / sq(ab);
    if (sgn(t) <= 0) return a;
    if (sgn(t - 1) >= 0) return b;
    return a + ab * t;
}

T segPoint(pt a, pt b, pt p) {
    return abs(p - closestPointOnSegment(a, b, p));
}

T segSeg(pt a, pt b, pt c, pt d) {
    pt dummy;
    if (properInter(a, b, c, d, dummy)) return 0;
    return min({segPoint(a, b, c), segPoint(a, b, d),
                segPoint(c, d, a), segPoint(c, d, b)});
}

// ======================
// Ray helpers
// ======================

bool onRay(pt a, pt b, pt p) {
    return sgn(cross(b - a, p - a)) == 0 && sgn(dot(p - a, b - a)) >= 0;
}

// Ray AB intersects line l?
bool rayLineInter(pt a, pt b, line l) {
    line r(a, b);
    pt out;

    if (inter(r, l, out)) {
        return sgn(dot(out - a, b - a)) >= 0;
    }

    // Parallel case: intersection exists only if collinear
    return sgn(l.side(a)) == 0;
}

T rayLine(pt a, pt b, line l) {
    if (rayLineInter(a, b, l)) return 0;
    return l.dist(a);
}

// One common point of two rays if they intersect.
// For collinear overlapping rays, returns one valid common endpoint.
bool rayInter(pt a, pt b, pt c, pt d, pt &p) {
    line l1(a, b), l2(c, d);

    if (inter(l1, l2, p)) {
        return onRay(a, b, p) && onRay(c, d, p);
    }

    if (sgn(l1.side(c)) != 0) return false; // parallel but not collinear

    if (onRay(a, b, c)) { p = c; return true; }
    if (onRay(c, d, a)) { p = a; return true; }

    return false;
}

T rayDist(pt a, pt b, pt p) {
    line l(a, b);
    if (sgn(dot(p - a, b - a)) >= 0) return l.dist(p);
    return abs(p - a);
}

T rayRayDist(pt a1, pt b1, pt a2, pt b2) {
    pt p;
    if (rayInter(a1, b1, a2, b2, p)) return 0;
    return min(rayDist(a1, b1, a2), rayDist(a2, b2, a1));
}

// ==========================================
// . 1D SEGMENT UNION (SWEEP LINE)
// ==========================================
T segmentUnionLength(const vector<pair<T, T>>& segments) {
    struct Event {
        T pos;    // Renamed from 'x' to 'pos' to avoid #define x real() macro collision
        int type; 
        
        bool operator<(const Event& o) const {
            if (sgn(pos - o.pos) != 0) return pos < o.pos;
            return type > o.type; 
        }
    };
    vector<Event> events;
    events.reserve(segments.size() * 2);

    for (const auto& seg : segments) {
        T l = min(seg.first, seg.second);
        T r = max(seg.first, seg.second);
        if (sgn(r - l) == 0) continue; 

        events.push_back({l, 1});
        events.push_back({r, -1});
    }

    sort(events.begin(), events.end());

    T result = 0.0L;
    int active_segments = 0;

    for (size_t i = 0; i < events.size(); i++) {
        if (i > 0 && active_segments > 0) {
            T dist = events[i].pos - events[i - 1].pos;
            result += max((T)0.0L, dist); 
        }
        active_segments += events[i].type;
    }

    return result;
}
