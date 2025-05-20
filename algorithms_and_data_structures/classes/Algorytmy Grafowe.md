|V| = n 
|E| = m
#### Definicja: Graf G = (V,E)
$$
	V - \text{zbiór wierzchołków} \ \ \set{1..n}
$$
$$
E \subseteq \set{\set{i,j}: i,j \in V}
$$
$$
$$
Graf skierowany:
$$
E \subseteq \set{(i,j): i,j\in V, i!=j}
$$
#### Metody przechowywania
##### Lista sąsiedztwa

| V1  | V3  |     |     |
| --- | --- | --- | --- |
| V2  | V1  | V3  | V5  |
| V3  | V2  |     |     |
| V4  | V1  | V5  |     |
| V5  | V3  |     |     |
| V6  |     |     |     |
O(n+m)
n+m - wielkość grafu
Listy sąsiedztwa - grafy rzadkie
##### Macierz sąsiedztwa
A = (a,i,j)
i = 1..n
j = j..n
aij = 
Drzewo to graf który nie posaida cykli
Macierz sąsiedztwa - grafy gęste 

```
explore (G,v){
	visited(v) = true
	previsit(v)
	for each edge (v,u) in E
		if not visited(u)
			explore(G,v)
	postvisit(v)
}
```
DFS - Depth Search First
```
DFS (G){
	for all v in V
		visited(v) = false
	for all v in V
		if not visited(v)
			explore(G,v)
}
```

