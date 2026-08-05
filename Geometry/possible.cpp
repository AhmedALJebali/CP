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
  // Constructor builds everything from two points
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
};
