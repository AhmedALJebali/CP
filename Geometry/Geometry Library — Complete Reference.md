# Geometry Library — Complete Reference (v2)

**What changed in this revision:** the old repo spread this code across 9 files (`Circle.cpp`, `Line.cpp`, `Points.cpp`, `Trigonometry.cpp`, `arc.cpp`, `Convex.cpp`, `Polygon.cpp`, `cmp`, `lineSegmentPolygon.cpp`), each re-declaring the same primitives (`sq`, `dot`, `cross`, `sgn`, `line`, ...) so that any file could be dropped in standalone. That worked for single-file submission, but it also meant:

- You couldn't `#include` two of them together (redefinition errors).
- Several real functions were never documented at all — `circleUnionArea`, `getPolygonWindowsPerimeter`, `circlePolygonArea`, `areaUnionOfRectangles`, the **dynamic convex hull** (`DynamicHull`, O(log n) insert/erase/point-query with O(1) area), `tangentsFromExteriorPoint`, and `lineConvexPolygonIntersection` all existed in the code but weren't in the old README.
- Two same-named-but-different `inDisk` overloads (circle-by-center-radius vs. disk-with-diameter-ab) were a landmine waiting to bite whoever grabbed the wrong file.

**`geometry.hpp`** merges everything into one dependency-ordered header — copy-paste the whole file (or `#include` it if you have multi-file judge support) and every section below is available together, once, with no redefinitions. Every function that existed anywhere in the old repo is preserved (bugs and all edge-case handling intact); nothing was silently dropped.

`pt = complex<T>` where `T = long double`. `EPS = 1e-9`. `PI = acos(-1.0L)`.

---

## 1. Core Primitives

| Function | Input | Output |
|---|---|---|
| `sq(pt p)` | vector | `T` — squared magnitude |
| `dist2(pt a, pt b)` | two points | `T` — squared distance |
| `dot(pt v, pt w)` | two vectors | `T` — dot product |
| `cross(pt v, pt w)` | two vectors | `T` — 2D cross product |
| `sgn(T val)` | value | `int` — `1`/`-1`/`0`, EPS-tolerant |
| `samePoint(pt a, pt b)` | two points | `bool` |
| `perp_ccw` / `perp_cw` | vector | `pt` — rotated ±90° |
| `orient(pt a, pt b, pt c)` | three points | `T` — `cross(b-a,c-a)` |
| `collinear(pt a, pt b, pt c)` | three points | `bool` |
| `vect(T r, T th)` | radius, angle | `pt` — polar → Cartesian |
| `isPerp(pt v, pt w)` | two vectors | `bool` |
| `gt()` | reads `cin` | `pt` |
| `struct cmp` | `operator()(pt,pt)` | strict weak order (x then y) for `set<pt,cmp>` / `map<pt,V,cmp>` |

---

## 2. Transformations
`translate`, `scale`, `rot`, `linearTransfo` — unchanged from before.

## 3. Angles
`toDegrees`/`toRadians`, `angle` (undirected, `[0,π]`), `orientedAngle` (CCW, `[0,2π)`), `angleTravelled` (shortest signed rotation, `(-π,π]`), `inAngle` (strict ray-betweenness).

---

## 4. Line

Constructors: `line(pt v, T c)` · `line(T a, T b, T c)` (`ax+by=c`) · `line(pt p, pt q)`.

| Method | Output |
|---|---|
| `side/dist/sqDist(pt p)` | signed side / perpendicular distance / squared distance |
| `proj/refl(pt p)` | projection / reflection |
| `translate(pt)/shiftLeft(T)` | shifted line |
| `prepThrough(pt p)` | perpendicular through `p` |
| `cmpProj(pt p, pt q)` | ordering along the line |

Free functions: `inter(line,line,pt&)`, `bisector`, `shortestPathPointOnLine` (Heron's problem), `getPerpBisector`, `struct LineKey` (normalized `(a,b,c)` map/set key).

---

## 5. Segments

`onSegment`, `properInter`, `segInter` (0/1/2 pts), `closestPointOnSegment`, `segPoint`, `segSeg`, `doIntersect`.

**Renamed for safety:** the old `inDisk(pt,pt,pt)` (disk whose *diameter* is the two points) is now **`inDiskOnDiameter`**, kept distinct in name from **`inDiskCR(pt c, T r, pt p)`** (circle given by center+radius, §11). They used to share the name `inDisk` across different files with different meanings — an easy copy-paste bug. Now there is exactly one of each, permanently distinguishable.

**`closestPointOnPolygon(poly, p)`** *(NEW)* — closest point on a polygon's boundary to `p` (just wasn't factored out as its own function before, despite being a one-liner over `closestPointOnSegment`).

---

## 6. Rays
`onRay`, `rayLineInter`, `rayLine`, `rayInter`, `rayDist`, `rayRayDist` — the old repo had two independently-written copies of these (one using `onRay`, one using `line::cmpProj`) with a documented behavioral gap on collinear-overlapping rays. This revision keeps only the **more defensive `onRay`-based version**, so that edge case is now handled everywhere it's used.

---

## 7. Polygon Basics
`signedAreaPolygon`, `areaPolygon`, `perimeterPolygon`, `isCCW`, `isConvex(strict=false)`, `normalizeCCW`.

---

## 8. Point-in-Polygon & Winding

`inPolygon(poly, pt, strict=true)`, `windingNumber` (robust for self-intersecting shapes), `pointInConvexPolygon` (`O(log n)`, works for either winding order).

**`pointInTriangleBarycentric(a,b,c,p,u,v,w)`** *(NEW)* — returns `bool` inside-or-on, and writes barycentric weights `u,v,w` (with `p = u·a + v·b + w·c`, `u+v+w=1`). Useful whenever you need more than yes/no — e.g. interpolating a value defined at the three vertices.

**`pointInPolygonRayCast(pt, poly)`** — the old `Circle.cpp` had its own independent ray-casting point-in-polygon (`pointInPolygon`), separate from `Polygon.cpp`'s `inPolygon`. Both are kept (renamed to avoid clashing) since some of your other code (`circlePolygonArea`'s helpers) depends on the exact semantics of this one; for new code, prefer `inPolygon`.

---

## 9. Polygon Centroid & Lattice Points
`polygonCentroid` (now consistently returns `{0,0}` for a degenerate polygon, matching the old `centroid` variant — the `NaN`-returning copy was dropped as strictly worse), `segmentBoundaryPoints`, `polygonBoundaryPoints`, `polygonDoubleAreaExact` (now `long long`, was silently `int` before — this overflowed on coordinates above ~±23000), `polygonInteriorPoints` (Pick's theorem).

---

## 10. Polygon Cut & Convex ∩ Convex

`polygonCut(poly, a, b)` — clip to the left of directed line `a→b`.

**`convexPolygonIntersection(P, Q)`** *(NEW)* — intersection of two convex polygons via repeated `polygonCut`, `O(N·M)`. The old repo could clip a polygon against one line, but had no convex-vs-convex intersection at all — a genuinely missing, commonly-needed primitive (visibility regions, common-area queries, etc.).

---

## 11. Circle

`onCircle`, `inDiskCR` (see §5 note), `circleLine`, `segCircleInter` (renamed from `getIntersections` — circle ∩ **segment**, not circle ∩ line), `circleCircle`, `circleIntersectionArea`, `circleSegmentArea`, `circumCenter`, `tangents` (circle-circle), `circleRelation` (enum classification), `welzl` (min enclosing circle).

**`tangentPointsFromPoint(p, c, r, out)`** *(NEW)* — tangent lines from an **external point** to a circle. This existed only implicitly as a degenerate case of circle-circle `tangents` (shrinking one circle to radius 0); it's common enough (e.g. visibility from a point, laser/line-of-sight problems) to deserve its own direct, named entry point rather than making every caller rediscover the substitution.

**Previously undocumented, now written up:**
- **`circleUnionArea(vector<pair<pt,T>>)`** — exact area of the union of N circles, `O(N² log N)`, via per-circle arc coverage + shoelace.
- **`getPolygonWindowsPerimeter(circles, poly)`** — total *exposed* perimeter of a union of circles, clipped to a polygon window (only the arcs visible through the window count). Reuses `segCircleInter` for the polygon-edge/circle intersections.
- **`circleTriangleIntersection` / `circlePolygonArea(c, r, poly)`** — exact area of intersection between a circle and an arbitrary simple polygon, via signed sector/triangle decomposition per edge.

---

## 12. Circular Arcs
_Arc = CCW curve from `a` to `b` on circle `(c,r)`._ `arcLength`, `arcSectorArea`, `arcSegmentArea`, `isPointOnArc`, `arcLineInter`, `arcArcInter`, `distPointArc`, `arcBoundingBox` — unchanged, deduplicated.

---

## 13. Triangles
`areaTriangle` (both overloads — vertices, and Heron's-formula-by-side-lengths), `isValidTriangle` (the duplicate `isValidTriangleLD` was dropped — it was byte-for-byte identical logic, just `ld`-typed, and `T` already **is** `ld`), `isNonDegenerateTriangle`, `perimeterTriangle`, `inCenter`, `orthocenter`, `inRadius`, `circumRadius`.

---

## 14. Regular Polygons & Pyramids
`getRegularPolygonArea`, `getCircumradiusR`, `getPyramidHeight`, `getPyramidVolume`, `getRegularPyramidVolume` — unchanged.

---

## 15. Convex Hull & Convex-Only Algorithms

`reorderConvex`, `cw`, `convex_hull` (Andrew's monotone chain / Graham-scan style), `minkowski` (`O(N+M)`), `convexDiameter` / `polygonDiameter` (rotating calipers, `O(N)`), `polygonWidth`, `minimumEnclosingRectanglePerimeter`, `maximum_dist_from_polygon_to_polygon_brute`.

**Previously undocumented, now written up:**
- **`tangentsFromExteriorPoint(poly, q)`** — `O(log n)` binary search for the two tangent-line vertices from an external point to a convex polygon.
- **`lineConvexPolygonIntersection(poly, A, B)`** — `O(log n)` intersection of an infinite directed line with a strictly-convex CCW polygon (0/1/2 points).

---

## 16. Half-Plane Intersection
`struct Halfplane`, `hp_intersect` — unchanged.

## 17. Radial Sorting
`sortClockwise`, `sortCounterClockwise` — unchanged.

## 18. Closest Pair of Points — `O(N log N)`
`cmpX`, `cmpY`, `closestPair` — unchanged.

## 19. 1D Segment Union (Sweep Line)
`segmentUnionLength` — unchanged.

## 20. `O(N log N)` Segment Intersection (Bentley–Ottmann)
`anyIntersection` — unchanged.

---

## 21. Area of Union of Axis-Aligned Rectangles *(previously undocumented)*

`areaUnionOfRectangles(vector<array<T,4>> rects)` — classic sweep-line + segment-tree-with-length, `O(N log N)`. `rects[i] = {x1,y1,x2,y2}`.

---

## 22. Dynamic Convex Hull *(previously undocumented — this is the single most powerful thing in the old repo that nobody had written up)*

`struct DynamicHull` — maintains the convex hull of an **online, insert/erase-able** integer point set:

| Method | Complexity | Output |
|---|---|---|
| `insert(pt p)` / `erase(pt p)` | `O(log n)` amortized | maintain hull |
| `is_inside(pt p)` | `O(log n)` | `0` outside / `1` strictly inside / `2` on boundary |
| `get_area()` | `O(1)` | current hull area, tracked incrementally |
| `get_hull()` | `O(hull size)` | current hull vertices, CCW |

Internally built from two `upper_hull` monotone chains (upper envelope of the points, and upper envelope of the negated points = lower envelope), each a `set<pt,cmp>` with incrementally-maintained signed area. Requires integer-valued coordinates (uses `llround` for exact cross products — this is what lets area tracking stay exact instead of drifting under repeated floating updates).

---

## 23. Point-in-Polygon, Alternate Form
`pointInPolygonRayCast` — see §8 note.

## 24. *(reserved — numbering placeholder so this doc's sections line up 1:1 with `geometry.hpp`'s section comments; no unique functions live here)*

---

## 25. 3D Primitives *(brand new — the old repo was 2D-only)*

| Item | Output |
|---|---|
| `struct pt3 { T X,Y,Z; }` | 3D point/vector (fields capitalized to avoid clashing with the `#define x real()` macro used for 2D `pt`) |
| `dot3`, `cross3`, `norm3`, `abs3`, `dist3` | standard 3D vector ops |
| `struct plane3(pt3 a, pt3 b, pt3 c)` or `(pt3 n, T d)` | plane through 3 points, or by normal+offset (`n·p = d`) |
| `plane3::side(pt3 p)` | signed distance numerator |
| `plane3::distTo(pt3 p)` | perpendicular distance |
| `plane3::project(pt3 p)` | projection onto the plane |
| `linePlaneInter(pt3 a, pt3 dir, plane3, pt3&)` | `bool` — line/plane intersection point |

This is intentionally minimal (just enough for "does this problem actually need 3D" contest problems — plane-through-3-points, point-plane distance, line-plane intersection) rather than a full 3D convex hull / Delaunay stack, which would roughly double the file size for techniques that come up far less often in ICPC-style sets than the 2D material above.

---

## File Map

Everything now lives in **one file**: `geometry.hpp`. The section numbers above match the `// === N. ... ===` comments inside it directly, so you can jump to the right block instead of hunting across 9 files. Drop the whole header into your template folder; `#include "geometry.hpp"` once and every section is available with zero redefinition conflicts.

**Verified:** compiled clean under `g++ -std=c++17 -O2 -Wall` with a smoke test covering convex hull, tangent-from-point, barycentric point-in-triangle, convex-convex clipping, the dynamic hull, and the new 3D plane distance — all producing correct numeric results.
