\* Minimalizacja_kosztow_produkcji_i_magazynowania *\
Minimize
Calkowity_koszt: 1500 Magazyn_1 + 1500 Magazyn_2 + 1500 Magazyn_3
 + 1500 Magazyn_4 + 8000 Produkcja_nadwymiarowa_1
 + 6000 Produkcja_nadwymiarowa_2 + 10000 Produkcja_nadwymiarowa_3
 + 11000 Produkcja_nadwymiarowa_4 + 6000 Produkcja_normalna_1
 + 4000 Produkcja_normalna_2 + 8000 Produkcja_normalna_3
 + 9000 Produkcja_normalna_4
Subject To
Bilans_magazynu_okres_1: Magazyn_0 - Magazyn_1 + Produkcja_nadwymiarowa_1
 + Produkcja_normalna_1 = 130
Bilans_magazynu_okres_2: Magazyn_1 - Magazyn_2 + Produkcja_nadwymiarowa_2
 + Produkcja_normalna_2 = 80
Bilans_magazynu_okres_3: Magazyn_2 - Magazyn_3 + Produkcja_nadwymiarowa_3
 + Produkcja_normalna_3 = 125
Bilans_magazynu_okres_4: Magazyn_3 - Magazyn_4 + Produkcja_nadwymiarowa_4
 + Produkcja_normalna_4 = 195
Limit_magazynu_1: Magazyn_1 <= 70
Limit_magazynu_2: Magazyn_2 <= 70
Limit_magazynu_3: Magazyn_3 <= 70
Limit_magazynu_4: Magazyn_4 <= 70
Limit_prod_nadwym_1: Produkcja_nadwymiarowa_1 <= 60
Limit_prod_nadwym_2: Produkcja_nadwymiarowa_2 <= 65
Limit_prod_nadwym_3: Produkcja_nadwymiarowa_3 <= 70
Limit_prod_nadwym_4: Produkcja_nadwymiarowa_4 <= 60
Limit_prod_normalnej_1: Produkcja_normalna_1 <= 100
Limit_prod_normalnej_2: Produkcja_normalna_2 <= 100
Limit_prod_normalnej_3: Produkcja_normalna_3 <= 100
Limit_prod_normalnej_4: Produkcja_normalna_4 <= 100
Magazyn_poczatek: Magazyn_0 = 15
End
