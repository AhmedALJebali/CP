const ld EPS=1e-9;
const ld PI = acos(-1.0);

typedef ld T;
typedef complex<T> pt;
#define x real()
#define y imag()

T sq(pt p){
    return p.x * p.x + p.y * p.y;
}

T dot(pt v, pt w){
    return v.x * w.x + v.y * w.y;
}

T cross(pt v, pt w){
    return v.x * w.y - v.y * w.x;
}

int sgn(T val){
    if(val > EPS) return 1;
    if(val < -EPS) return -1;
    return 0;
}

bool isPerp(pt v, pt w){
    return fabs(dot(v, w)) < EPS;
}

pt prep(pt p){
    return {-p.y, p.x};
}

//////////////// TRASFORMATIONS ////////////////////////
pt translate(pt v, pt p) {return p+v;}

pt scale(pt c, ld factor, pt p) {
    return c + (p-c)*factor;
}

pt rot(pt p, pt c, ld a){
    pt v = p - c;
    pt rotate = {cos(a), sin(a)};
    return c + rotate * v;
}

pt linearTransfo(pt p, pt q, pt r, pt fp, pt fq) {
    return fp + (r-p) * (fq-fp) / (q-p);
}

/////////////// Angles //////////////////////////

T orient(pt a, pt b, pt c){
    return cross(b - a, c - a);
}

T angle(pt v, pt w){
    return acos(clamp(dot(v, w) / abs(v) / abs(w), (T)-1.0, (T)1.0));
}

T orientedAngle(pt a, pt b, pt c){
    ld ampli = angle(b - a, c - a);
    if(orient(a, b, c) > 0) return ampli;
    else return 2 * PI - ampli;
}

T angleTravelled(pt a, pt b, pt c){
    ld ampli = angle(b - a, c - a);
    if (orient(a, b, c) > 0) return ampli;
    if (orient(a, b, c) < 0) return -ampli;
    return 0;
}

//check p in between angle(bac) counter clockwise
bool inAngle(pt a, pt b, pt c, pt p) {
    T abp = orient(a, b, p), acp = orient(a, c, p), abc = orient(a, b, c);
    if (abc < 0) swap(abp, acp);
    return (abp >= 0 && acp <= 0) ^ (abc < 0);
}

struct line{
    pt v; T c;

    line(pt v, T c) : v(v), c(c) {}

    // from equation ax+by = c
    line(T a, T b, T _c){
        v = {b, -a};
        c = _c;
    }

    //line from two points
    line(pt p, pt q){
        v = q - p;
        c = cross(v, p);
    }

    T side(pt p) {return cross(v,p)-c;}
    ld dist(pt p) {return abs(side(p)) / abs(v);}
    double sqDist(pt p) {return side(p)*side(p) / (T)sq(v);}
    line prepThrought(pt p){ return {p, p + prep(v)};}
    bool cmpProj(pt p, pt q){
        return dot(v, p) < dot(v, q);
    }
    line translate(pt t) {return {v, c + cross(v,t)};}
    line shiftLeft(T dist) {return {v, c + dist*abs(v)};}
    pt proj(pt p) {return p - prep(v)*side(p)/sq(v);}
    pt refl(pt p) {return p - prep(v) * (T)2.0 * side(p)/sq(v);}
};

bool inter(line l1, line l2, pt &out) {
    T d = cross(l1.v, l2.v);
    if (sgn(d) == 0) return false;
    out = (l2.v*l1.c - l1.v*l2.c) / d;
    return true;
}

line bisector(line l1, line l2, bool interior) {
    assert(cross(l1.v, l2.v) != 0); // l1 and l2 cannot be parallel!
    ld sign = interior ? 1 :-1;
    return {l2.v/abs(l2.v) + l1.v/abs(l1.v) * sign,
            l2.c/abs(l2.v) + l1.c/abs(l1.v) * sign};
}

//////////////////////////////////////////  SEGMENTS   //////////////////////////////////////////

bool inDisk(pt a, pt b, pt p) {
    return sgn(dot(a-p, b-p)) <= 0;
}

bool onSegment(pt a, pt b, pt c){
    return sgn(orient(a,b,c)) == 0 && inDisk(a,b,c);
}

bool properInter(pt a, pt b, pt c, pt d, pt &out) {
    T oa = orient(c,d,a),
            ob = orient(c,d,b),
            oc = orient(a,b,c),
            od = orient(a,b,d);
// Proper intersection exists iff opposite signs
    if (sgn(oa)*sgn(ob) < 0 && sgn(oc)*sgn(od) < 0) {
        out = (a*ob - b*oa) / (ob-oa);
        return true;
    }
    return false;
}

set<pair<ld,ld>> inters(pt a, pt b, pt c, pt d) {
    set<pair<ld,ld>> s;
    pt out;
    if(a == c || a == d){
        s.insert(make_pair(a.x, a.y));
    }
    if(b == c || b == d){
        s.insert(make_pair(b.x, b.y));
    }
    if(s.size()) return s;

    if (properInter(a,b,c,d,out)) return {make_pair(out.x, out.y)};
    if (onSegment(c,d,a)) s.insert(make_pair(a.x, a.y));
    if (onSegment(c,d,b)) s.insert(make_pair(b.x, b.y));
    if (onSegment(a,b,c)) s.insert(make_pair(c.x, c.y));
    if (onSegment(a,b,d)) s.insert(make_pair(d.x, d.y));

    return s;
}
pt closestPointOnSegment(pt a, pt b, pt p) {
    if (a == b) return a;

    pt ab = b - a;
    T t = dot(p - a, ab) / sq(ab);

    if (sgn(t) <= 0) return a;
    if (sgn(t - 1) >= 0) return b;

    return a + ab * t;
}
ld segPoint(pt a, pt b, pt p) {
    pt q = closestPointOnSegment(a, b, p);
    return abs(p - q);
}
ld segSeg(pt a, pt b, pt c, pt d) {
    pt dummy;
    if (properInter(a,b,c,d,dummy))
        return 0;
    return min({segPoint(a,b,c), segPoint(a,b,d),
                segPoint(c,d,a), segPoint(c,d,b)});
}

////////////////////// polygons ////////////////////////////

ld areaTriangle(pt a, pt b, pt c) {
    return abs(cross(b-a, c-a)) / 2.0;
}

ld areaPolygon(vector<pt> p) {
    ld area = 0.0;
    for (int i = 0, n = p.size(); i < n; i++) {
        area += cross(p[i], p[(i+1)%n]); // wrap back to 0 if i == n - 1
    }
    return abs(area) / 2.0;
}

bool above(pt a, pt p) {
    return p.y >= a.y;
}
// check if [PQ] crosses ray from A
bool crossesRay(pt a, pt p, pt q) {
    return (above(a,q)- above(a,p)) * orient(a,p,q) > 0;
}

bool inPolygon(vector<pt> p, pt a, bool strict = true) {
    int numCrossings = 0;
    for (int i = 0, n = p.size(); i < n; i++) {
        if (onSegment(p[i], p[(i+1)%n], a))
            return !strict;
        numCrossings += crossesRay(a, p[i], p[(i+1)%n]);
    }
    return numCrossings & 1; // inside if odd number of crossings
}

// 1. Calculate the area of a regular polygon base
T getRegularPolygonArea(int n, T L) {
    if (n < 3 || L <= 0) {
        return 0.0;
    }
    return (n * L * L) / (4.0 * tan(PI / n));
}

// 2. Calculate the circumradius (distance from center to a base vertex)
T getCircumradiusR(int n, T L) {
    if (n < 3 || L <= 0) {
        return 0.0;
    }
    return L / (2.0 * sin((PI / n)));
}

// 3. Calculate the pyramid's height based on the lateral edge and circumradius
T getPyramidHeight(T lateral_edge, T R) {
    if (lateral_edge <= R || lateral_edge <= 0 || R < 0) {
        return 0.0;
    }
    return sqrt((lateral_edge * lateral_edge) - (R * R));
}

// 4. Calculate the final volume based on area and height
T getPyramidVolume(T A, T H) {
    if (A <= 0 || H <= 0) {
        return 0.0;
    }
    return (A * H) / 3.0;
}

// ---------------------------------------------------------
// The Composite Function
// Computes the volume of a regular pyramid where ALL edges
// (both base and lateral) are equal to 's'.
// ---------------------------------------------------------
T getEquilateralPyramidVolume(int n, T s) {
    // Step 1: Get the base area (edge length is s)
    T baseArea = getRegularPolygonArea(n, s);

    // Step 2: Get the circumradius of the base
    T circumradius = getCircumradiusR(n, s);

    // Step 3: Get the height
    // (Since all edges are equal, the lateral edge is also s)
    T height = getPyramidHeight(s, circumradius);

    // Step 4: Calculate and return the final volume
    return getPyramidVolume(baseArea, height);
}
T toDegrees(T radians) {
    return radians * (180.0 / PI);
}
T toRadians(T degrees) {
    return degrees * (PI / 180.0);
}
/**
 * Calculates the optimal point P on line_L such that the total distance
 * dist(A, P) + dist(P, B) is minimized.
 * * Uses the Reflection Principle:
 * The shortest path touching a line is equivalent to a straight line
 * connecting the reflected image of one point to the other point.
 */
pt getOptimalPoint(pt A, pt B, line line_L) {
    // 1. Reflect point A across the constraint line (L) to create a virtual point A'.
    // Geometrically, any path from A to P + P to B is equal in length
    // to A' to P + P to B.
    pt A_prime = line_L.refl(A);

    // 2. Construct a straight line passing through the reflected point A' and destination B.
    // The straight line path A' -> B is the shortest possible distance between the two.
    line path(A_prime, B);

    // 3. Find the intersection of this straight-line path with the constraint line (L).
    // This intersection point (intersectionPoint) is the optimal point P
    // where the shortest path touches the line L.
    pt intersectionPoint;
    inter(path, line_L, intersectionPoint);

    return intersectionPoint;
}

pt gt() {
    T xx, yy;
    cin>>xx>>yy;
    return pt(xx,yy);
}
