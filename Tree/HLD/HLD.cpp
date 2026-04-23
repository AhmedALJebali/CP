struct HLD {
  int Nodes,t=1,root=1;
  vector<int>parent,depth,heavy,pos,sz,head;
  vector<vector<int>>adj;
  HLD(int n, vector<vector<int>>&g ) {
    Nodes=n,adj=g;
    parent.assign(n+1,0);
    depth.assign(n+1,0);
    heavy.assign(n+1,0);
    pos.assign(n+1,0);
    sz.assign(n+1,0);
    head.assign(n+1,0);
    dfs(root,0);
    decompose(root,root);
  }
  void dfs(int u, int p) {
    parent[u]=p;
    sz[u]=1;
    for (auto v:adj[u]) {
      if (v==p)continue;
      depth[v]=depth[u]+1;
      dfs(v,u);
      sz[u]+=sz[v];
      if (!heavy[u] || sz[heavy[u]]<sz[v]) heavy[u]=v;
    }
  }
  void decompose(int u, int h) {
    head[u]=h;
    pos[u]=t++;
    if (heavy[u]) decompose(heavy[u],h);
    for (auto v : adj[u]) {
      if (v == parent[u] || v == heavy[u]) continue;
      decompose(v, v);
    }
  }
  vector<pair<int,int>> path(int u,int  v) {
    vector<pair<int,int>> ret;
    while (head[u]!=head[v]) {
      if (depth[head[u]]<depth[head[v]])swap(u,v);
      ret.push_back({pos[head[u]],pos[u]});
      u=parent[head[u]];
    }
    if (depth[u] > depth[v])
      swap(u, v);
    ret.push_back({pos[u], pos[v]});
    return ret;
  };
};