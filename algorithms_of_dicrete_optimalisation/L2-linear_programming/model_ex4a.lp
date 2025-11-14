\* Najkrotsza_sciezka_z_limitem_czasu *\
Minimize
Calkowity_koszt_sciezki: 3 Sciezka_(1,_2) + 4 Sciezka_(1,_3)
 + 7 Sciezka_(1,_4) + 8 Sciezka_(1,_5) + 2 Sciezka_(2,_3) + 6 Sciezka_(3,_10)
 + 4 Sciezka_(3,_4) + 2 Sciezka_(3,_5) + Sciezka_(4,_5) + 3 Sciezka_(4,_7)
 + 5 Sciezka_(5,_10) + 5 Sciezka_(5,_6) + 3 Sciezka_(5,_7) + 5 Sciezka_(6,_1)
 + 7 Sciezka_(6,_10) + 2 Sciezka_(6,_7) + 4 Sciezka_(7,_3) + 3 Sciezka_(7,_8)
 + Sciezka_(7,_9) + Sciezka_(8,_9) + 2 Sciezka_(9,_10)
Subject To
Limit_czasu: 4 Sciezka_(1,_2) + 9 Sciezka_(1,_3) + 10 Sciezka_(1,_4)
 + 12 Sciezka_(1,_5) + 3 Sciezka_(2,_3) + 11 Sciezka_(3,_10)
 + 6 Sciezka_(3,_4) + 2 Sciezka_(3,_5) + Sciezka_(4,_5) + 5 Sciezka_(4,_7)
 + 8 Sciezka_(5,_10) + 6 Sciezka_(5,_6) + 3 Sciezka_(5,_7) + 8 Sciezka_(6,_1)
 + 11 Sciezka_(6,_10) + 2 Sciezka_(6,_7) + 6 Sciezka_(7,_3) + 5 Sciezka_(7,_8)
 + Sciezka_(7,_9) + 2 Sciezka_(8,_9) + 2 Sciezka_(9,_10) <= 15
Przeplyw_koniec_10: Sciezka_(3,_10) + Sciezka_(5,_10) + Sciezka_(6,_10)
 + Sciezka_(9,_10) = 1
Przeplyw_posredni_2: Sciezka_(1,_2) - Sciezka_(2,_3) = 0
Przeplyw_posredni_3: Sciezka_(1,_3) + Sciezka_(2,_3) - Sciezka_(3,_10)
 - Sciezka_(3,_4) - Sciezka_(3,_5) + Sciezka_(7,_3) = 0
Przeplyw_posredni_4: Sciezka_(1,_4) + Sciezka_(3,_4) - Sciezka_(4,_5)
 - Sciezka_(4,_7) = 0
Przeplyw_posredni_5: Sciezka_(1,_5) + Sciezka_(3,_5) + Sciezka_(4,_5)
 - Sciezka_(5,_10) - Sciezka_(5,_6) - Sciezka_(5,_7) = 0
Przeplyw_posredni_6: Sciezka_(5,_6) - Sciezka_(6,_1) - Sciezka_(6,_10)
 - Sciezka_(6,_7) = 0
Przeplyw_posredni_7: Sciezka_(4,_7) + Sciezka_(5,_7) + Sciezka_(6,_7)
 - Sciezka_(7,_3) - Sciezka_(7,_8) - Sciezka_(7,_9) = 0
Przeplyw_posredni_8: Sciezka_(7,_8) - Sciezka_(8,_9) = 0
Przeplyw_posredni_9: Sciezka_(7,_9) + Sciezka_(8,_9) - Sciezka_(9,_10) = 0
Przeplyw_start_1: Sciezka_(1,_2) + Sciezka_(1,_3) + Sciezka_(1,_4)
 + Sciezka_(1,_5) - Sciezka_(6,_1) = 1
Binaries
Sciezka_(1,_2)
Sciezka_(1,_3)
Sciezka_(1,_4)
Sciezka_(1,_5)
Sciezka_(2,_3)
Sciezka_(3,_10)
Sciezka_(3,_4)
Sciezka_(3,_5)
Sciezka_(4,_5)
Sciezka_(4,_7)
Sciezka_(5,_10)
Sciezka_(5,_6)
Sciezka_(5,_7)
Sciezka_(6,_1)
Sciezka_(6,_10)
Sciezka_(6,_7)
Sciezka_(7,_3)
Sciezka_(7,_8)
Sciezka_(7,_9)
Sciezka_(8,_9)
Sciezka_(9,_10)
End
