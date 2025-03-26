Raport z realizacji zadań

Zadanie 1: Implementacja kolejki (FIFO) oraz stosu (LIFO)

Opis zadania

Celem zadania było zaimplementowanie kolejki (FIFO) oraz stosu (LIFO), a następnie przeprowadzenie operacji dodawania i usuwania 50 elementów. Dodatkowo należało zaimplementować obsługę błędów przy próbie pobrania elementu z pustej struktury.

Realizacja

Implementacja:

Stos został zaimplementowany jako lista dynamiczna, gdzie operacje push i pop realizują odpowiednio dodawanie i usuwanie elementów z końca listy.

Kolejka została zaimplementowana jako lista dynamiczna, gdzie operacje enqueue i dequeue realizują odpowiednio dodawanie na końcu oraz usuwanie elementów z początku listy.

Obsługa błędów została dodana w formie warunku sprawdzającego, czy struktura nie jest pusta przed usunięciem elementu.

Testowanie:

Dodano 50 elementów do każdej ze struktur.

Następnie usunięto wszystkie elementy, wypisując wartości w kolejności zgodnej z FIFO i LIFO.

Przetestowano próbę usunięcia elementu z pustej struktury, co poprawnie zgłosiło błąd.

Wyniki

Kolejka działa zgodnie z FIFO: pierwszy dodany element był pierwszym usuniętym.

Stos działa zgodnie z LIFO: ostatni dodany element był pierwszym usuniętym.

Obsługa błędów poprawnie wykrywa próbę pobrania elementu z pustej struktury.

Zadanie 2: Lista jednokierunkowa cykliczna

Opis zadania

Zaimplementowano listę jednokierunkową cykliczną liczb całkowitych oraz funkcje insert (dodawanie elementu) i merge (scalanie list). Przeprowadzono badania nad kosztami wyszukiwania losowych liczb.

Realizacja

Struktura danych:

Każdy węzeł zawiera liczbę oraz wskaźnik na następny element, tworząc cykl.

Wskaźnik listy wskazuje na dowolny element cyklu.

Funkcje:

insert(l, i): Dodaje nowy element z liczbą i do listy.

merge(l1, l2): Łączy dwie listy.

Testowanie:

Scalono dwie listy o 10 elementach każda.

Wstawiono 10 000 losowych liczb z zakresu [0, 100 000].

Przeprowadzono 1000 wyszukiwań liczb na liście oraz spoza niej, zliczając liczbę porównań.

Wyniki

Koszt wyszukiwania liczb znajdujących się na liście był niższy niż dla liczb spoza listy.

Łączenie list działa poprawnie, zachowując cykliczność struktury.

Zadanie 3: Lista dwukierunkowa cykliczna

Opis zadania

Rozszerzono poprzednie zadanie o wskaźniki na poprzedni element oraz wprowadzono losowy wybór kierunku wyszukiwania.

Realizacja

Struktura danych:

Każdy węzeł ma wskaźnik na poprzedni i następny element.

Wskaźnik listy wskazuje na dowolny element cyklu.

Funkcje:

insert(l, i): Dodaje nowy element.

merge(l1, l2): Łączy dwie listy.

Testowanie:

Scalono listy.

Wstawiono 10 000 liczb.

Wyszukiwania losowo wybierały kierunek przeszukiwania.

Wyniki

Wyszukiwanie w dwóch kierunkach obniżyło średni koszt.

Struktura zachowała cykliczność i poprawnie przechowywała dane.

Podsumowanie

Wszystkie struktury działają poprawnie.

Kolejka i stos spełniają wymagania FIFO i LIFO.

Implementacja list cyklicznych działa zgodnie z założeniami, a testy wyszukiwania potwierdziły zależność kosztu od rodzaju struktury.

Dwukierunkowość poprawiła efektywność wyszukiwania.

