**Class: [[Functional programming]]**
**Date: 2025-04-29**
**Topics:** #monoid #semigroup 
Semigroup (półgrupy): (X,\*) : \* - łączne
Monoid (X, \*, e):  (X,\*) - grupa
				 e -element neutralny

Endomorfizmy:

$$
Endo(X) = (X^X, \circ, Id_x)
$$
Własność:
$$
(N, *, e) \leftarrow monoid
$$
$$
(N, \circ, e)
$$
$$
x * y = y \circ x
$$
to
$$
(N, \circ, e) \leftarrow monoid
$$

$$ 
M \cup \{ \uparrow \\ \} : \circ | M\times M = \star
$$
```
fold r:: (a -> (b -> b)) -> b -> STa -> b
		((a -> Endo(b)) -> b -> STa -> b
fold f b tree = (foldMap(\x->Endo(\y->fxy))tree)

foldr (\x acc -> x+ acc)0[x1,x2,x3]
	foldMap(\x acc -> x+ acc)0[x1,x2,x3]=
	-> ((\a -> x1+a)0(\a->x2+a)0(\a->x3+a))
```