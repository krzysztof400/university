**Class: [[Algorithms and data structures]]**
**Date: 2025-04-29**
**Topics:**
## Wprowadzenie

Notacja asymptotyczna jest podstawowym narzędziem w analizie algorytmów, służącym do opisu ich wydajności w miarę wzrostu rozmiaru danych wejściowych. Umożliwia charakteryzowanie złożoności czasowej i pamięciowej algorytmów w sposób niezależny od szczegółów implementacji czy specyfiki sprzętu. Notatka omawia główne typy notacji asymptotycznych: ( O ), ( \Omega ), ( \Theta ), ( o ), ( \omega ), ich definicje, interpretacje oraz zastosowania.

## Definicje i Interpretacje

Notacja asymptotyczna opisuje zachowanie funkcji ( f(n) ) w granicy, gdy ( n \to \infty ). Skupia się na dominujących składnikach funkcji, ignorując stałe i mniej znaczące wyrazy. Poniżej przedstawiono formalne definicje.

### Notacja ( O ) (Górna granica)

Notacja ( O ) określa asymptotyczną górną granicę funkcji.

**Definicja**: Funkcja ( f(n) ) należy do ( O(g(n)) ), jeśli istnieją stałe ( c > 0 ) i ( n_0 \geq 0 ), takie że dla każdego ( n \geq n_0 ):

$$ f(n) \leq c \cdot g(n). $$

**Interpretacja**: ( f(n) ) rośnie nie szybciej niż ( g(n) ), pomnożone przez pewną stałą, dla wystarczająco dużych ( n ). Przykładowo, jeśli algorytm ma złożoność ( O(n^2) ), jego czas wykonania nie przekroczy kwadratowej funkcji rozmiaru wejścia.

### Notacja ( \Omega ) (Dolna granica)

Notacja ( \Omega ) określa asymptotyczną dolną granicę funkcji.

**Definicja**: Funkcja ( f(n) ) należy do ( \Omega(g(n)) ), jeśli istnieją stałe ( c > 0 ) i ( n_0 \geq 0 ), takie że dla każdego ( n \geq n_0 ):

$$ f(n) \geq c \cdot g(n). $$

**Interpretacja**: ( f(n) ) rośnie co najmniej tak szybko jak ( g(n) ), pomnożone przez pewną stałą. Na przykład, każdy algorytm sortowania oparty na porównaniach ma złożoność ( \Omega(n \log n) ).

### Notacja ( \Theta ) (Ścisła granica)

Notacja ( \Theta ) opisuje funkcje, które mają zarówno górną, jak i dolną granicę tego samego rzędu.

**Definicja**: Funkcja ( f(n) ) należy do ( \Theta(g(n)) ), jeśli istnieją stałe ( c_1, c_2 > 0 ) i ( n_0 \geq 0 ), takie że dla każdego ( n \geq n_0 ):

$$ c_1 \cdot g(n) \leq f(n) \leq c_2 \cdot g(n). $$

**Interpretacja**: ( f(n) ) rośnie dokładnie w tym samym tempie co ( g(n) ), z dokładnością do stałych. Przykładem jest algorytm MergeSort z złożonością ( \Theta(n \log n) ).

### Notacja ( o ) (Ścisła górna granica)

Notacja ( o ) opisuje funkcje, które rosną ściśle wolniej niż dana funkcja.

**Definicja**: Funkcja ( f(n) ) należy do ( o(g(n)) ), jeśli dla każdej stałej ( c > 0 ) istnieje ( n_0 \geq 0 ), takie że dla każdego ( n \geq n_0 ):

$$ f(n) < c \cdot g(n). $$

**Interpretacja**: ( f(n) ) jest asymptotycznie mniejsze niż ( g(n) ). Na przykład, ( n \in o(n^2) ).

### Notacja ( \omega ) (Ścisła dolna granica)

Notacja ( \omega ) opisuje funkcje, które rosną ściśle szybciej niż dana funkcja.

**Definicja**: Funkcja ( f(n) ) należy do ( \omega(g(n)) ), jeśli dla każdej stałej ( c > 0 ) istnieje ( n_0 \geq 0 ), takie że dla każdego ( n \geq n_0 ):

$$ f(n) > c \cdot g(n). $$

**Interpretacja**: ( f(n) ) jest asymptotycznie większe niż ( g(n) ). Na przykład, ( n^2 \in \omega(n) ).

## Relacje między Notacjami

Notacje są powiązane hierarchicznie:

- ( f(n) \in \Theta(g(n)) ) implikuje ( f(n) \in O(g(n)) ) i ( f(n) \in \Omega(g(n)) ).
- ( f(n) \in o(g(n)) ) implikuje ( f(n) \in O(g(n)) ), ale nie ( f(n) \in \Theta(g(n)) ).
- ( f(n) \in \omega(g(n)) ) implikuje ( f(n) \in \Omega(g(n)) ), ale nie ( f(n) \in \Theta(g(n)) ).

Diagram relacji można przedstawić w LaTeX jako:

```latex
\documentclass{standalone}
\usepackage{tikz}
\begin{document}
\begin{tikzpicture}
  \node (Theta) at (0,0) {$\Theta(g(n))$};
  \node (O) at (-2,2) {$O(g(n))$};
  \node (Omega) at (2,2) {$\Omega(g(n))$};
  \node (o) at (-4,4) {$o(g(n))$};
  \node (omega) at (4,4) {$\omega(g(n))$};
  \draw[->] (Theta) -- (O);
  \draw[->] (Theta) -- (Omega);
  \draw[->] (o) -- (O);
  \draw[->] (omega) -- (Omega);
\end{tikzpicture}
\end{document}
```

(Tutaj powinien znajdować się diagram hierarchii notacji asymptotycznych, pokazujący relacje między ( O ), ( \Omega ), ( \Theta ), ( o ), ( \omega ).)

## Przykłady Zastosowań

### Przykład 1: Analiza Algorytmu

Rozważmy funkcję ( f(n) = 3n^2 + 2n + 5 ). Chcemy określić jej złożoność w notacji asymptotycznej.

- **( O )**: Ponieważ ( 3n^2 + 2n + 5 \leq 3n^2 + 2n^2 + 5n^2 = 10n^2 ) dla ( n \geq 1 ), mamy ( f(n) \in O(n^2) ).
- **( \Omega )**: Ponieważ ( 3n^2 + 2n + 5 \geq 3n^2 ), mamy ( f(n) \in \Omega(n^2) ).
- **( \Theta )**: Łącząc powyższe, ( f(n) \in \Theta(n^2) ).

### Przykład 2: Algorytm Sortowania

Algorytm QuickSort ma:

- **Najlepszy przypadek**: ( \Theta(n \log n) ), gdy podziały są zrównoważone.
- **Średni przypadek**: ( \Theta(n \log n) ), w analizie probabilistycznej.
- **Najgorszy przypadek**: ( O(n^2) ), gdy tablica jest już posortowana, a pivot jest źle wybierany.

## Analiza Matematyczna

### Wyprowadzenie Granic

Rozważmy funkcję ( f(n) = n^2 + n ). Aby pokazać, że ( f(n) \in O(n^2) ):

1. Szukamy ( c ) i ( n_0 ), takich że ( n^2 + n \leq c \cdot n^2 ).
2. Dla ( n \geq 1 ), mamy ( n^2 + n \leq n^2 + n^2 = 2n^2 ).
3. Wybieramy ( c = 2 ), ( n_0 = 1 ), co spełnia definicję.

Podobnie dla ( \Omega(n^2) ):

1. ( n^2 + n \geq n^2 ).
2. Wybieramy ( c = 1 ), ( n_0 = 1 ).

### Porównanie Funkcji

Często porównujemy funkcje w notacji asymptotycznej. Na przykład:

- ( n \log n \in o(n^2) ), ponieważ ( \lim_{n \to \infty} \frac{n \log n}{n^2} = \lim_{n \to \infty} \frac{\log n}{n} = 0 ).
- ( n^2 \in \omega(n) ), ponieważ ( \lim_{n \to \infty} \frac{n^2}{n} = \lim_{n \to \infty} n = \infty ).

## Zastosowanie w Analizie Algorytmów

Notacja asymptotyczna pozwala:

- **Porównywać algorytmy**: Np. MergeSort (( \Theta(n \log n) )) jest asymptotycznie szybszy niż BubbleSort (( \Theta(n^2) )).
- **Ignorować stałe**: Skupiamy się na rzędzie wzrostu, a nie na dokładnych czasach wykonania.
- **Analizować przypadki**: Najlepszy, średni i najgorszy przypadek dają pełniejszy obraz wydajności.

(Tutaj powinien znajdować się wykres porównujący wzrost funkcji ( n ), ( n \log n ), ( n^2 ), ( 2^n ), z osią X jako ( n ) i osią Y jako wartość funkcji.)

## Pytania Sprawdzające

1. Podaj formalne definicje notacji ( O ), ( \Omega ), ( \Theta ). Wyjaśnij różnice między nimi na przykładzie funkcji ( f(n) = 2n + 3 ).
2. Pokaż, że funkcja ( f(n) = 5n^3 + 2n^2 + n ) należy do ( \Theta(n^3) ). Wyprowadź odpowiednie stałe ( c_1 ), ( c_2 ), ( n_0 ).
3. Dlaczego notacja ( o ) jest bardziej restrykcyjna niż ( O )? Podaj przykład funkcji ( f(n) \in o(n^2) ), ale ( f(n) \notin \Theta(n^2) ).
4. Dla algorytmu o złożoności ( O(n^2) ) w najgorszym przypadku i ( \Theta(n) ) w najlepszym przypadku, opisz możliwy scenariusz danych wejściowych dla każdego przypadku.
5. Używając granicy, pokaż, że ( n^2 \log n \in \omega(n^2) ).

## Podsumowanie

Notacja asymptotyczna (( O ), ( \Omega ), ( \Theta ), ( o ), ( \omega )) jest kluczowym narzędziem w analizie algorytmów, umożliwiającym opis ich wydajności w sposób zwięzły i niezależny od implementacji. ( O ) określa górną granicę, ( \Omega ) dolną, a ( \Theta ) ścisłą, podczas gdy ( o ) i ( \omega ) są bardziej restrykcyjne. Notacje te pozwalają porównywać algorytmy, analizować przypadki (najlepszy, średni, najgorszy) i skupiać się na rzędzie wzrostu funkcji. Zrozumienie notacji asymptotycznej jest fundamentem projektowania i analizy efektywnych algorytmów.