const ld EPS = 1e-9;
const ld PI = acos(-1.0L);
typedef ld T;
typedef complex<T> pt;
#define x real()
#define y imag()
// ==========================================
// the different Line Forms
// ==========================================
struct FormSlopeIntercept { T m, b; bool isVertical; };
struct FormPointSlope     { pt p; T m; bool isVertical; };
struct FormParametric     { pt p, v; };
struct FormVectorNormal   { pt n, p; };
struct FormStandard       { T a, b_coeff, c; }; // ax + by = c

// ==========================================
// Unified 2D Line Structure
// ==========================================
struct Line {
  pt p;
  pt v;
  pt n;
  T m;
  T b;
  bool isVertical;
  Line() {}
  Line(pt P, pt Q) {
    p = P;
    v = Q - P;
    n = pt(-v.y, v.x);

    if (fabs(v.x) < EPS) {
      isVertical = true;
      m = 0;
      b = p.x;
    } else {
      isVertical = false;
      m = v.y / v.x;
      b = p.y - (m * p.x);
    }
  }
  Line(pt P, T input_m) {
    p = P;
    if (isinf(input_m)) {
      isVertical = true;
      m = 0;
      b = p.x;
      v = pt(0, 1);
      n = pt(-1, 0);
    }
    else if (fabs(input_m) < EPS) {
      isVertical = false;
      m = 0;
      b = p.y;
      v = pt(1, 0);
      n = pt(0, 1);
    }
    else {
      isVertical = false;
      m = input_m;
      b = p.y - (m * p.x);
      v = pt(1, m);
      n = pt(-m, 1);
    }
  }
  // Line L = Line::fromPointAndDir(P, dir);
  static Line fromPointAndDir(pt P, pt dir) {
    Line L;
    L.p = P;
    L.v = dir;
    L.n = pt(-dir.y, dir.x);
    if (fabs(dir.x) < EPS) {
      L.isVertical = true;
      L.m = 0;
      L.b = P.x;
    } else {
      L.isVertical = false;
      L.m = dir.y / dir.x;
      L.b = P.y - (L.m * P.x);
    }
    return L;
  }
  // 1. Slope-Intercept: y = mx + b
  FormSlopeIntercept getSlopeIntercept() {
    return {m, b, isVertical};
  }
  // 2. Point-Slope: (y - p.y) = m(x - p.x)
  FormPointSlope getPointSlope() {
    return {p, m, isVertical};
  }
  // 3. Parametric: r(t) = p + v*t
  FormParametric getParametric() {
    return {p, v};
  }
  // 4. Vector Normal: n . (r - p) = 0
  FormVectorNormal getVectorNormal() {
    return {n, p};
  }
  // 5. Standard: ax + by = c
  FormStandard getStandard() {
    if (isVertical) {
      return {1, 0, b};  // 1x + 0y = b
    } else {
      return {-m, 1, b}; // -mx + 1y = b
    }
  }
  bool isParallel(Line other) {
    // Cross product of their direction vectors == 0
    return fabs(v.x * other.v.y - v.y * other.v.x) < EPS;
  }

  bool isPerpendicular(Line other) {
    // Dot product of their direction vectors == 0
    return fabs(v.x * other.v.x + v.y * other.v.y) < EPS;
  }
  Line perp_through_point_on_segmentab(pt a, pt b, pt X) {
    return Line::fromPointAndDir(X, Line(a, b).n);
  }
};
