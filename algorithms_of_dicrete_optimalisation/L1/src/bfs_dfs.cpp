#include <bits/stdc++.h>
using namespace std;

struct Graph { bool directed; int n,m; vector<vector<int>> adj; void read(){
    string f;
    if(!(cin>>f)) exit(1);
    directed = (f=="D"||f=="d");
    cin>>n>>m; adj.assign(n+1, {});
    for(int i=0;i<m;i++){
        int u,v;
        cin>>u>>v;
        if(u>=1&&u<=n && v>=1&&v<=n){ 
            adj[u].push_back(v); if(!directed) adj[v].push_back(u); 
        }
    }
}};

struct Res { vector<int> order; vector<pair<int,int>> tree; };

Res bfs(const Graph& G, int start){
    Res r; 
    vector<char> vis(G.n+1,0); 
    queue<int> q; 
    auto start_from=[&](int s){
        if(vis[s]) return; 
        vis[s]=1; 
        q.push(s); 
        r.order.push_back(s);
        while(!q.empty()){
            int u=q.front(); q.pop();
            for(int v: G.adj[u]) if(!vis[v]){
                vis[v]=1; r.tree.emplace_back(u,v); q.push(v); r.order.push_back(v);
            }
        }
    };
    if(start>=1 && start<=G.n) start_from(start);
    for(int i=1;i<=G.n;i++) if(!vis[i]) start_from(i);
    return r;
}
Res dfs(const Graph& G, int start){
    Res r; vector<char> vis(G.n+1,0);
    auto start_from=[&](int s){
        if(vis[s]) return;
        vector<pair<int,int>> st;
        st.emplace_back(s,0); vis[s]=1; r.order.push_back(s);
        while(!st.empty()){
            int u = st.back().first;
            int &it = st.back().second;
            if(it < (int)G.adj[u].size()){
                int v = G.adj[u][it++];
                if(!vis[v]){ vis[v]=1; r.tree.emplace_back(u,v); r.order.push_back(v); st.emplace_back(v,0); }
            } 
            else st.pop_back();
        }
    };
    if(start>=1 && start<=G.n) start_from(start);
    for(int i=1;i<=G.n;i++) if(!vis[i]) start_from(i);
    return r;
}
int main(int argc,char**argv){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string mode="dfs";
    int start=-1;
    int print_tree=0;
    for(int i=1;i<argc;i++){
        string s=argv[i];
        if(s=="dfs"||s=="bfs") mode=s;
        else if(s.rfind("start=",0)==0) start=stoi(s.substr(6));
        else if(s.rfind("print_tree=",0)==0) print_tree=stoi(s.substr(11));
    }
    Graph G; G.read();
    Res res = (mode=="bfs") ? bfs(G,start) : dfs(G,start);
    cout<<"VISIT_ORDER:";
    for(int v: res.order) cout<<' '<<v;
    cout<<"\n";
    if(print_tree){
        cout<<"TREE_EDGES:\n";
        for(auto &e: res.tree) cout<<e.first<<' '<<e.second<<'\n';
    }
    return 0;
}
