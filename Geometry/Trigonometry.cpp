const ld EPS = 1e-9;
const ld PI = acos(-1.0L);

typedef ld T;
typedef complex<T> pt;
#define x real()
#define y imag()
T sq(pt p) { return p.x * p.x + p.y * p.y; }
T dot(pt v, pt w) { return v.x * w.x + v.y * w.y; }
T cross(pt v, pt w) { return v.x * w.y - v.y * w.x; }
int sgn(T val) {
    if (val > EPS) return 1;
    if (val < -EPS) return -1;
    return 0;
}
pt perp_ccw(pt p) { return {-p.y, p.x}; }
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
/*
    Calculates the incenter of a triangle defined by three points.
    (The center of the inscribed circle, where the three angle bisectors meet).
    Input: Three vertices of a triangle (a, b, c).
    Output: The coordinate of the incenter
*/
pt inCenter(pt a, pt b, pt c) {
    if (!isNonDegenerateTriangle(a, b, c))
        return {numeric_limits<T>::quiet_NaN(),
                numeric_limits<T>::quiet_NaN()};
    T la = abs(b - c);
    T lb = abs(a - c);
    T lc = abs(a - b);
    return (a * la + b * lb + c * lc) / (la + lb + lc);
}
/*
    Calculates the orthocenter of a triangle defined by three points.
    (The intersection of the three altitudes of the triangle).
    Input: Three vertices of a triangle (a, b, c).
    Output: The coordinate of the orthocenter. Returns {NaN, NaN} if the triangle is degenerate.
*/
pt orthocenter(pt a, pt b, pt c) {
    if (!isNonDegenerateTriangle(a, b, c))
        return {numeric_limits<T>::quiet_NaN(),
                numeric_limits<T>::quiet_NaN()};

    pt o = circumCenter(a, b, c);
    return a + b + c - 2.0L * o;
}
/*
    Calculates the inradius of a triangle defined by three points.
    (The radius of the largest circle that can fit inside the triangle).
    Input: Three vertices of a triangle (a, b, c).
    Output: The inradius (returns 0 if the points are collinear or coincident).
*/
T inRadius(pt a, pt b, pt c) {
    if (!isNonDegenerateTriangle(a, b, c)) return 0;
    T A = areaTriangle(a, b, c);
    T s = perimeterTriangle(a, b, c) / 2.0L;
    return A / s;
}
/*
    Calculates the circumradius of a triangle defined by three points.
    (The radius of the circle that passes through all three vertices).
    Input: Three vertices of a triangle (a, b, c).
    Output: The circumradius (returns 0 if the points are collinear or coincident).
*/
T circumRadius(pt a, pt b, pt c) {
    if (!isNonDegenerateTriangle(a, b, c)) return 0;
    T x = abs(a - b);
    T y = abs(b - c);
    T z = abs(c - a);
    T A = areaTriangle(a, b, c);
    return x * y * z / (4.0L * A);
}

/*
    Calculates the exact area of a regular polygon.
    Input: Number of sides (n) and the side length (L).
    Output: The total area of the regular polygon.
*/
T getRegularPolygonArea(int n, T L) {
    if (n < 3 || sgn(L) <= 0) return 0.0L;
    return (n * L * L) / (4.0L * tan(PI / n));
}

/*
    Calculates the circumradius (R) of a regular polygon.
    (The distance from the center to any vertex).
    Input: Number of sides (n) and the side length (L).
    Output: The circumradius of the regular polygon.
*/
T getCircumradiusR(int n, T L) {
    if (n < 3 || sgn(L) <= 0) return 0.0L;
    return L / (2.0L * sin(PI / n));
}

/*
    Calculates the perpendicular height of a regular pyramid.
    Input: Length of the lateral edge (from apex to base corner) and the circumradius (R) of the base.
    Output: Perpendicular height of the pyramid (returns 0 if geometry is impossible).
*/
T getPyramidHeight(T lateral_edge, T R) {
    if (sgn(lateral_edge) <= 0 || sgn(R) < 0) return 0.0L;
    if (lateral_edge < R - EPS) return 0.0L;
    return sqrt(max((T)0.0L,lateral_edge * lateral_edge - R * R));
}

/*
    Calculates the volume of any general pyramid or cone.
    Input: Base area (A) and perpendicular height (H).
    Output: Volume of the pyramid.
*/
T getPyramidVolume(T A, T H) {
    if (sgn(A) <= 0 || sgn(H) <= 0) return 0.0L;
    return (A * H) / 3.0L;
}

/*
    Calculates the volume of a regular pyramid where ALL edges (base and lateral) are equal to 's'.
    WARNING: A regular pyramid with all edges equal can only exist for n = 3, 4, or 5.
    Input: Number of sides (n) and edge length (s).
    Output: Volume of the pyramid.
*/
T getRegularPyramidVolume(int n, T s) {
    if (n < 3 || sgn(s) <= 0) return 0.0L;
    T baseArea = getRegularPolygonArea(n, s);
    T R = getCircumradiusR(n, s);
    T H = getPyramidHeight(s, R);
    return getPyramidVolume(baseArea, H);
}
/*
    Checks if a point is strictly inside, on the boundary, or outside a triangle.
    Input: Three triangle vertices (a, b, c) and the point to test (p).
    Output: 
       -1 if strictly INSIDE the triangle.
        0 if on the BOUNDARY of the triangle.
        1 if strictly OUTSIDE the triangle.
*/
int is_point_in_triangle(pt a, pt b, pt c, pt p) {
    if (sgn(cross(b - a,c - a)) < 0) swap(b, c);
    int c1 = sgn(cross(b - a,p - a));
    int c2 = sgn(cross(c - b,p - b));
    int c3 = sgn(cross(a - c,p - c));
    if (c1<0 || c2<0 || c3 < 0) return 1;
    if (c1 + c2 + c3 != 3) return 0;
    return -1;
}
