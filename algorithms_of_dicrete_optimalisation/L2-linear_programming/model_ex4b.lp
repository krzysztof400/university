\* Najkrotsza_sciezka_z_limitem_czasu *\
Minimize
Calkowity_koszt_sciezki: 100 Sciezka_(1,_10) + 2 Sciezka_(1,_2)
 + 3 Sciezka_(1,_3) + 6 Sciezka_(1,_5) + 3 Sciezka_(2,_10) + 3 Sciezka_(3,_4)
 + 4 Sciezka_(4,_10) + 6 Sciezka_(5,_10)
Subject To
Limit_czasu: Sciezka_(1,_10) + 10 Sciezka_(1,_2) + 3 Sciezka_(1,_3)
 + 6 Sciezka_(1,_5) + 10 Sciezka_(2,_10) + 3 Sciezka_(3,_4)
 + 4 Sciezka_(4,_10) + 6 Sciezka_(5,_10) <= 15
Przeplyw_koniec_10: Sciezka_(1,_10) + Sciezka_(2,_10) + Sciezka_(4,_10)
 + Sciezka_(5,_10) = 1
Przeplyw_posredni_2: Sciezka_(1,_2) - Sciezka_(2,_10) = 0
Przeplyw_posredni_3: Sciezka_(1,_3) - Sciezka_(3,_4) = 0
Przeplyw_posredni_4: Sciezka_(3,_4) - Sciezka_(4,_10) = 0
Przeplyw_posredni_5: Sciezka_(1,_5) - Sciezka_(5,_10) = 0
Przeplyw_start_1: Sciezka_(1,_10) + Sciezka_(1,_2) + Sciezka_(1,_3)
 + Sciezka_(1,_5) = 1
Binaries
Sciezka_(1,_10)
Sciezka_(1,_2)
Sciezka_(1,_3)
Sciezka_(1,_5)
Sciezka_(2,_10)
Sciezka_(3,_4)
Sciezka_(4,_10)
Sciezka_(5,_10)
End
