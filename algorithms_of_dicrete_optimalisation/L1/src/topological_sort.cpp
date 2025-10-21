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

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Graph G; G.read();
    vector<int> indeg(G.n+1,0);

    for(int u=1;u<=G.n;u++) for(int v: G.adj[u]) indeg[v]++;

    queue<int> q;
    for(int i=1;i<=G.n;i++) if(indeg[i]==0) q.push(i);

    vector<int> order;
    order.reserve(G.n);

    while(!q.empty()){
        int u=q.front(); q.pop(); order.push_back(u);
        for(int v: G.adj[u]) if(--indeg[v]==0) q.push(v);
    }

    if((int)order.size() != G.n) {
        cout<<"CYCLE\n";
    } 
    else {
        cout<<"ACYCLIC\n";
        if(G.n <= 200){
            cout<<"TOPO_ORDER:";
            for(int v: order) cout<<' '<<v;
            cout<<"\n";
        }
    }
    return 0;
}
