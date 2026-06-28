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
pt translate(pt p, pt v) { return p + v; }
pt scale(pt c, T factor, pt p) { return c + (p - c) * factor; }
pt rot(pt p, pt c, T a) { return c + (p - c) * pt(cos(a), sin(a)); } // Complex mult is 2D rot
// Maps point r from coordinate system (p, q) to (fp, fq) using complex division
pt linearTransfo(pt p, pt q, pt r, pt fp, pt fq) { return fp + (r - p) * (fq - fp) / (q - p); }

// --- 3. ANGLES ---
T toDegrees(T rad) { return rad * (180.0 / PI); }
T toRadians(T deg) { return deg * (PI / 180.0); }
T orient(pt a, pt b, pt c) { return cross(b - a, c - a); } // Orientation of C relative to AB
T angle(pt v, pt w) { return acos(clamp(dot(v, w) / abs(v) / abs(w), (T)-1.0, (T)1.0)); }

// Returns angle [0, 2PI) moving strictly Counter-Clockwise from AB to AC
T orientedAngle(pt a, pt b, pt c) {
  if (orient(a,b,c) >= 0)
    return angle(b-a, c-a);
  else
    return 2*PI - angle(b-a, c-a);
}
// Returns shortest rotational path (-PI, PI]. + is CCW (Left), - is CW (Right)
T angleTravelled(pt a, pt b, pt c) {
    T ampli = angle(b - a, c - a);
    if (sgn(orient(a, b, c)) > 0) return ampli;
    if (sgn(orient(a, b, c)) < 0) return -ampli;
    // Collinear case: same direction => 0, opposite directions => PI
    return (sgn(dot(b - a, c - a)) >= 0) ? (T)0 : PI;
}

// Checks if ray AP is strictly between rays AB and AC
bool inAngle(pt a, pt b, pt c, pt p) {
  T abp = orient(a, b, p), acp = orient(a, c, p), abc = orient(a, b, c);
  if (abc < 0) swap(abp, acp);
  return (abp >= 0 && acp <= 0) ^ (abc < 0);
}
bool inDisk(pt a, pt b, pt p) { return sgn(dot(a - p, b - p)) <= 0; }
bool onSegment(pt a, pt b, pt c) { return sgn(orient(a, b, c)) == 0 && inDisk(a, b, c); }
bool samePoint(pt a, pt b) {return abs(a - b) <= EPS;}

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

// ==========================================
// --- CLOSEST PAIR OF POINTS O(N log N) ---
// ==========================================

// Custom comparators for the complex<T> point type using strictly sgn()
bool cmpX(const pt& a, const pt& b) {
    if (sgn(a.x - b.x) != 0) return sgn(a.x - b.x) < 0;
    return sgn(a.y - b.y) < 0;
}

bool cmpY(const pt& a, const pt& b) {
    if (sgn(a.y - b.y) != 0) return sgn(a.y - b.y) < 0;
    return sgn(a.x - b.x) < 0;
}

// Returns a pair containing:
// 1. A pair of the two closest points {pt1, pt2}
// 2. The minimum distance between them
pair<pair<pt, pt>, T> closestPair(vector<pt> pts) {
    int n = pts.size();
    if (n < 2) return {{pt(0, 0), pt(0, 0)}, 0.0L};

    // Initial sort by X coordinate
    sort(pts.begin(), pts.end(), cmpX);

    // Initialize with the distance between the first two points
    // This perfectly avoids using arbitrary "infinity" values
    T best_dist = abs(pts[0] - pts[1]);
    pair<pt, pt> best_pair = {pts[0], pts[1]};

    // Helper to conditionally update the minimum distance
    auto upd = [&](const pt& a, const pt& b) {
        T d = abs(a - b);
        if (sgn(d - best_dist) < 0) {
            best_dist = d;
            best_pair = {a, b};
        }
    };

    vector<pt> t(n); // Pre-allocated temporary array for the merge step and strip

    // Recursive Divide and Conquer using a lambda
    auto solve = [&](auto& self, int l, int r) -> void {
        // Base case: 3 or fewer points. Brute force and sort by Y.
        if (r - l <= 3) {
            for (int i = l; i < r; ++i) {
                for (int j = i + 1; j < r; ++j) {
                    upd(pts[i], pts[j]);
                }
            }
            sort(pts.begin() + l, pts.begin() + r, cmpY); 
            return;
        }

        int mid = (l + r) >> 1;
        pt mid_pt = pts[mid];

        // Solve left and right halves
        self(self, l, mid);
        self(self, mid, r);

        // Trick to keep it strictly O(N log N): 
        // Merge the two Y-sorted halves in O(N) time instead of re-sorting
        merge(pts.begin() + l, pts.begin() + mid,
              pts.begin() + mid, pts.begin() + r,
              t.begin(), cmpY);
        copy(t.begin(), t.begin() + (r - l), pts.begin() + l);

        // Build the central strip of points within `best_dist` of the dividing line
        int tsz = 0;
        for (int i = l; i < r; ++i) {
            if (sgn(abs(pts[i].x - mid_pt.x) - best_dist) < 0) {
                // Check against previously added points in the strip.
                // Geometrically proven to run at most 6 times per point!
                for (int j = tsz - 1; j >= 0 && sgn(pts[i].y - t[j].y - best_dist) < 0; --j) {
                    upd(pts[i], t[j]);
                }
                t[tsz++] = pts[i];
            }
        }
    };

    solve(solve, 0, n);
    return {best_pair, best_dist};
}
