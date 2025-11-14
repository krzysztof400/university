\* Minimalizacja_liczby_radiowozow *\
Minimize
Calkowita_liczba_przydzialow: Radiowozy_p1_z1 + Radiowozy_p1_z2
 + Radiowozy_p1_z3 + Radiowozy_p2_z1 + Radiowozy_p2_z2 + Radiowozy_p2_z3
 + Radiowozy_p3_z1 + Radiowozy_p3_z2 + Radiowozy_p3_z3
Subject To
Min_lacznie_dzielnica_p1: Radiowozy_p1_z1 + Radiowozy_p1_z2 + Radiowozy_p1_z3
 >= 10
Min_lacznie_dzielnica_p2: Radiowozy_p2_z1 + Radiowozy_p2_z2 + Radiowozy_p2_z3
 >= 14
Min_lacznie_dzielnica_p3: Radiowozy_p3_z1 + Radiowozy_p3_z2 + Radiowozy_p3_z3
 >= 13
Min_lacznie_zmiana_z1: Radiowozy_p1_z1 + Radiowozy_p2_z1 + Radiowozy_p3_z1
 >= 10
Min_lacznie_zmiana_z2: Radiowozy_p1_z2 + Radiowozy_p2_z2 + Radiowozy_p3_z2
 >= 20
Min_lacznie_zmiana_z3: Radiowozy_p1_z3 + Radiowozy_p2_z3 + Radiowozy_p3_z3
 >= 18
Bounds
 2 <= Radiowozy_p1_z1 <= 3
 4 <= Radiowozy_p1_z2 <= 7
 3 <= Radiowozy_p1_z3 <= 5
 3 <= Radiowozy_p2_z1 <= 5
 6 <= Radiowozy_p2_z2 <= 7
 5 <= Radiowozy_p2_z3 <= 10
 5 <= Radiowozy_p3_z1 <= 8
 7 <= Radiowozy_p3_z2 <= 12
 6 <= Radiowozy_p3_z3 <= 10
Generals
Radiowozy_p1_z1
Radiowozy_p1_z2
Radiowozy_p1_z3
Radiowozy_p2_z1
Radiowozy_p2_z2
Radiowozy_p2_z3
Radiowozy_p3_z1
Radiowozy_p3_z2
Radiowozy_p3_z3
End
