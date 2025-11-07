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
    vector<vector<int>> und(G.n+1);

    
    for(int u=1;u<=G.n;u++){
        for(int v: G.adj[u]){ und[u].push_back(v); und[v].push_back(u); }
    }
    vector<int> color(G.n+1,0);
    queue<int> q;
    for(int i=1;i<=G.n;i++){
        if(color[i]==0){
            color[i]=1; q.push(i);
            while(!q.empty()){
                int u=q.front(); q.pop();
                for(int v: und[u]){
                    if(color[v]==0){ color[v]=3-color[u]; q.push(v); }
                    else if(color[v]==color[u]){ cout<<"BIPARTITE NO\n"; return 0; }
                }
            }
        }
    }
    cout<<"BIPARTITE YES\n";
    if(G.n <= 200){
        cout<<"V0:";
        for(int i=1;i<=G.n;i++) if(color[i]==1) cout<<' '<<i;
        cout<<"\nV1:";
        for(int i=1;i<=G.n;i++) if(color[i]==2) cout<<' '<<i;
        cout<<"\n";
    }
    return 0;
}
