#include <bits/stdc++.h>
using namespace std;
struct Graph{ bool directed; int n,m; vector<vector<int>> adj; void read(){
    string f; if(!(cin>>f)) exit(1);
    directed = (f=="D"||f=="d");
    cin>>n>>m; adj.assign(n+1,{});
    for(int i=0;i<m;i++){ int u,v; cin>>u>>v; if(u>=1&&u<=n && v>=1&&v<=n){ adj[u].push_back(v); if(!directed) adj[v].push_back(u);} }
}};

struct Tarjan {
    const Graph &G;
    vector<int> disc, low, st;
    vector<char> on;
    int time=0;
    vector<vector<int>> comps;
    Tarjan(const Graph& g): G(g) {
        disc.assign(G.n+1,-1);
        low.assign(G.n+1,0);
        on.assign(G.n+1,0);
    }
    void run(){ for(int i=1;i<=G.n;i++) if(disc[i]==-1) dfs(i); }
    void dfs(int u){
        disc[u]=low[u]=time++; st.push_back(u); on[u]=1;
        for(int v: G.adj[u]){
            if(disc[v]==-1){ 
                dfs(v); low[u]=min(low[u], low[v]);
            }
            else if(on[v]) low[u]=min(low[u], disc[v]);
        }
        if(low[u]==disc[u]){
            comps.emplace_back();
            while(true){
                int w=st.back(); st.pop_back(); on[w]=0;
                comps.back().push_back(w);
                if(w==u) break;
            }
        }
    }
};
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Graph G; G.read();
    Tarjan t(G); t.run();
    cout<<"SCC_COUNT "<<t.comps.size()<<"\n";
    cout<<"SCC_SIZES:";
    for(auto &c: t.comps) cout<<' '<<c.size();
    cout<<"\n";
    if(G.n <= 200){
        cout<<"SCC_COMPONENTS:\n";
        for(size_t i=0;i<t.comps.size();++i){
            cout<< (i+1) << ":";
            for(int v: t.comps[i]) cout<<' '<<v;
            cout<<"\n";
        }
    }
    return 0;
}
