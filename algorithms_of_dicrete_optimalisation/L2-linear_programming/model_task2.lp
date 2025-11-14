\* Maksymalizacja_zysku_produkcji *\
Maximize
Calkowity_Zysk: 4.2 p1 + 5.5 p2 + 4.55 p3 + 3.75 p4
Subject To
Czas_pracy_maszyny_m1: 5 p1 + 3 p2 + 4 p3 + 4 p4 <= 3600
Czas_pracy_maszyny_m2: 10 p1 + 6 p2 + 5 p3 + 2 p4 <= 3600
Czas_pracy_maszyny_m3: 6 p1 + 4 p2 + 3 p3 + p4 <= 3600
Bounds
 p1 <= 400
 p2 <= 100
 p3 <= 150
 p4 <= 500
End
