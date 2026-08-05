const ld EPS = 1e-9;
const ld PI  = acos(-1.0L);

typedef ld T;
typedef complex<T> pt;
#define x real()
#define y imag()

// =====================
// Basic vector helpers
// =====================

T sq(pt p) { return p.x * p.x + p.y * p.y; }
T dot(pt a, pt b) { return a.x * b.x + a.y * b.y; }
T cross(pt a, pt b) { return a.x * b.y - a.y * b.x; }
pt perp_ccw(pt p) { return {-p.y, p.x}; }

int sgn(T val) {
    if (val > EPS) return 1;
    if (val < -EPS) return -1;
    return 0;
}

bool samePoint(pt a, pt b) {
    return abs(a - b) <= EPS;
}

// =====================
// Line
// =====================

struct line {
    pt v; 
    T c; // cross(v, p) = c

    line(pt v, T c) : v(v), c(c) {}
    line(T a, T b, T _c) : v(b, -a), c(_c) {}
    line(pt p, pt q) : v(q - p), c(cross(v, p)) {}

    T side(pt p) const { return cross(v, p) - c; }
    T dist(pt p) const { return abs(side(p)) / abs(v); }
    T sqDist(pt p) const { return side(p) * side(p) / sq(v); }

    line translate(pt t) const { return {v, c + cross(v, t)}; }
    line shiftLeft(T d) const { return {v, c + d * abs(v)}; }

    pt proj(pt p) const { return p - perp_ccw(v) * side(p) / sq(v); }
    pt refl(pt p) const { return p - perp_ccw(v) * (T)2.0L * side(p) / sq(v); }
};

bool inter(line l1, line l2, pt &out) {
    T d = cross(l1.v, l2.v);
    if (sgn(d) == 0) return false;
    out = (l2.v * l1.c - l1.v * l2.c) / d;
    return true;
}

// =====================
// Circle
// =====================

// 0 = none, 1 = tangent, 2 = secant
int circleLine(pt c, T r, line l, pair<pt, pt>& out) {
    T dis = r * r - l.sqDist(c);
    if (sgn(dis) < 0) return 0;

    pt p = l.proj(c);
    pt dir = l.v / abs(l.v);
    T h = (sgn(dis) == 0 ? 0 : sqrt(dis));

    out = {p + dir * h, p - dir * h};
    return 1 + sgn(dis);
}

// 0 = none / concentric / identical treated as 0, 1 = tangent, 2 = secant
int circleCircle(pt c1, T r1, pt c2, T r2, pair<pt, pt>& out) {
    pt v = c2 - c1;
    T d = abs(v);

    if (sgn(r1) <= 0 || sgn(r2) <= 0) return 0;
    if (sgn(d) == 0) return 0;
    if (sgn(d - (r1 + r2)) > 0 || sgn(d - abs(r1 - r2)) < 0) return 0;

    T cosTheta = clamp((r1 * r1 + d * d - r2 * r2) / (2.0L * r1 * d), (T)-1.0, (T)1.0);
    T dP = r1 * cosTheta;
    pt p = c1 + (v / d) * dP;

    T h2 = max((T)0.0, r1 * r1 - dP * dP);
    T h = sqrt(h2);
    pt perp = perp_ccw(v) / d;

    out = {p + perp * h, p - perp * h};
    return (sgn(d - (r1 + r2)) == 0 || sgn(d - abs(r1 - r2)) == 0) ? 1 : 2;
}

bool onCircle(pt c, T r, pt p) {
    return sgn(abs(p - c) - r) == 0;
}

bool inDisk(pt c, T r, pt p) {
    return sgn(abs(p - c) - r) <= 0;
}

// Area of overlap between two circles
T circleIntersectionArea(pt c1, T r1, pt c2, T r2) {
    if (sgn(r1) <= 0 || sgn(r2) <= 0) return 0.0L;

    T d = abs(c2 - c1);
    if (sgn(d - (r1 + r2)) >= 0) return 0.0L;
    if (sgn(d - abs(r1 - r2)) <= 0) return PI * min(r1, r2) * min(r1, r2);

    T a1 = clamp((r1 * r1 + d * d - r2 * r2) / (2.0L * r1 * d), (T)-1.0, (T)1.0);
    T a2 = clamp((r2 * r2 + d * d - r1 * r1) / (2.0L * r2 * d), (T)-1.0, (T)1.0);

    T ang1 = 2.0L * acos(a1);
    T ang2 = 2.0L * acos(a2);

    T area1 = 0.5L * r1 * r1 * (ang1 - sin(ang1));
    T area2 = 0.5L * r2 * r2 * (ang2 - sin(ang2));
    return area1 + area2;
}

// Circumcenter of triangle ABC
pt circumCenter(pt a, pt b, pt c) {
    b = b - a, c = c - a;
    T d = 2.0L * cross(b, c);
    if (sgn(d) == 0) return {numeric_limits<T>::quiet_NaN(), numeric_limits<T>::quiet_NaN()};
    pt ans = perp_ccw(b * dot(c, c) - c * dot(b, b)) / d;
    return a + ans;
}

// Tangents between two circles.
// inner = false -> outer tangents
// inner = true  -> inner tangents
// Returns count: 0, 1, or 2.
// Each pair in out = {touch point on first circle, touch point on second circle}
int tangents(pt o1, T r1, pt o2, T r2, bool inner, vector<pair<pt, pt>>& out) {
    if (inner) r2 = -r2;

    pt d = o2 - o1;
    T dr = r1 - r2;
    T d2 = sq(d);
    T h2 = d2 - dr * dr;

    if (sgn(d2) == 0 || sgn(h2) < 0) return 0;

    T h = sqrt(max((T)0.0, h2));

    for (T sign : {-1.0L, 1.0L}) {
        pt v = (d * dr + perp_ccw(d) * h * sign) / d2;
        out.push_back({o1 + v * r1, o2 + v * r2});
        if (sgn(h2) == 0) break;
    }

    return 1 + (sgn(h2) > 0);
}
enum CircleRelation {
  IDENTICAL,          // Same circle
  DISJOINT,           // No intersection
  EXTERNAL_TANGENT,   // One external tangent point
  INTERSECT,          // Two intersection points
  INTERNAL_TANGENT,   // One internal tangent point
  C1_INSIDE_C2,       // Circle 1 completely inside Circle 2
  C2_INSIDE_C1        // Circle 2 completely inside Circle 1
};

CircleRelation circleRelation(pt c1, ld r1, pt c2, ld r2) {
  ld d = abs(c1 - c2);
  // Same center
  if (sgn(d) == 0) {
    if (sgn(r1 - r2) == 0)
      return IDENTICAL;
    return (r1 < r2 ? C1_INSIDE_C2 : C2_INSIDE_C1);
  }
  // Separate
  if (sgn(d - (r1 + r2)) > 0)
    return DISJOINT;
  // External tangent
  if (sgn(d - (r1 + r2)) == 0)
    return EXTERNAL_TANGENT;

  ld diff = fabsl(r1 - r2);
  // One circle completely inside the other
  if (sgn(d - diff) < 0)
    return (r1 < r2 ? C1_INSIDE_C2 : C2_INSIDE_C1);
  // Internal tangent
  if (sgn(d - diff) == 0)
    return INTERNAL_TANGENT;

  // Two intersection points
  return INTERSECT;
}
ld circleSegmentArea(ld r, ld d){
    return r*r*acos(d/r)-d*sqrt(r*r-d*d);
}

// ==========================================
// --- 8. MINIMUM ENCLOSING CIRCLE ---
// ==========================================
 
// Welzl's algorithm for Minimum Enclosing Circle in O(N) expected time.
// Returns a pair: {center_point, radius}
pair<pt, T> welzl(vector<pt> P) { // Passed by value so we can shuffle safely
    if (P.empty()) return {pt(0, 0), 0.0L};
    if (P.size() == 1) return {P[0], 0.0L};
    mt19937 gen(1337);
    shuffle(P.begin(), P.end(), gen);
    pt c = P[0];
    T r = 0;
    for (int i = 1; i < (int)P.size(); i++) {
        if (abs(P[i] - c) > r + EPS) {
            c = P[i];
            r = 0;
            for (int j = 0; j < i; j++) {
                if (abs(P[j] - c) > r + EPS) {
                    c = (P[i] + P[j]) / 2.0L;
                    r = abs(P[i] - c);
                    for (int k = 0; k < j; k++) {
                        if (abs(P[k] - c) > r + EPS) {
                            c = circumCenter(P[i], P[j], P[k]);
                            r = abs(P[k] - c);
                        }
                    }
                }
            }
        }
    }
    return {c, r};
}

// Computes the exact area covered by the union of N circles in O(N^2 log N)
T circleUnionArea(const vector<pair<pt,T>>& circles_input) {
    int n = circles_input.size();
    if (n == 0) return 0.0L;
    vector<pair<pt,T>> circles;
    for (int i = 0; i < n; ++i) {
        if (sgn(circles_input[i].second) > 0) {
            circles.push_back(circles_input[i]);
        }
    }
    n = circles.size();
    vector<bool> covered(n, false);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j) continue;
            T d = abs(circles[i].first - circles[j].first);
            if (sgn(d + circles[i].second - circles[j].second) <= 0) {
                if (sgn(circles[i].second - circles[j].second) == 0) {
                    if (i < j) covered[i] = true;
                } else {
                    covered[i] = true;
                }
            }
        }
    }
    T total_area = 0.0L;
    for (int i = 0; i < n; ++i) {
        if (covered[i]) continue;
        vector<pair<T, T>> intervals;
        for (int j = 0; j < n; ++j) {
            if (i == j || covered[j]) continue;
            T d = abs(circles[i].first - circles[j].first);
            if (sgn(d - (circles[i].second + circles[j].second)) >= 0) continue;
            if (sgn(d - abs(circles[i].second - circles[j].second)) <= 0) continue;
            T phi = arg(circles[j].first - circles[i].first);
            T cosTheta = (circles[i].second * circles[i].second + d * d - circles[j].second * circles[j].second) / (2.0L * circles[i].second * d);
            cosTheta = max((T)-1.0, min((T)1.0, cosTheta));
            T theta = acos(cosTheta);
            T left = phi - theta;
            T right = phi + theta;
            if (left < -PI) {
                intervals.push_back({left + 2.0L * PI, PI});
                intervals.push_back({-PI, right});
            } else if (right > PI) {
                intervals.push_back({left, PI});
                intervals.push_back({-PI, right - 2.0L * PI});
            } else {
                intervals.push_back({left, right});
            }
        }
        sort(intervals.begin(), intervals.end());
        vector<pair<T, T>> merged;
        if (!intervals.empty()) {
            T cur_left = intervals[0].first;
            T cur_right = intervals[0].second;
            for (int k = 1; k < (int)intervals.size(); ++k) {
                if (intervals[k].first <= cur_right + 1e-12) {
                    cur_right = max(cur_right, intervals[k].second);
                } else {
                    merged.push_back({cur_left, cur_right});
                    cur_left = intervals[k].first;
                    cur_right = intervals[k].second;
                }
            }
            merged.push_back({cur_left, cur_right});
        }
        T prev = -PI;
        for (const auto& p : merged) {
            T left = p.first;
            T right = p.second;
            if (left > prev) {
                pt p1 = circles[i].first + pt(circles[i].second * cos(prev), circles[i].second * sin(prev));
                pt p2 = circles[i].first + pt(circles[i].second * cos(left), circles[i].second * sin(left));
                T delta = left - prev;
                total_area += cross(p1, p2) / 2.0L;
                total_area += circles[i].second * circles[i].second * (delta - sin(delta)) / 2.0L;
            }
            prev = right;
        }
        if (prev < PI) {
            pt p1 = circles[i].first + pt(circles[i].second * cos(prev), circles[i].second * sin(prev));
            pt p2 = circles[i].first + pt(circles[i].second * cos(PI), circles[i].second * sin(PI));
            T delta = PI - prev;
            total_area += cross(p1, p2) / 2.0L;
            total_area += circles[i].second * circles[i].second * (delta - sin(delta)) / 2.0L;
        }
    }
    return total_area;
}
// ==========================================
// --- 9. CIRCLE-POLYGON INTERSECTION AREA ---
// ==========================================

// Helper: Computes the signed area of the intersection between a circle
// centered at the origin (0,0) with radius r, and a triangle formed by (0,0), a, and b.
T circleTriangleIntersection(T r, pt a, pt b) {
    if (sgn(cross(a, b)) == 0) return 0.0L;
    auto sector_or_triangle = [&](pt p1, pt p2) {
        bool in1 = sgn(abs(p1) - r) <= 0;
        bool in2 = sgn(abs(p2) - r) <= 0;
        if (in1 && in2) {
            return cross(p1, p2) / 2.0L;
        }
        return r * r * atan2(cross(p1, p2), dot(p1, p2)) / 2.0L;
    };
    line l(a, b);
    pair<pt, pt> out;
    int pts = circleLine(pt(0, 0), r, l, out);
    vector<pt> p = {a};
    if (pts > 0) {
        pt p1 = out.first, p2 = out.second;
        if (dot(p1 - a, b - a) > dot(p2 - a, b - a)) {
            swap(p1, p2);
        }
        if (sgn(dot(p1 - a, b - a)) > 0 && sgn(dot(p1 - b, a - b)) > 0) {
            p.push_back(p1);
        }
        if (pts == 2 && sgn(dot(p2 - a, b - a)) > 0 && sgn(dot(p2 - b, a - b)) > 0) {
            p.push_back(p2);
        }
    }
    p.push_back(b);
    T ans = 0.0L;
    for (size_t i = 0; i + 1 < p.size(); ++i) {
        ans += sector_or_triangle(p[i], p[i+1]);
    }
    return ans;
}
T circlePolygonArea(pt c, T r, const vector<pt>& poly) {
    T area = 0.0L;
    int n = poly.size();
    for (int i = 0; i < n; ++i) {
        pt a = poly[i] - c;
        pt b = poly[(i + 1) % n] - c;
        area += circleTriangleIntersection(r, a, b);
    }
    return abs(area);
}
// Ray-casting algorithm to check if a point is strictly inside a polygon
bool pointInPolygon(pt p, const vector<pt>& poly) {
    bool inside = false;
    int n = poly.size();
    for (int i = 0, j = n - 1; i < n; j = i++) {
        pt a = poly[i];
        pt b = poly[j];
        if (a.y > b.y) swap(a, b);

        // If the point's Y is within the edge's Y range
        if (p.y > a.y && p.y <= b.y) {
            // Find the X coordinate of the intersection
            T x_intersect = a.x + (p.y - a.y) * (b.x - a.x) / (b.y - a.y);
            if (x_intersect > p.x) {
                inside = !inside;
            }
        }
    }
    return inside;
}

// Intersect a circle (Center C, radius R) with a line segment (A to B)
vector<pt> getSegmentCircleIntersections(pt C, T R, pt A, pt B) {
    vector<pt> res;
    pt V = B - A;
    T L = abs(V);
    if (L < 1e-11) return res;

    pt U = V / L;
    pt W = C - A;
    T t_proj = dot(W, U);
    pt P = A + U * t_proj; // Closest point on line to center

    T d = abs(C - P);
    if (d > R + 1e-9) return res; // Line is too far

    T m = 0;
    if (R > d) m = sqrt(max((T)0.0, R * R - d * d));

    T t1 = t_proj - m;
    T t2 = t_proj + m;

    // Check if intersections lie on the segment
    if (t1 >= 0 && t1 <= L) res.push_back(A + U * t1);
    if (m > 1e-9 && t2 >= 0 && t2 <= L) res.push_back(A + U * t2);

    return res;
}


T getPolygonWindowsPerimeter(const vector<pair<pt, T>>& circles_input, const vector<pt>& poly) {
    int n = circles_input.size();
    if (n == 0 || poly.size() < 3) return 0.0L;

    vector<pair<pt, T>> circles;
    for (int i = 0; i < n; ++i) {
        if (sgn(circles_input[i].second) > 0) {
            circles.push_back(circles_input[i]);
        }
    }
    n = circles.size();
    vector<bool> covered(n, false);

    // 1. Mark completely identical/swallowed circles
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j) continue;
            T d = abs(circles[i].first - circles[j].first);
            if (sgn(d + circles[i].second - circles[j].second) <= 0) {
                if (sgn(circles[i].second - circles[j].second) == 0) {
                    if (i < j) covered[i] = true;
                } else {
                    covered[i] = true;
                }
            }
        }
    }

    T total_perimeter = 0.0L;

    for (int i = 0; i < n; ++i) {
        if (covered[i]) continue;

        vector<pair<T, T>> intervals;
        pt center = circles[i].first;
        T R = circles[i].second;

        // Helper function to safely wrap and add intervals
        auto add_interval = [&](T left, T right) {
            if (right - left >= 2.0L * PI - 1e-11) {
                intervals.push_back({-PI, PI});
                return;
            }
            while (left < -PI) { left += 2.0L * PI; right += 2.0L * PI; }
            while (left >= PI) { left -= 2.0L * PI; right -= 2.0L * PI; }

            if (right > PI) {
                intervals.push_back({left, PI});
                intervals.push_back({-PI, right - 2.0L * PI});
            } else {
                intervals.push_back({left, right});
            }
        };

        // 2. Add covered intervals from OTHER CIRCLES
        for (int j = 0; j < n; ++j) {
            if (i == j || covered[j]) continue;
            T d = abs(circles[i].first - circles[j].first);

            if (sgn(d - (R + circles[j].second)) >= 0) continue;
            if (sgn(d - abs(R - circles[j].second)) <= 0) continue;

            T phi = arg(circles[j].first - circles[i].first);
            T cosTheta = (R * R + d * d - circles[j].second * circles[j].second) / (2.0L * R * d);
            cosTheta = clamp(cosTheta, (T)-1.0, (T)1.0);
            T theta = acos(cosTheta);

            add_interval(phi - theta, phi + theta);
        }

        // 3. Add covered intervals from the POLYGON BOUNDARIES
        vector<T> poly_angles = {-PI, PI}; // Start with base circle wrap angles
        int p_sz = poly.size();

        // Find all intersection points with the polygon
        for (int k = 0; k < p_sz; ++k) {
            pt A = poly[k];
            pt B = poly[(k + 1) % p_sz];
            vector<pt> intersections = getSegmentCircleIntersections(center, R, A, B);
            for (const pt& p : intersections) {
                poly_angles.push_back(arg(p - center));
            }
        }
        // Sort the boundary angles to process arcs sequentially
        sort(poly_angles.begin(), poly_angles.end());
        // Check the midpoint of each resulting arc
        for (size_t k = 0; k < poly_angles.size() - 1; ++k) {
            T left = poly_angles[k];
            T right = poly_angles[k+1];
            if (right - left < 1e-9) continue;
            T mid_angle = (left + right) / 2.0L;
            pt test_point = center + polar(R, mid_angle);
            // If the midpoint of this arc is OUTSIDE the polygon, this arc is exposed/covered
            if (!pointInPolygon(test_point, poly)) {
                add_interval(left, right);
            }
        }
        // 4. Sort and Merge all intervals
        sort(intervals.begin(), intervals.end());
        vector<pair<T, T>> merged;
        if (!intervals.empty()) {
            T cur_left = intervals[0].first;
            T cur_right = intervals[0].second;
            for (int k = 1; k < (int)intervals.size(); ++k) {
                if (intervals[k].first <= cur_right + 1e-11) {
                    cur_right = max(cur_right, intervals[k].second);
                } else {
                    merged.push_back({cur_left, cur_right});
                    cur_left = intervals[k].first;
                    cur_right = intervals[k].second;
                }
            }
            merged.push_back({cur_left, cur_right});
        }

        // 5. Calculate uncovered Arc Length (Perimeter)
        T prev = -PI;
        T exposed_angle = 0.0L;
        for (const auto& p : merged) {
            T left = p.first;
            T right = p.second;

            if (left > prev) {
                exposed_angle += (left - prev);
            }
            prev = max(prev, right);
        }
        if (prev < PI) {
            exposed_angle += (PI - prev);
        }

        total_perimeter += exposed_angle * R;
    }

    return total_perimeter;
}
