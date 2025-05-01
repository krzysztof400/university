**Class: [[Algorithms and data structures]]**
**Date: 2025-04-29**
**Topics:** #merge_sort #merge #sorting 
## Wprowadzenie

MergeSort (sortowanie przez scalanie) to efektywny, stabilny algorytm sortowania oparty na strategii "dziel i zwyciężaj". Polega na rekurencyjnym dzieleniu tablicy na mniejsze podtablice, sortowaniu ich, a następnie scalaniu w posortowaną całość. Algorytm ten jest szczególnie ceniony za przewidywalną złożoność czasową i stabilność, co czyni go odpowiednim do zastosowań akademickich i praktycznych.

## Definicja i Zasada Działania

MergeSort dzieli problem sortowania na trzy główne kroki:

1. **Podział**: Tablica jest rekurencyjnie dzielona na dwie mniej więcej równe części, aż każda podtablica zawiera jeden element (taka podtablica jest trywialnie posortowana).
2. **Rozwiązanie podproblemów**: Każda podtablica jest sortowana rekurencyjnie za pomocą tego samego algorytmu.
3. **Scalanie**: Posortowane podtablice są łączone w jedną posortowaną tablicę.

### Właściwości

- **Stabilność**: MergeSort zachowuje względną kolejność elementów o równych kluczach.
- **Rekurencja**: Wykorzystuje podejście rekurencyjne, co prowadzi do eleganckiego kodu, ale wymaga dodatkowej pamięci na stos wywołań.
- **Zastosowanie**: Idealny do dużych zbiorów danych, szczególnie gdy dane są przechowywane na nośnikach sekwencyjnych (np. dyskach).

## Pseudokod

Poniżej przedstawiono pseudokod algorytmu MergeSort oraz procedury scalania.

### Algorytm MergeSort

```
Funkcja MergeSort(T, lewy, prawy):
    // T - tablica do posortowania
    // lewy, prawy - indeksy określające zakres sortowania
    Jeśli lewy < prawy:
        środek = (lewy + prawy) // 2
        MergeSort(T, lewy, środek)         // Sortuj lewą połowę
        MergeSort(T, środek + 1, prawy)    // Sortuj prawą połowę
        Merge(T, lewy, środek, prawy)      // Scal podtablice
```

### Procedura Scalania (Merge)

```
Funkcja Merge(T, lewy, środek, prawy):
    // Tworzenie tymczasowych tablic dla lewej i prawej podtablicy
    L = T[lewy ... środek]
    P = T[środek + 1 ... prawy]
    i = 0, j = 0, k = lewy
    
    // Scalanie elementów w posortowanej kolejności
    Dopóki i < długość(L) i j < długość(P):
        Jeśli L[i] <= P[j]:
            T[k] = L[i]
            i = i + 1
        Inaczej:
            T[k] = P[j]
            j = j + 1
        k = k + 1
    
    // Kopiowanie pozostałych elementów z L, jeśli istnieją
    Dopóki i < długość(L):
        T[k] = L[i]
        i = i + 1
        k = k + 1
    
    // Kopiowanie pozostałych elementów z P, jeśli istnieją
    Dopóki j < długość(P):
        T[k] = P[j]
        j = j + 1
        k = k + 1
```

## Analiza Złożoności

### Złożoność Czasowa

Złożoność czasowa MergeSort wynika z rekurencyjnego podziału tablicy i liniowego scalania. Rozważmy tablicę o rozmiarze ( n ).

#### Równanie Rekurencyjne

Niech ( T(n) ) oznacza czas potrzebny do posortowania tablicy rozmiaru ( n ). Algorytm dzieli tablicę na dwie części, każda o rozmiarze ( \approx n/2 ), oraz wykonuje operację scalania o koszcie liniowym. Równanie rekurencyjne ma postać:

$$ T(n) = 2T\left(\frac{n}{2}\right) + \Theta(n) $$

- ( 2T\left(\frac{n}{2}\right) ): Dwa rekurencyjne wywołania dla podtablic.
- ( \Theta(n) ): Koszt scalania dwóch podtablic.

#### Rozwiązanie Równania

Równanie to można rozwiązać za pomocą twierdzenia o rekurencji (Master Theorem). Dla równania postaci:

$$ T(n) = aT\left(\frac{n}{b}\right) + f(n) $$

mamy:

- ( a = 2 ) (dwa podproblemy),
- ( b = 2 ) (podział na pół),
- ( f(n) = \Theta(n) ).

Porównujemy ( f(n) = \Theta(n) ) z ( n^{\log_b a} = n^{\log_2 2} = n ). Ponieważ ( f(n) = \Theta(n^{\log_b a}) ), stosujemy przypadek 2 twierdzenia:

$$ T(n) = \Theta(n^{\log_b a} \log n) = \Theta(n \log n). $$

Zatem złożoność czasowa wynosi ( \Theta(n \log n) ).

#### Przypadki

- **Najlepszy przypadek**: ( \Theta(n \log n) ). Nawet jeśli tablica jest już posortowana, algorytm wykonuje pełne podziały i scalania.
- **Średni przypadek**: ( \Theta(n \log n) ). Złożoność nie zależy od rozmieszczenia elementów.
- **Najgorszy przypadek**: ( \Theta(n \log n) ). Dotyczy np. tablicy posortowanej odwrotnie.

### Złożoność Pamięciowa

- **Przestrzeń dodatkowa**: MergeSort wymaga dodatkowej pamięci na tymczasowe tablice podczas scalania, co daje złożoność ( \Theta(n) ).
- **Stos rekurencji**: W implementacji rekurencyjnej stos przechowuje ( \Theta(\log n) ) wywołań.
- **Całkowita złożoność pamięciowa**: ( \Theta(n) ), dominowana przez tablice tymczasowe.

## Zalety i Wady

### Zalety

- **Stabilność**: Zachowuje kolejność równych elementów.
- **Przewidywalność**: Złożoność ( \Theta(n \log n) ) jest stała dla wszystkich przypadków.
- **Efektywność w systemach sekwencyjnych**: Dobrze sprawdza się w sortowaniu danych na dyskach.

### Wady

- **Zużycie pamięci**: Wymaga dodatkowej przestrzeni ( \Theta(n) ).
- **Brak in-place**: Nie sortuje tablicy w miejscu, co może być problematyczne przy ograniczonych zasobach.
- **Koszt rekurencji**: Głębokość rekurencji ( \Theta(\log n) ) może być problematyczna dla bardzo dużych tablic.

## Diagramy i Wizualizacje

(Tutaj powinien znajdować się diagram drzewa rekurencji MergeSort, przedstawiający podział tablicy na podtablice oraz proces scalania. Oś X mogłaby reprezentować indeksy tablicy, a oś Y poziomy rekurencji.)

## Pytania Sprawdzające

1. Wyjaśnij, dlaczego MergeSort jest algorytmem stabilnym. Podaj przykład sytuacji, w której stabilność ma znaczenie.
2. Wyprowadź równanie rekurencyjne dla MergeSort i pokaż, jak prowadzi do złożoności ( \Theta(n \log n) ).
3. Porównaj MergeSort z QuickSort pod względem złożoności czasowej i pamięciowej. W jakich sytuacjach wybrałbyś MergeSort?
4. Jak zmodyfikować procedurę scalania, aby zminimalizować zużycie pamięci dodatkowej? Czy jest to możliwe bez utraty stabilności?
5. Oblicz dokładną liczbę porównań w procedurze scalania dla tablicy o rozmiarze ( n = 8 ), zakładając, że podtablice są równe.

## Podsumowanie

MergeSort to klasyczny algorytm sortowania o złożoności czasowej ( \Theta(n \log n) ) we wszystkich przypadkach, co czyni go przewidywalnym i niezawodnym. Jego stabilność i efektywność w systemach sekwencyjnych są kluczowymi zaletami, choć wymaga dodatkowej pamięci ( \Theta(n) ). Algorytm opiera się na strategii "dziel i zwyciężaj", co prowadzi do eleganckiej implementacji, ale wiąże się z kosztem rekurencji. Zrozumienie MergeSort jest istotne dla analizy algorytmów i projektowania efektywnych systemów sortowania.