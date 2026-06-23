const ld EPS = 1e-9;
const ld PI = acos(-1.0L);

typedef ld T;
typedef complex<T> pt;
#define x real()
#define y imag()

// ==========================================
// --- 1. MINIMAL DEPENDENCIES ---
// ==========================================

T sq(pt p) { return p.x * p.x + p.y * p.y; }
T dot(pt v, pt w) { return v.x * w.x + v.y * w.y; }
T cross(pt v, pt w) { return v.x * w.y - v.y * w.x; }

int sgn(T val) {
    if (val > EPS) return 1;
    if (val < -EPS) return -1;
    return 0;
}

pt perp_ccw(pt p) { return {-p.y, p.x}; }

// ==========================================
// --- 2. TRIANGLES ---
// ==========================================

T areaTriangle(pt a, pt b, pt c) {
    return abs(cross(b - a, c - a)) / 2.0L;
}

// Heron's Formula
T areaTriangle(T a, T b, T c) {
    if (!((sgn(a) > 0) && (sgn(b) > 0) && (sgn(c) > 0))) return 0;
    T s = (a + b + c) / 2.0L;
    return sqrt(max((T)0.0L, s * (s - a) * (s - b) * (s - c)));
}

bool isValidTriangle(T a, T b, T c) {
    if (sgn(a) <= 0 || sgn(b) <= 0 || sgn(c) <= 0) return false;
    return sgn(a + b - c) > 0 &&
           sgn(a + c - b) > 0 &&
           sgn(b + c - a) > 0;
}

bool isNonDegenerateTriangle(pt a, pt b, pt c) {
    return sgn(cross(b - a, c - a)) != 0;
}

T perimeterTriangle(pt a, pt b, pt c) {
    return abs(a - b) + abs(b - c) + abs(c - a);
}

pt circumCenter(pt a, pt b, pt c) {
    b -= a;
    c -= a;

    T d = 2.0L * cross(b, c);
    if (sgn(d) == 0)
        return {numeric_limits<T>::quiet_NaN(),
                numeric_limits<T>::quiet_NaN()};

    pt ans = perp_ccw(b * dot(c, c) - c * dot(b, b)) / d;
    return a + ans;
}

pt inCenter(pt a, pt b, pt c) {
    if (!isNonDegenerateTriangle(a, b, c))
        return {numeric_limits<T>::quiet_NaN(),
                numeric_limits<T>::quiet_NaN()};

    T la = abs(b - c);
    T lb = abs(a - c);
    T lc = abs(a - b);

    return (a * la + b * lb + c * lc) / (la + lb + lc);
}

pt orthocenter(pt a, pt b, pt c) {
    if (!isNonDegenerateTriangle(a, b, c))
        return {numeric_limits<T>::quiet_NaN(),
                numeric_limits<T>::quiet_NaN()};

    pt o = circumCenter(a, b, c);
    return a + b + c - 2.0L * o;
}

// Inradius: r = A / s
T inRadius(pt a, pt b, pt c) {
    if (!isNonDegenerateTriangle(a, b, c)) return 0;

    T A = areaTriangle(a, b, c);
    T s = perimeterTriangle(a, b, c) / 2.0L;

    return A / s;
}

// Circumradius: R = abc / (4A)
T circumRadius(pt a, pt b, pt c) {
    if (!isNonDegenerateTriangle(a, b, c)) return 0;

    T x = abs(a - b);
    T y = abs(b - c);
    T z = abs(c - a);

    T A = areaTriangle(a, b, c);

    return x * y * z / (4.0L * A);
}

// ==========================================
// --- 3. REGULAR POLYGONS & PYRAMIDS ---
// ==========================================

// Regular n-gon area
T getRegularPolygonArea(int n, T L) {
    if (n < 3 || sgn(L) <= 0) return 0.0L;
    return (n * L * L) / (4.0L * tan(PI / n));
}

// Circumradius of regular n-gon
T getCircumradiusR(int n, T L) {
    if (n < 3 || sgn(L) <= 0) return 0.0L;
    return L / (2.0L * sin(PI / n));
}

// Pyramid height from lateral edge and base circumradius
T getPyramidHeight(T lateral_edge, T R) {
    if (sgn(lateral_edge) <= 0 || sgn(R) < 0) return 0.0L;
    if (lateral_edge < R - EPS) return 0.0L;

    return sqrt(max((T)0.0L,
                    lateral_edge * lateral_edge - R * R));
}

// Pyramid volume
T getPyramidVolume(T A, T H) {
    if (sgn(A) <= 0 || sgn(H) <= 0) return 0.0L;
    return (A * H) / 3.0L;
}

// Regular pyramid where ALL edges equal s
T getRegularPyramidVolume(int n, T s) {
    if (n < 3 || sgn(s) <= 0) return 0.0L;

    T baseArea = getRegularPolygonArea(n, s);
    T R = getCircumradiusR(n, s);
    T H = getPyramidHeight(s, R);

    return getPyramidVolume(baseArea, H);
}
