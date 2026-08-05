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
T segLineDist(pt a, pt b, line l) {
    if (segLineInter(a, b, l)) return 0;
    return min(l.dist(a), l.dist(b));
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
T segmentUnionLength(vector<pair<T, T>>& segments) {
    if (segments.empty()) return 0.0L;

    struct Event {
        T pos;
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

    T total_length = 0.0L;
    int active_segments = 0;

    for (size_t i = 0; i < events.size(); i++) {
        if (i > 0 && active_segments > 0) {
            total_length += events[i].pos - events[i - 1].pos;
        }
        active_segments += events[i].type;
    }

    return total_length;
}

// ==========================================
// --- O(N log N) SEGMENT INTERSECTION ---
// ==========================================

// Quick boolean check if two segments intersect
// Much faster than segInter() when we only need a yes/no answer.
bool doIntersect(pt a, pt b, pt c, pt d) {
    pt dummy;
    if (properInter(a, b, c, d, dummy)) return true;
    return onSegment(c, d, a) || onSegment(c, d, b) || 
           onSegment(a, b, c) || onSegment(a, b, d);
}

// Bentley-Ottmann Sweep Line Algorithm
// Returns the indices (0-based) of ANY two intersecting segments.
// If no segments intersect, returns {-1, -1}.
pair<int, int> anyIntersection(vector<pair<pt, pt>> segs) {
    int n = segs.size();
    if (n < 2) return {-1, -1};

    // TRICK: Rotate all segments by an arbitrary angle.
    // This perfectly eliminates the "Vertical Line" edge case which usually
    // breaks Sweep Line algorithms, while preserving all valid intersections.
    T ang = 1.00123456789; // Arbitrary angle in radians
    pt rot(cos(ang), sin(ang));
    
    for (int i = 0; i < n; i++) {
        segs[i].first *= rot;
        segs[i].second *= rot;
        // Guarantee left endpoint strictly comes before right endpoint
        if (sgn(segs[i].first.x - segs[i].second.x) > 0 || 
           (sgn(segs[i].first.x - segs[i].second.x) == 0 && sgn(segs[i].first.y - segs[i].second.y) > 0)) {
            swap(segs[i].first, segs[i].second);
        }
    }

    struct Event {
        T pos; int type, id;
        bool operator<(const Event& o) const {
            if (sgn(pos - o.pos) != 0) return pos < o.pos;
            return type > o.type; // Left endpoints (+1) processed before right (-1)
        }
    };

    vector<Event> events;
    for (int i = 0; i < n; i++) {
        events.push_back({segs[i].first.x, 1, i});
        events.push_back({segs[i].second.x, -1, i});
    }
    sort(events.begin(), events.end());

    T sweep_x = 0; // Tracks the current X of the sweep line
    
    // Evaluates the Y coordinate of segment 'id' at the current sweep_x
    auto evalY = [&](int id) {
        pt p = segs[id].first;
        pt q = segs[id].second;
        if (sgn(p.x - q.x) == 0) return p.y; // Safety fallback
        return p.y + (q.y - p.y) * (sweep_x - p.x) / (q.x - p.x);
    };

    // Dynamic Comparator for the active set
    auto cmp = [&](int a, int b) {
        if (a == b) return false;
        T y1 = evalY(a), y2 = evalY(b);
        if (sgn(y1 - y2) != 0) return y1 < y2;
        return a < b; // Fallback to segment ID
    };

    set<int, decltype(cmp)> active(cmp);
    vector<set<int, decltype(cmp)>::iterator> its(n);

    auto check = [&](int i, int j) {
        return doIntersect(segs[i].first, segs[i].second, segs[j].first, segs[j].second);
    };

    for (auto e : events) {
        sweep_x = e.pos; // Must update global sweep_x before any Set operations
        
        if (e.type == 1) { // Left Endpoint (Insert Segment)
            auto it = active.insert(e.id).first;
            its[e.id] = it;
            
            auto nxt = next(it);
            auto prv = (it == active.begin() ? active.end() : prev(it));
            
            // Check intersection with neighbor strictly ABOVE
            if (nxt != active.end() && check(*it, *nxt)) return {*it, *nxt};
            
            // Check intersection with neighbor strictly BELOW
            if (prv != active.end() && check(*it, *prv)) return {*it, *prv};
            
        } else { // Right Endpoint (Remove Segment)
            auto it = its[e.id];
            auto nxt = next(it);
            auto prv = (it == active.begin() ? active.end() : prev(it));
            
            // Before removing, check if the two neighbors (above and below) intersect
            if (nxt != active.end() && prv != active.end() && check(*prv, *nxt)) return {*prv, *nxt};
            
            active.erase(it);
        }
    }
    return {-1, -1}; // No intersections found
}
line getPerpBisector(pt a, pt b) {
    pt mid = (a + b) / (T)2.0;     
    line ab(a, b);             
    return ab.prepThrough(mid);  
}
struct LineKey {
    T a, b, c;
    LineKey(T a, T b, T c) : a(a), b(b), c(c) {
        normalize();
    }
    LineKey(line l) : a(l.v.x), b(l.v.y), c(l.c) {
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
    bool operator==(const LineKey& o) const {
        int cmp = sgn(a - o.a);
        if (cmp != 0) return false;
        cmp = sgn(b - o.b);
        if (cmp != 0) return false;
        return sgn(c - o.c) == 0;
    }
};
// Returns a list of all pairs of intersecting segments (by their 0-based indices).
// Runs in O((N + K) log N) where K is the number of intersecting pairs.
vector<pair<int, int>> findAllIntersectionsBO(const vector<pair<pt, pt>>& original_segs) {
    int n = original_segs.size();
    if (n < 2) return {};
    vector<pair<pt, pt>> segs = original_segs;
    // Rotate all segments to eliminate vertical lines edge cases
    T ang = 1.00123456789;
    pt rot(cos(ang), sin(ang));
    for (int i = 0; i < n; i++) {
        segs[i].first *= rot;
        segs[i].second *= rot;
        if (sgn(segs[i].first.x - segs[i].second.x) > 0 ||
           (sgn(segs[i].first.x - segs[i].second.x) == 0 && sgn(segs[i].first.y - segs[i].second.y) > 0)) {
            swap(segs[i].first, segs[i].second);
        }
    }
    struct Event {
        T xx;
        int type;     // -1 = right endpoint, 0 = intersection, 1 = left endpoint
        int id1, id2; // Segment IDs (id2 is used for intersections)

        bool operator<(const Event& o) const {
            if (sgn(xx - o.xx) != 0) return xx < o.xx;
            if (type != o.type) return type < o.type;
            if (id1 != o.id1) return id1 < o.id1;
            return id2 < o.id2;
        }
    };
    set<Event> events;
    for (int i = 0; i < n; i++) {
        events.insert({segs[i].first.x, 1, i, -1});
        events.insert({segs[i].second.x, -1, i, -1});
    }
    T sweep_x = 0;
    auto evalY = [&](int id, T x_val) {
        pt p = segs[id].first, q = segs[id].second;
        if (sgn(p.x - q.x) == 0) return p.y;
        return p.y + (q.y - p.y) * (x_val - p.x) / (q.x - p.x);
    };
    // Active Set Comparator: Resolves crossings by looking slightly ahead (sweep_x + EPS)
    auto cmp = [&](int a, int b) {
        if (a == b) return false;
        T y1 = evalY(a, sweep_x), y2 = evalY(b, sweep_x);
        if (sgn(y1 - y2) != 0) return y1 < y2;

        // If Y is exactly the same, they are currently crossing!
        // Break tie by evaluating slightly to the right to maintain post-intersection order.
        T y1_next = evalY(a, sweep_x + EPS), y2_next = evalY(b, sweep_x + EPS);
        if (sgn(y1_next - y2_next) != 0) return y1_next < y2_next;
        return a < b;
    };

    set<int, decltype(cmp)> active(cmp);
    set<pair<int, int>> reported; // Tracks pairs we've already found to avoid duplicates
    vector<pair<int, int>> intersections;
    auto check = [&](int i, int j) {
        if (i == -1 || j == -1) return;
        int u = min(i, j), v = max(i, j);
        if (reported.count({u, v})) return;
        pt p;
        if (properInter(segs[u].first, segs[u].second, segs[v].first, segs[v].second, p)) {
            // Strictly intersects: schedule an event to swap them in the active set
            if (sgn(p.x - sweep_x) >= 0) {
                events.insert({p.x, 0, u, v});
            }
        } else if (doIntersect(segs[u].first, segs[u].second, segs[v].first, segs[v].second)) {
            // Intersects at endpoints (or collinear overlap). No active-set swap needed.
            reported.insert({u, v});
            intersections.push_back({u, v});
        }
    };
    while (!events.empty()) {
        auto e = *events.begin();
        events.erase(events.begin());
        sweep_x = e.xx;
        if (e.type == 1) { // Left Endpoint
            auto it = active.insert(e.id1).first;
            auto nxt = next(it), prv = (it == active.begin() ? active.end() : prev(it));
            if (nxt != active.end()) check(*it, *nxt);
            if (prv != active.end()) check(*it, *prv);

        } else if (e.type == -1) { // Right Endpoint
            auto it = active.find(e.id1);
            if (it != active.end()) {
                auto nxt = next(it), prv = (it == active.begin() ? active.end() : prev(it));
                if (nxt != active.end() && prv != active.end()) check(*prv, *nxt);
                active.erase(it);
            }

        } else { // Intersection Event (Swap segments in the active set)
            int u = e.id1, v = e.id2;
            if (reported.count({u, v})) continue;
            reported.insert({u, v});
            intersections.push_back({u, v});

            auto it1 = active.find(u), it2 = active.find(v);
            if (it1 != active.end() && it2 != active.end()) {
                active.erase(it1);
                active.erase(it2);

                // Re-inserting them at the intersection point automatically swaps them
                // because evalY uses `sweep_x + EPS` to break the tie.
                auto nIt1 = active.insert(u).first;
                auto nIt2 = active.insert(v).first;

                auto nxt1 = next(nIt1), prv1 = (nIt1 == active.begin() ? active.end() : prev(nIt1));
                auto nxt2 = next(nIt2), prv2 = (nIt2 == active.begin() ? active.end() : prev(nIt2));

                if (nxt1 != active.end()) check(*nIt1, *nxt1);
                if (prv1 != active.end()) check(*nIt1, *prv1);
                if (nxt2 != active.end()) check(*nIt2, *nxt2);
                if (prv2 != active.end()) check(*nIt2, *prv2);
            }
        }
    }

    return intersections;
}
