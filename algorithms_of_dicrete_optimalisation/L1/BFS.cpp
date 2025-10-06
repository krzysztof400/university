#include <bits/stdc++.h>
using namespace std;

using Graph = vector<vector<int>>;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    char flag;
    int n, m;

    if (!(cin >> flag >> n >> m)) {
        cerr << "Blad wczytywania podstawowych danych grafu\n";
        return 1;
    }

    if (flag != 'D' && flag != 'U') {
        cerr << "Niepoprawna flaga grafu. Uzyj 'D' lub 'U'.\n";
        return 1;
    }

    if (n <= 0 || m < 0) {
        cerr << "Niepoprawna liczba wierzcholkow lub krawedzi\n";
        return 1;
    }

    Graph adj(n + 1);

    for (int i = 0; i < m; i++) {
        int u, v;
        if (!(cin >> u >> v)) {
            cerr << "Blad wczytywania krawedzi nr " << i + 1 << "\n";
            return 1;
        }
        if (u < 1 || u > n || v < 1 || v > n) {
            cerr << "Niepoprawny numer wierzcholka w krawedzi nr " << i + 1 << "\n";
            return 1;
        }
        adj[u].push_back(v);
        if (flag == 'U') adj[v].push_back(u);
    }

    for (int i = 1; i <= n; i++) {
        cout << i << ": ";
        for (int v : adj[i]) cout << v << " ";
        cout << "\n";
    }

    return 0;
}

void BFS(){
    
}