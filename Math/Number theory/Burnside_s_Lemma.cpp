// ==========================================
// 0. Core Math Helpers
// ==========================================
int power(int base, int exp) {
    int res = 1; base %= MOD; if (base < 0) base += MOD;
    while (exp > 0) {
        if (exp & 1) res = (res * base) % MOD;
        base = (base * base) % MOD; exp >>= 1;
    }
    return res;
}
int modInverse(int n) { n %= MOD; if (n < 0) n += MOD; return power(n, MOD - 2); }

int get_phi(int n) {
    int res = n;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            while (n % i == 0) n /= i;
            res -= res / i;
        }
    }
    if (n > 1) res -= res / n;
    return res;
}

// Mobius function mu(n): 0 if n has a squared prime factor, else (-1)^k
// for k distinct prime factors. Needed for aperiodic-necklace / Lyndon
// word counting.
int get_mobius(int n) {
    int res = 1;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            n /= i;
            if (n % i == 0) return 0; // squared factor
            res = -res;
        }
    }
    if (n > 1) res = -res;
    return res;
}

int cp_gcd(int a, int b) { return b == 0 ? a : cp_gcd(b, a % b); }

// nCr mod p (small n, r) via direct product -- used by exact-count Polya.
int nCr_mod(int n, int r) {
    if (r < 0 || r > n) return 0;
    r = min(r, n - r);
    int num = 1, den = 1;
    for (int i = 0; i < r; i++) {
        num = (num * ((n - i) % MOD)) % MOD;
        den = (den * ((i + 1) % MOD)) % MOD;
    }
    return (num * modInverse(den)) % MOD;
}



// ==========================================
// 1D: Cycles and Rings (rotations / dihedral)
// ==========================================
// Number of necklaces of length N using C colors, up to ROTATION only.
int solve_1D_Necklace(int N, int C) {
    int ans = 0;
    for (int d = 1; d * d <= N; d++) {
        if (N % d == 0) {
            ans = (ans + power(C, d) * (get_phi(N / d) % MOD)) % MOD;
            int d2 = N / d;
            if (d2 != d) {
                ans = (ans + power(C, d2) * (get_phi(N / d2) % MOD)) % MOD;
            }
        }
    }
    return (ans * modInverse(N)) % MOD;
}
// Number of bracelets of length N using C colors, up to ROTATION + REFLECTION
int solve_1D_Bracelet(int N, int C) {
    int ans = (solve_1D_Necklace(N, C) * (N % MOD)) % MOD; // sum over rotations
    int MOD_phi = MOD - 1;
    int reflection_ans = 0;

    if (N % 2 == 1) {
        int E = ((N + 1) / 2) % MOD_phi;
        reflection_ans = (N % MOD) * power(C, E) % MOD;
    } else {
        int E1 = ((N + 2) / 2) % MOD_phi;
        int E2 = (N / 2) % MOD_phi;
        int half_N = (N / 2) % MOD;
        reflection_ans = (half_N * power(C, E1)) % MOD;
        reflection_ans = (reflection_ans + half_N * power(C, E2)) % MOD;
    }
    ans = (ans + reflection_ans) % MOD;
    return (ans * modInverse((2 * (N % MOD)) % MOD)) % MOD;
}
// Number of BINARY necklaces (C=2)
int count_binary_necklaces(int N) { return solve_1D_Necklace(N, 2); }
// it counts only necklaces whose minimal period is exactly N
int count_aperiodic_necklaces(int N, int C) {
    int ans = 0;
    for (int d = 1; d * d <= N; d++) {
        if (N % d == 0) {
            int mu_d = get_mobius(d);
            if (mu_d != 0) ans = (ans + mu_d * power(C, N / d) % MOD + MOD) % MOD;
            int d2 = N / d;
            if (d2 != d) {
                int mu_d2 = get_mobius(d2);
                if (mu_d2 != 0) ans = (ans + mu_d2 * power(C, N / d2) % MOD + MOD) % MOD;
            }
        }
    }
    return (ans * modInverse(N)) % MOD;
}
// Returns the number of distinct N-bead binary necklaces that contain exactly k beads of the first color
int count_binary_necklaces_exact_k(int N, int k) {
    int g = cp_gcd(N, k);
    int ans = 0;
    for (int d = 1; d * d <= g; d++) {
        if (g % d == 0) {
            ans = (ans + (get_phi(d) % MOD) * nCr_mod(N / d, k / d)) % MOD;
            int d2 = g / d;
            if (d2 != d) ans = (ans + (get_phi(d2) % MOD) * nCr_mod(N / d2, k / d2)) % MOD;
        }
    }
    return (ans * modInverse(N)) % MOD;
}
// Returns the number of valid ways to color an N-vertex circular ring using C colors such that no two adjacent vertices share the same color
int count_cycle_proper_colorings(int N, int C) {
    int term = power(C - 1, N);
    int sign = (N % 2 == 0) ? 1 : -1;
    int ans = (term + sign * ((C - 1) % MOD) % MOD + MOD) % MOD;
    return ans;
}
// Returns the number of valid ways to color a straight path (line) using C colors such that no two adjacent vertices share the same color
int count_path_proper_colorings(int N, int C) {
    if (N <= 0) return 1;
    return (C % MOD) * power(C - 1, N - 1) % MOD;
}
// Returns the 0-based starting index of the lexicographically smallest cyclic rotation of the given string
int least_rotation(const string& s) {
    string t = s + s;
    int n = t.size(), i = 0, j = 1, k = 0;
    while (i < s.size() && j < s.size() && k < n) {
        char a = t[i + k], b = t[j + k];
        if (a == b) { k++; continue; }
        if (a > b) i += k + 1; else j += k + 1;
        if (i == j) j++;
        k = 0;
    }
    return min(i, j);
}
// Returns the unique sequence of Lyndon words into which the given string can be factorized.
vector<string> duval_lyndon_factorization(const string& s) {
    int n = s.size(), i = 0;
    vector<string> factors;
    while (i < n) {
        int j = i + 1, k = i;
        while (j < n && s[k] <= s[j]) {
            if (s[k] < s[j]) k = i; else k++;
            j++;
        }
        while (i <= k) { factors.push_back(s.substr(i, j - k)); i += j - k; }
    }
    return factors;
}

// ==========================================
// 2D: Grids and Matrices
// ==========================================

// Returns the number of distinct valid ways to color an NxM grid using C colors, treating grids that can be rotated to match each other as identical
int solve_2D_Grid_Rotations(int N, int M, int C) {
    int MOD_phi = MOD - 1;
    unsigned int S = (unsigned int)N * M;
    if (N == M) {
        int E1 = S % MOD_phi;
        int E2 = ((S + 1) / 2) % MOD_phi;
        int E3 = ((S + 3) / 4) % MOD_phi;
        int ans = (power(C, E1) + power(C, E2) + 2 * power(C, E3)) % MOD;
        return (ans * modInverse(4)) % MOD;
    } else {
        int E1 = S % MOD_phi;
        int E2 = ((S + 1) / 2) % MOD_phi;
        int ans = (power(C, E1) + power(C, E2)) % MOD;
        return (ans * modInverse(2)) % MOD;
    }
}

// Returns the number of distinct valid ways to color an NxN square grid using C colors, treating configurations that are equivalent under both rotation and reflection (mirroring) as identical
int solve_2D_Grid_All_Symmetries(int N, int C) {
    int MOD_phi = MOD - 1;
    unsigned int S = (unsigned int)N * N;
    int E1 = S % MOD_phi;
    int E2 = ((S + 1) / 2) % MOD_phi;
    int E3 = ((S + 3) / 4) % MOD_phi;
    int ans = (power(C, E1) + power(C, E2) + 2 * power(C, E3)) % MOD;
    int E_HV = (N * ((N + 1) / 2)) % MOD_phi;
    int E_Diag = ((S + N) / 2) % MOD_phi;
    ans = (ans + 2 * power(C, E_HV) + 2 * power(C, E_Diag)) % MOD;
    return (ans * modInverse(8)) % MOD;
}
// Returns the number of distinct valid ways to color an NxM grid using C colors, treating grids that match when flipped across their horizontal axis (top-to-bottom) as identical
int solve_2D_Grid_HFlip_Only(int N, int M, int C) {
    unsigned int S = (unsigned int)N * M;
    int MOD_phi = MOD - 1;
    int E1 = S % MOD_phi;
    int E2 = (M * ((N + 1) / 2)) % MOD_phi;
    int ans = (power(C, E1) + power(C, E2)) % MOD;
    return (ans * modInverse(2)) % MOD;
}
// Returns the number of distinct valid ways to color an NxM grid using C colors, treating grids that match when flipped across their vertical axis (left-to-right) as identical
int solve_2D_Grid_VFlip_Only(int N, int M, int C) {
    unsigned int S = (unsigned int)N * M;
    int MOD_phi = MOD - 1;
    int E1 = S % MOD_phi;
    int E2 = (N * ((M + 1) / 2)) % MOD_phi;
    int ans = (power(C, E1) + power(C, E2)) % MOD;
    return (ans * modInverse(2)) % MOD;
}
// Returns the number of distinct valid ways to color an NxM rectangular grid using C colors, treating configurations that match under 180-degree rotations, horizontal flips, or vertical flips as identical
int solve_2D_Grid_Klein4(int N, int M, int C) {
    unsigned int S = (unsigned int)N * M;
    int MOD_phi = MOD - 1;
    int E_id   = S % MOD_phi;
    int E_180  = ((S + 1) / 2) % MOD_phi;
    int E_hflip = (M * ((N + 1) / 2)) % MOD_phi;
    int E_vflip = (N * ((M + 1) / 2)) % MOD_phi;
    int ans = (power(C, E_id) + power(C, E_180) + power(C, E_hflip) + power(C, E_vflip)) % MOD;
    return (ans * modInverse(4)) % MOD;
}
// Returns the number of distinct valid ways to color an NxM toroidal grid (a grid that wraps around its edges) using C colors, treating configurations that match under cyclic shifts (translations) as identical
int solve_2D_Torus_Shifts(int N, int M, int C) {
    int ans = 0;
    for (int i = 0; i < N; i++) {
        int gi = cp_gcd(i == 0 ? N : i, N);
        int p = N / gi;
        for (int j = 0; j < M; j++) {
            int gj = cp_gcd(j == 0 ? M : j, M);
            int q = M / gj;
            int cycles = gi * gj * cp_gcd(p, q);
            ans = (ans + power(C, cycles)) % MOD;
        }
    }
    return (ans * modInverse((N % MOD) * (M % MOD) % MOD)) % MOD;
}
// Returns the number of distinct valid ways to color the specified parts (vertices, edges, or faces) of a 3D object using C colors, by automatically generating the full symmetry group from basic rotations and applying Burnside's Lemma
struct Vec3 { int x, y, z; bool operator<(const Vec3&o) const { return tie(x,y,z)<tie(o.x,o.y,o.z);} bool operator==(const Vec3&o) const {return x==o.x&&y==o.y&&z==o.z;} };
using Rot3 = function<Vec3(Vec3)>;
int burnside_from_generators(vector<Rot3> gens, vector<Vec3> parts, int C) {
    int n = parts.size();
    map<Vec3,int> idx;
    for (int i = 0; i < n; i++) idx[parts[i]] = i;
    auto toPerm = [&](const Rot3& f) {
        vector<int> p(n);
        for (int i = 0; i < n; i++) {
            Vec3 img = f(parts[i]);
            auto it = idx.find(img);
            p[i] = it->second; 
        }
        return p;
    };
    vector<vector<int>> genPerms;
    for (auto& g : gens) genPerms.push_back(toPerm(g));

    vector<int> identity(n); iota(identity.begin(), identity.end(), 0);
    set<vector<int>> group; group.insert(identity);
    queue<vector<int>> q; q.push(identity);
    while (!q.empty()) {
        vector<int> cur = q.front(); q.pop();
        for (auto& g : genPerms) {
            vector<int> nxt(n);
            for (int i = 0; i < n; i++) nxt[i] = g[cur[i]];
            if (group.insert(nxt).second) q.push(nxt);
        }
    }
    int total = 0;
    for (auto& perm : group) {
        vector<bool> vis(n, false);
        int cycles = 0;
        for (int i = 0; i < n; i++) {
            if (!vis[i]) {
                cycles++;
                int j = i;
                while (!vis[j]) { vis[j] = true; j = perm[j]; }
            }
        }
        total = (total + power(C, cycles)) % MOD;
    }
    return (total * modInverse((int)group.size())) % MOD;
}
// Returns a list of the 8 3D coordinate points corresponding to the corners (vertices) of a cube centered at the origin.
vector<Vec3> cube_vertices() {
    vector<Vec3> v;
    for (int x : {-1, 1}) for (int y : {-1, 1}) for (int z : {-1, 1}) v.push_back({x, y, z});
    return v;
}
// Returns a list of the 6 3D coordinate points corresponding to the centers of the faces of a cube centered at the origin.
vector<Vec3> cube_faces() {
    return { {1,0,0},{-1,0,0},{0,1,0},{0,-1,0},{0,0,1},{0,0,-1} };
} 
// Returns a list of the 12 3D coordinate points corresponding to the midpoints of the edges of a cube centered at the origin.
vector<Vec3> cube_edges() {
    vector<Vec3> e;
    for (int a : {-1, 1}) for (int b : {-1, 1}) { e.push_back({a,b,0}); e.push_back({a,0,b}); e.push_back({0,a,b}); }
    return e;
}
// Returns a list of two mathematical function generators (a 90-degree face rotation and a 120-degree vertex rotation) that, when combined, can produce all 24 proper 3D rotations of a cube or octahedron.
vector<Rot3> cube_rotation_generators() {
    Rot3 faceRot = [](Vec3 p) -> Vec3 { return { -p.y, p.x, p.z }; };
    Rot3 vertRot = [](Vec3 p) -> Vec3 { return { p.y, p.z, p.x }; };
    return { faceRot, vertRot };
}
// Returns a list of function generators that produce all 48 symmetries of a cube, including both proper 3D rotations and reflections (spatial inversion).
vector<Rot3> cube_full_symmetry_generators() {
    auto gens = cube_rotation_generators();
    gens.push_back([](Vec3 p) -> Vec3 { return { -p.x, -p.y, -p.z }; });
    return gens;
}
// Returns the number of distinct valid ways to color the faces, vertices, or edges of a cube using C colors, either under proper 3D rotations or full 3D symmetry (including reflections)
int solve_3D_Cube_Faces(int C)    { return burnside_from_generators(cube_rotation_generators(), cube_faces(), C); }
int solve_3D_Cube_Vertices(int C) { return burnside_from_generators(cube_rotation_generators(), cube_vertices(), C); }
int solve_3D_Cube_Edges(int C)    { return burnside_from_generators(cube_rotation_generators(), cube_edges(), C); }
int solve_3D_Cube_Faces_FullSym(int C)    { return burnside_from_generators(cube_full_symmetry_generators(), cube_faces(), C); }
int solve_3D_Cube_Vertices_FullSym(int C) { return burnside_from_generators(cube_full_symmetry_generators(), cube_vertices(), C); }
int solve_3D_Cube_Edges_FullSym(int C)    { return burnside_from_generators(cube_full_symmetry_generators(), cube_edges(), C); }

//Returns the number of distinct valid ways to color the faces, vertices, or edges of an octahedron using C colors
int solve_3D_Octahedron_Faces(int C)    { return solve_3D_Cube_Vertices(C); }
int solve_3D_Octahedron_Vertices(int C) { return solve_3D_Cube_Faces(C); }
int solve_3D_Octahedron_Edges(int C)    { return solve_3D_Cube_Edges(C); }
// Returns the number of distinct valid ways to color the vertices, faces, or edges of a regular tetrahedron using C colors, under both proper 3D rotations and full symmetry (including reflections)
vector<Vec3> tetra_vertices() { return { {1,1,1}, {1,-1,-1}, {-1,1,-1}, {-1,-1,1} }; }
vector<Vec3> tetra_faces()    { return tetra_vertices(); } 
vector<Vec3> tetra_edge_midpoints() {
    auto V = tetra_vertices();
    vector<Vec3> mids;
    for (int i = 0; i < 4; i++) for (int j = i + 1; j < 4; j++)
        mids.push_back({ V[i].x+V[j].x, V[i].y+V[j].y, V[i].z+V[j].z }); // 2x midpoint, still unique per edge
    return mids;
}
vector<Rot3> tetra_rotation_generators() {
    Rot3 r1 = [](Vec3 p) -> Vec3 { return { p.y, p.z, p.x }; };       // 120-degree vertex axis
    Rot3 r2 = [](Vec3 p) -> Vec3 { return { p.x, -p.y, -p.z }; };     // 180-degree edge axis
    return { r1, r2 };
}
vector<Rot3> tetra_full_symmetry_generators() {
    auto gens = tetra_rotation_generators();
    gens.push_back([](Vec3 p) -> Vec3 { return { p.y, p.x, p.z }; }); // swap x,y: odd permutation of vertices => reflection
    return gens;
}
int solve_3D_Tetrahedron_Vertices(int C) { return burnside_from_generators(tetra_rotation_generators(), tetra_vertices(), C); }
int solve_3D_Tetrahedron_Faces(int C)    { return burnside_from_generators(tetra_rotation_generators(), tetra_faces(), C); }
int solve_3D_Tetrahedron_Edges(int C)    { return burnside_from_generators(tetra_rotation_generators(), tetra_edge_midpoints(), C); }
int solve_3D_Tetrahedron_Vertices_FullSym(int C) { return burnside_from_generators(tetra_full_symmetry_generators(), tetra_vertices(), C); }
int solve_3D_Tetrahedron_Faces_FullSym(int C)    { return burnside_from_generators(tetra_full_symmetry_generators(), tetra_faces(), C); }
int solve_3D_Tetrahedron_Edges_FullSym(long long C)    { return burnside_from_generators(tetra_full_symmetry_generators(), tetra_edge_midpoints(), C); }

