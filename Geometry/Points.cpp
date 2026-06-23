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
    T ampli = angle(b - a, c - a);
    // When collinear and same direction ampli=0, orient<=0 would give 2PI. Fix: return 0.
    if (sgn(ampli) == 0) return 0;
    return sgn(orient(a, b, c)) > 0 ? ampli : 2 * PI - ampli;
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
    // Normalize: ensure ABC is CCW
    if (sgn(orient(a, b, c)) < 0) swap(b, c);
    // P is inside angle ABC iff it is CCW from AB and CW from AC
    return sgn(orient(a, b, p)) > 0 && sgn(orient(a, c, p)) < 0;
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
