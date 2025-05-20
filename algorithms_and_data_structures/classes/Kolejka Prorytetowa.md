### Decrease/Increase
```
Increase/Decrease key(Q, i, newKey){
	if Q[i] > newKey
		Q[i] = newKey
		Heapify(Q,i)
	else
		while i > 1 && Q[Parent(i)] < newKey
			Q[i] = Q[Parent(i)]
			i = Parent(i)
		Q[i] = newKey 
}
```
O(n) = O(lg(n))
### Porównania i inne 

|             | Kopiec binarny | Kopiec dwumianowy | Kopiec Fibonacciego* |
| ----------- | -------------- | ----------------- | -------------------- |
| Insert      | O(logn)        | O(logn)           | O(1)                 |
| Maximum     | O(1)           | O(logn)           | O(1)                 |
| Extract max | O(logn)        | O(logn)           | O(logn)              |
| D/I key     | O(logn)        | O(logn)           | O(1)                 |
| Delete      | O(logn)        | O(logn)           | O(logn)              |
| Union       | O(n)           | O(logn)           | O(1)                 |
\*wartości zaamortyzowane, struktura lazy, czyli wymaga odpowiedniej struktury, kiedy takowej nie ma trzeba przebudować kopiec, przebudowanie jest drogie

'96 TREAP
2021 - Zip Tree, Tarjan