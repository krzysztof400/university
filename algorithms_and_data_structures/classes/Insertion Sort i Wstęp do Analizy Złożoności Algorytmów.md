## 1. Wprowadzenie

Analiza złożoności algorytmów oraz znajomość prostych algorytmów sortujących to kluczowe elementy wykształcenia informatycznego. Insertion Sort (sortowanie przez wstawianie) to jeden z podstawowych algorytmów, idealny do nauki podstaw analizy złożoności obliczeniowej.

---

## 2. Insertion Sort (Sortowanie przez wstawianie)

### 2.1 Opis algorytmu

Algorytm Insertion Sort sortuje tablicę, budując posortowaną część krok po kroku: dla każdego elementu znajduje odpowiednie miejsce w posortowanej części i wstawia go tam, przesuwając pozostałe elementy.

### 2.2 Pseudokod

```text
InsertionSort(A)
  for i ← 1 to length(A) - 1
    key ← A[i]
    j ← i - 1
    while j ≥ 0 and A[j] > key
      A[j + 1] ← A[j]   // przesuwanie elementów większych od key
      j ← j - 1
    A[j + 1] ← key       // wstawienie elementu key na właściwe miejsce
```

### 2.3 Przykład działania

Dla wejścia: `A = [5, 2, 4, 6, 1, 3]`, kolejne kroki działania algorytmu doprowadzą do:

```
[2, 5, 4, 6, 1, 3]
[2, 4, 5, 6, 1, 3]
[2, 4, 5, 6, 1, 3]
[1, 2, 4, 5, 6, 3]
[1, 2, 3, 4, 5, 6]
```

---

## 3. Analiza złożoności czasowej i pamięciowej

### 3.1 Wprowadzenie do złożoności obliczeniowej

Złożoność algorytmu określa zasoby (czas, pamięć), jakie są potrzebne do jego wykonania względem rozmiaru danych wejściowych nn.

Typowe notacje:

$$
O(g(n))\mathcal{O}(g(n)): \text{ograniczenie z gory (ang. _Big-O_)
$$

$$
Ω(g(n))\Omega(g(n)): \text{ograniczenie z dołu}
$$

$$
 Θ(g(n))\Theta(g(n)):  \text{ścisłe ograniczenie (góra i dół)}
$$


### 3.2 Złożoność czasowa Insertion Sort

#### a) Najlepszy przypadek (dane już posortowane)

Dla każdego ii, warunek `A[j] > key` w pętli `while` jest fałszywy natychmiast:

$$
T(n)=∑i=2n1=O(n)T(n) = \sum_{i=2}^{n} 1 = \mathcal{O}(n)
$$

#### b) Najgorszy przypadek (dane posortowane malejąco)

Każdy nowy element jest wstawiany na początek tablicy, co wymaga przesunięcia wszystkich wcześniejszych elementów:

T(n)=∑i=2n(i−1)=∑k=1n−1k=(n−1)n2=O(n2)T(n) = \sum_{i=2}^{n} (i - 1) = \sum_{k=1}^{n-1} k = \frac{(n - 1)n}{2} = \mathcal{O}(n^2)

#### c) Przypadek średni

Zakładamy losowe permutacje danych. Średnia liczba przesunięć dla elementu ii to i/2i/2:

T(n)≈∑i=2ni2=12∑i=2ni=O(n2)T(n) \approx \sum_{i=2}^{n} \frac{i}{2} = \frac{1}{2} \sum_{i=2}^{n} i = \mathcal{O}(n^2)

### 3.3 Złożoność pamięciowa

Algorytm jest _in-place_, tzn. nie używa dodatkowej pamięci poza stałymi pomocniczymi zmiennymi:

- Pamięć: O(1)\mathcal{O}(1)
    

---

## 4. Cechy Insertion Sort

|Cecha|Wartość|
|---|---|
|Stabilność|Tak|
|Sortowanie _in-place_|Tak|
|Złożoność czasowa (najg.)|O(n2)\mathcal{O}(n^2)|
|Złożoność czasowa (najlep.)|O(n)\mathcal{O}(n)|
|Złożoność pamięciowa|O(1)\mathcal{O}(1)|
|Nadaje się dla małych danych|Tak|

---

## 5. Wstęp do analizy złożoności algorytmów

### 5.1 Cel analizy

Zrozumienie, jak długo będzie działał algorytm oraz jakich zasobów wymaga, niezależnie od konkretnej maszyny.

### 5.2 Asymptotyczna analiza

W analizie asymptotycznej pomijamy stałe i niskiego rzędu składniki, skupiając się na zachowaniu dla dużych nn.

- Funkcje porównawcze:
    
    - Liniowa: nn
        
    - Kwadratowa: n2n^2
        
    - Logarytmiczna: log⁡n\log n
        
    - Liniowo-logarytmiczna: nlog⁡nn \log n
        
    - Wykładnicza: 2n2^n
        

**Miejsce na wykres porównania funkcji złożoności**: „Wykres przedstawiający funkcje: log⁡n\log n, nn, nlog⁡nn \log n, n2n^2, 2n2^n”.

### 5.3 Czas rzeczywisty vs liczba operacji

Analiza teoretyczna operuje na liczbie operacji elementarnych, a nie na czasie wykonania w sekundach, który zależy od sprzętu.

---

## 6. Pytania sprawdzające

1. Na czym polega algorytm Insertion Sort?
    
2. Jaka jest złożoność czasowa Insertion Sort w najlepszym i najgorszym przypadku?
    
3. Czym różni się notacja O(n)\mathcal{O}(n) od Θ(n)\Theta(n)?
    
4. Czy Insertion Sort jest algorytmem stabilnym?
    
5. Dlaczego analiza asymptotyczna pomija stałe?
    

---

## 7. Podsumowanie

- **Insertion Sort** to prosty, stabilny algorytm sortowania działający w miejscu.
    
- Ma **złożoność czasową O(n2)\mathcal{O}(n^2)** w najgorszym przypadku, ale może działać liniowo na danych już posortowanych.
    
- **Złożoność asymptotyczna** pozwala analizować algorytmy niezależnie od sprzętu.
    
- **Notacje asymptotyczne** (O,Θ,Ω\mathcal{O}, \Theta, \Omega) służą do klasyfikacji algorytmów według efektywności.
    

---