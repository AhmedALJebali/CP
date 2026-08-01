
`pt = complex<T>` where `T = long double`. `EPS = 1e-9`. `PI = acos(-1.0L)`. Every function below appears **once**; where the same function is redefined identically in multiple files (`Circle.cpp`, `Line.cpp`, `Points.cpp`, `Trigonometry.cpp`, `arc.cpp`, `Convex.cpp`, `Polygon.cpp`, `cmp`, `lineSegmentPolygon.cpp`), that's noted only where behavior actually differs.

---

## 1. Core Primitives

| Function                           | Input         | Output                                                                              |
| ---------------------------------- | ------------- | ----------------------------------------------------------------------------------- |
| `sq(pt p)`                         | vector        | `T` — squared magnitude                                                             |
| `dot(pt v, pt w)`                  | two vectors   | `T` — dot product (`>0` acute, `0` perpendicular, `<0` obtuse)                      |
| `cross(pt v, pt w)`                | two vectors   | `T` — 2D cross product; magnitude = parallelogram area; `>0` if `w` is CCW from `v` |
| `sgn(T val)`                       | value         | `int` — `1`/`-1`/`0`, with `0` for anything within `EPS` of zero                    |
| `samePoint(pt a, pt b)`            | two points    | `bool` — `true` if distance `≤ EPS`                                                 |
| `perp_ccw(pt p)` / `perp_cw(pt p)` | vector        | `pt` — rotated 90° CCW / CW                                                         |
| `orient(pt a, pt b, pt c)`         | three points  | `T` — `cross(b-a, c-a)`; `>0` left of `ab`, `<0` right, `0` collinear               |
| `vect(T r, T th)`                  | radius, angle | `pt` — polar → Cartesian                                                            |
| `isPerp(pt v, pt w)`               | two vectors   | `bool` — `true` if `dot(v,w)==0`                                                    |
| `gt()`                             | reads `cin`   | `pt` — point from stdin                                                             |
| `dist2(pt a, pt b)`                | two points    | `T` — squared distance (`norm(a-b)`)                                                |
| `collinear(pt a, pt b, pt c)`      | three points  | `bool` — same test as `orient == 0`, given its own name in `Convex.cpp`             |

---

## 2. Transformations

|Function|Input|Output|
|---|---|---|
|`translate(pt p, pt v)`|point, vector|`pt` — shifted|
|`scale(pt c, T factor, pt p)`|center, factor, point|`pt` — scaled toward/away from `c`|
|`rot(pt p, pt c, T a)`|point, center, angle (rad)|`pt` — rotated|
|`linearTransfo(pt p, pt q, pt r, pt fp, pt fq)`|old basis `p,q`; point `r`; new basis `fp,fq`|`pt` — `r` remapped to the new system|

---

## 3. Angles

|Function|Input|Output|
|---|---|---|
|`toDegrees(T rad)` / `toRadians(T deg)`|angle|converted|
|`angle(pt v, pt w)`|two vectors|`T` — undirected angle, `[0, π]`|
|`orientedAngle(pt a, pt b, pt c)`|center `a`, rays `ab`, `ac`|`T` — CCW angle from `ab` to `ac`, `[0, 2π)`. _(The `lineSegmentPolygon.cpp` copy adds an explicit early-return of `0` for the collinear-same-direction case before checking orientation sign — same result, just derived more defensively.)_|
|`angleTravelled(pt a, pt b, pt c)`|center `a`, rays `ab`, `ac`|`T` — shortest rotation, `(-π, π]`; `+` CCW, `-` CW|
|`inAngle(pt a, pt b, pt c, pt p)`|angle `∠bac`, test point `p`|`bool` — `true` if ray `ap` strictly between `ab` and `ac`. _(Two equivalent implementations exist in the repo — an XOR-based one and a normalize-then-compare one — same result.)_|

---

## 4. Line

**Constructors** (3 overloads): `line(pt v, T c)` direction+constant · `line(T a, T b, T c)` standard form `ax+by=c` · `line(pt p, pt q)` through two points.

|Method|Input|Output|
|---|---|---|
|`side(pt p)`|test point|`T` — `>0` left, `0` on, `<0` right|
|`dist(pt p)`|test point|`T` — perpendicular distance|
|`sqDist(pt p)`|test point|`T` — squared perpendicular distance|
|`proj(pt p)`|test point|`pt` — perpendicular projection onto the line|
|`refl(pt p)`|test point|`pt` — reflection across the line|
|`translate(pt t)`|translation vector|`line` — shifted by `t`|
|`shiftLeft(T d)`|distance|`line` — shifted left by `d`|
|`prepThrough(pt p)`|reference point|`line` — perpendicular through `p`|
|`cmpProj(pt p, pt q)`|two points|`bool` — `true` if `p`'s projection precedes `q`'s along the line|

> A **minimal variant** of this struct (used internally by the Arcs module, §12) only implements `side`, `sqDist`, `proj`, and is only constructible from two points — same semantics, fewer methods.

|Free function|Input|Output|
|---|---|---|
|`inter(line l1, line l2, pt &out)`|two lines, out-ref|`bool` — `true` if they meet at one point (writes to `out`), `false` if parallel|
|`bisector(line l1, line l2, bool interior)`|two non-parallel lines, interior/exterior flag|`line` — angle bisector|
|`shortestPathPointOnLine(pt a, pt b, line l)`|points `a,b`, mirror line `l`|`pt` — point on `l` minimizing `\|AP\|+\|PB\|` (Heron's problem)|
|`getPerpBisector(pt a, pt b)`|two points|`line` — perpendicular bisector of segment `ab`|
|`struct LineKey`|`(T a, T b, T c)` or a `line`|Normalized `(a,b,c)` so equivalent lines compare/hash identically — usable as a `map`/`set` key (`<` and `==` defined)|

---

## 5. Segments

|Function|Input|Output|
|---|---|---|
|`onSegment(pt a, pt b, pt p)`|segment, test point|`bool` — `true` if `p` lies exactly on segment `ab`|
|`properInter(pt a, pt b, pt c, pt d, pt &out)`|two segments, out-ref|`bool` — `true` if they intersect strictly in both interiors|
|`segInter(pt a, pt b, pt c, pt d)`|two segments|`vector<pt>` — 0/1/2 intersection points (2 only for overlapping collinear segments)|
|`closestPointOnSegment(pt a, pt b, pt p)`|segment, point|`pt` — closest point on `ab` to `p`|
|`segPoint(pt a, pt b, pt p)`|segment, point|`T` — distance from `p` to segment|
|`segSeg(pt a, pt b, pt c, pt d)`|two segments|`T` — shortest distance between them (`0` if intersecting)|
|`doIntersect(pt a, pt b, pt c, pt d)`|two segments|`bool` — fast yes/no (no intersection point computed)|

**`inDisk` has two unrelated meanings in this repo — don't confuse them:**

- `inDisk(pt c, T r, pt p)` — circle given by **center + radius**: `true` if `p` is inside/on it. _(Used in `Circle.cpp`.)_
- `inDisk(pt a, pt b, pt p)` — circle whose **diameter's endpoints** are `a, b`: `true` if `p` is inside/on it. _(Used everywhere else — `Line.cpp`, `Points.cpp`, `Polygon.cpp`.)_

---

## 6. Rays

|Function|Input|Output|
|---|---|---|
|`onRay(pt a, pt b, pt p)`|origin `a`, direction point `b`, test point|`bool` — `true` if `p` is on ray `a→b`|
|`rayLineInter(pt a, pt b, line l)`|ray, line|`bool` — intersects?|
|`rayLine(pt a, pt b, line l)`|ray, line|`T` — shortest distance|
|`rayInter(pt a, pt b, pt c, pt d, pt &p)`|two rays, out-ref|`bool` — writes common point if they meet|
|`rayDist(pt a, pt b, pt p)`|ray, point|`T` — distance|
|`rayRayDist(pt a1, pt b1, pt a2, pt b2)`|two rays|`T` — shortest distance|

> One copy of these five (in `lineSegmentPolygon.cpp`) is implemented using `line::cmpProj` instead of `onRay`. It agrees for ordinary cases, but unlike the primary implementation it does **not** explicitly handle collinear-overlapping rays — worth checking if that edge case matters for your use.

---

## 7. Polygon Basics (Area, Perimeter, Convexity)

|Function|Input|Output|
|---|---|---|
|`signedAreaPolygon(vector<pt>& p)`|polygon vertices|`T` — shoelace signed area (`+` if CCW)|
|`areaPolygon(vector<pt>& p)`|polygon vertices|`T` — absolute area|
|`perimeterPolygon(vector<pt>& p)`|polygon vertices|`T` — sum of edge lengths|
|`isCCW(vector<pt>& p)`|polygon vertices|`bool` — `true` if signed area `> 0`|
|`isConvex(vector<pt>& p, bool strict=false)`|polygon vertices, strict flag|`bool` — `true` if all turns share one sign (collinear turns allowed unless `strict`)|
|`normalizeCCW(vector<pt>& p)`|polygon vertices|`void` — reverses in-place if CW, so result is CCW|

---

## 8. Point-in-Polygon & Winding

|Function|Input|Output|
|---|---|---|
|`above(pt a, pt p)`|reference, point|`bool` — `p.y >= a.y` (helper)|
|`crossesRay(pt a, pt p, pt q)`|test point, edge `p→q`|`bool` — helper for `inPolygon`|
|`inPolygon(vector<pt>& p, pt a, bool strict=true)`|polygon (any shape, non-self-intersecting), point, strict flag|`bool` — ray-casting test; boundary counts as inside unless `strict`|
|`windingNumber(vector<pt>& p, pt a)`|polygon, point|`int` — `0` outside, nonzero inside, `-1` on boundary; more robust than `inPolygon` for tricky/self-intersecting shapes|
|`pointInConvexPolygon(vector<pt>& poly, pt q, bool strict=true)`|**convex** polygon (CW or CCW), point, strict flag|`bool` — `O(log n)` binary-search test, faster than the above for convex-only cases|

---

## 9. Polygon Centroid & Lattice Points

|Function|Input|Output|
|---|---|---|
|`polygonCentroid(vector<pt>& p)`|polygon vertices|`pt` — area-weighted centroid. _(A second copy, named `centroid`, is identical except it returns `{0,0}` instead of `NaN` for a degenerate polygon.)_|
|`segmentBoundaryPoints(pt a, pt b)`|segment endpoints (integer-valued coords)|`int` — lattice points on `ab`, excluding `a` itself (via `gcd`)|
|`polygonBoundaryPoints(vector<pt>& p)`|polygon (integer-valued coords)|`int` — total boundary lattice points `B`|
|`polygonDoubleAreaExact(vector<pt>& p)`|polygon (integer-valued coords)|`int` — exact `2×area`, avoiding float error|
|`polygonInteriorPoints(vector<pt>& p)`|polygon (integer-valued coords)|`int` — interior lattice points `I`, via Pick's Theorem: `I = (2A - B + 2) / 2`|

---

## 10. Polygon Cut

|Function|Input|Output|
|---|---|---|
|`polygonCut(vector<pt>& p, pt a, pt b)`|polygon, directed line `a→b`|`vector<pt>` — polygon clipped to keep only the side left of (and on) the line|

---

## 11. Circle

|Function|Input|Output|
|---|---|---|
|`onCircle(pt c, T r, pt p)`|circle, point|`bool` — exactly on boundary|
|`circleLine(pt c, T r, line l, pair<pt,pt>& out)`|circle, line, out-ref|`int` — `0` none, `1` tangent, `2` secant; writes point(s)|
|`circleCircle(pt c1, T r1, pt c2, T r2, pair<pt,pt>& out)`|two circles, out-ref|`int` — `0` none/identical/concentric, `1` tangent, `2` secant|
|`circleIntersectionArea(pt c1, T r1, pt c2, T r2)`|two circles|`T` — overlap area|
|`circleRelation(pt c1, ld r1, pt c2, ld r2)`|two circles|`CircleRelation` enum: `IDENTICAL, DISJOINT, EXTERNAL_TANGENT, INTERSECT, INTERNAL_TANGENT, C1_INSIDE_C2, C2_INSIDE_C1`|
|`circleSegmentArea(ld r, ld d)`|radius, center-to-chord distance|`ld` — area of the smaller circular segment|
|`tangents(pt o1, T r1, pt o2, T r2, bool inner, vector<pair<pt,pt>>& out)`|two circles, inner/outer flag, out-vec|`int` — 0/1/2 tangent lines; appends touch-point pairs|
|`circumCenter(pt a, pt b, pt c)`|three points|`pt` — center of circle through all three (`NaN` if collinear)|

---

## 12. Circular Arcs

_An arc is always the CCW curve from `a` to `b` on circle `(c, r)`._

|Function|Input|Output|
|---|---|---|
|`arcLength(pt c, T r, pt a, pt b)`|circle, arc endpoints|`T` — arc length|
|`arcSectorArea(pt c, T r, pt a, pt b)`|circle, arc endpoints|`T` — pie-sector area|
|`arcSegmentArea(pt c, T r, pt a, pt b)`|circle, arc endpoints|`T` — area between arc and chord|
|`isPointOnArc(pt c, T r, pt a, pt b, pt p)`|arc, test point|`bool`|
|`arcLineInter(pt c, T r, pt a, pt b, line l)`|arc, line|`vector<pt>` — 0–2 points|
|`arcArcInter(pt c1, T r1, pt a1, pt b1, pt c2, T r2, pt a2, pt b2)`|two arcs|`vector<pt>` — 0–2 shared points|
|`distPointArc(pt c, T r, pt a, pt b, pt p)`|arc, point|`T` — shortest distance|
|`arcBoundingBox(pt c, T r, pt a, pt b)`|arc|`pair<pt,pt>` — `{bottom_left, top_right}` AABB|

---

## 13. Triangles

|Function|Input|Output|
|---|---|---|
|`areaTriangle(pt a, pt b, pt c)`|vertices|`T` — area|
|`areaTriangle(T a, T b, T c)`|side lengths|`T` — Heron's-formula area (`0` if invalid)|
|`isValidTriangle(T a, T b, T c)`|side lengths|`bool` — satisfies triangle inequality. _(A duplicate named `isValidTriangleLD` exists with identical logic, just `ld`-typed.)_|
|`isNonDegenerateTriangle(pt a, pt b, pt c)`|vertices|`bool` — nonzero area|
|`perimeterTriangle(pt a, pt b, pt c)`|vertices|`T` — perimeter|
|`inCenter(pt a, pt b, pt c)`|vertices|`pt` — incenter (`NaN` if degenerate)|
|`orthocenter(pt a, pt b, pt c)`|vertices|`pt` — altitude intersection (`NaN` if degenerate)|
|`inRadius(pt a, pt b, pt c)`|vertices|`T` — inscribed circle radius|
|`circumRadius(pt a, pt b, pt c)`|vertices|`T` — circumscribed circle radius|

---

## 14. Regular Polygons & Pyramids

|Function|Input|Output|
|---|---|---|
|`getRegularPolygonArea(int n, T L)`|sides, side length|`T` — area of regular `n`-gon|
|`getCircumradiusR(int n, T L)`|sides, side length|`T` — circumradius|
|`getPyramidHeight(T lateral_edge, T R)`|slant edge, base circumradius|`T` — apex-to-base height|
|`getPyramidVolume(T A, T H)`|base area, height|`T` — volume|
|`getRegularPyramidVolume(int n, T s)`|sides, uniform edge length|`T` — volume of the fully regular pyramid|

---

## 15. Convex Hull & Convex-Only Algorithms

|Function|Input|Output|
|---|---|---|
|`reorderConvex(vector<pt>& p)`|convex polygon vertices|`void` — rotates in-place so the lowest-then-leftmost point is `p[0]`|
|`cw(pt a, pt b, pt c, bool col)`|three points, collinear-inclusive flag|`bool` — clockwise turn (or non-left, if `col`)|
|`convex_hull(vector<pt>& a, bool include_collinear=true)`|points (mutated in-place)|`void` — replaces `a` with its CCW convex hull|
|`minkowski(vector<pt> P, vector<pt> Q)`|two convex polygons|`vector<pt>` — Minkowski sum, `O(N+M)`|
|`convexDiameter(vector<pt>& poly)` _(a.k.a. `polygonDiameter`, which returns just the distance)_|convex polygon|`pair<pair<pt,pt>, T>` — farthest pair + distance, rotating calipers, `O(N)`|
|`polygonWidth(vector<pt>& p)`|convex polygon|`T` — min distance between two parallel supporting lines|
|`minimumEnclosingRectanglePerimeter(vector<pt>& p)`|convex polygon|`T` — perimeter of the min-area enclosing rectangle|
|`maximum_dist_from_polygon_to_polygon_brute(vector<pt>& u, vector<pt>& v)`|two point sets|`T` — brute-force `O(N·M)` max pairwise distance (prefer `minkowski(u, -v)` for large inputs)|

---

## 16. Half-Plane Intersection

|Item|Input|Output|
|---|---|---|
|`struct Halfplane(pt a, pt b)`|directed edge `a→b` (left side = "in")|Stores `p`, direction `pq`, polar `angle`; `.out(pt r)` → `bool` strictly outside; sortable by angle; friend `inter(Halfplane, Halfplane)` → `pt` intersection|
|`hp_intersect(vector<Halfplane> H)`|list of half-planes|`vector<pt>` — resulting convex polygon's vertices (bounded internally by a large box); empty if degenerate (`<3` vertices)|

---

## 17. Radial Sorting

|Function|Input|Output|
|---|---|---|
|`sortClockwise(vector<pt>& pts, pt center)`|points, center|`void` — sorts in-place, CW|
|`sortCounterClockwise(vector<pt>& pts, pt center)`|points, center|`void` — sorts in-place, CCW|

---

## 18. Closest Pair of Points — `O(N log N)`

|Function|Input|Output|
|---|---|---|
|`cmpX(const pt& a, const pt& b)`|two points|`bool` — order by X, then Y|
|`cmpY(const pt& a, const pt& b)`|two points|`bool` — order by Y, then X|
|`closestPair(vector<pt> pts)`|points (by value)|`pair<pair<pt,pt>, T>` — closest pair + distance|

---

## 19. Minimum Enclosing Circle

|Function|Input|Output|
|---|---|---|
|`welzl(vector<pt> P)`|points (by value, shuffled internally)|`pair<pt, T>` — center + radius of the smallest enclosing circle, expected `O(N)`|

> Depends on `circumCenter` (§11).

---

## 20. 1D Segment Union (Sweep Line)

|Function|Input|Output|
|---|---|---|
|`segmentUnionLength(vector<pair<T,T>>& segments)`|list of 1D intervals|`T` — total length of their union|

---

## 21. `O(N log N)` Segment Intersection (Bentley–Ottmann)

|Function|Input|Output|
|---|---|---|
|`anyIntersection(vector<pair<pt,pt>> segs)`|list of segments|`pair<int,int>` — indices of any two intersecting segments; `{-1,-1}` if none|

> Uses `doIntersect` (§5) internally. `LineKey` (§4) pairs naturally with this for map/set-based line dedup during a sweep.

---

## 22. Point Comparator

|Item|Input|Output|
|---|---|---|
|`struct cmp`|two `pt`s via `operator()`|`bool` — strict weak order (x then y, via `sgn`) — for `set<pt, cmp>` / `map<pt, int, cmp>`|

---

## File Map

For reference, which source file each section's code actually lives in (files are meant to be dropped in standalone, so most low-level helpers like `sq`/`dot`/`cross`/`sgn` are copy-pasted into each one):

|File|Covers sections|
|---|---|
|`Circle.cpp`|1 (partial), 11, 19 (`circumCenter`, `welzl`)|
|`Line.cpp`|1 (partial), 4, 5, 6, 20, 21|
|`Points.cpp`|1, 2, 3, 17, 18|
|`Trigonometry.cpp`|1 (partial), 13, 14|
|`arc.cpp`|12 (+ minimal deps for it)|
|`Convex.cpp`|7 (own copy), 15, 16|
|`Polygon.cpp`|7 (own copy), 8, 9, 10, 15 (`polygonDiameter`/`polygonWidth`/`minimumEnclosingRectanglePerimeter`), 17 (own copy)|
|`cmp`|22|
|`lineSegmentPolygon.cpp`|A standalone merge of nearly all sections above into one file — see the inline variant notes in §3 and §6 for the handful of spots where its implementation differs slightly from the others.|