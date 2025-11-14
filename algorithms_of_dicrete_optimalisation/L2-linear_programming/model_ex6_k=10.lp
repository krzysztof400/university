\* Rozmieszczenie_kamer_k=10 *\
Minimize
Liczba_kamer: Kamera_(0,_1) + Kamera_(0,_2) + Kamera_(0,_3) + Kamera_(1,_0)
 + Kamera_(1,_1) + Kamera_(1,_2) + Kamera_(1,_3) + Kamera_(1,_4)
 + Kamera_(2,_0) + Kamera_(2,_1) + Kamera_(2,_3) + Kamera_(2,_4)
 + Kamera_(3,_0) + Kamera_(3,_1) + Kamera_(3,_2) + Kamera_(3,_3)
 + Kamera_(3,_4) + Kamera_(4,_0) + Kamera_(4,_2) + Kamera_(4,_4)
Subject To
Pokrycie_kontenera_0_0: Kamera_(0,_1) + Kamera_(0,_2) + Kamera_(0,_3)
 + Kamera_(1,_0) + Kamera_(2,_0) + Kamera_(3,_0) + Kamera_(4,_0) >= 1
Pokrycie_kontenera_0_4: Kamera_(0,_1) + Kamera_(0,_2) + Kamera_(0,_3)
 + Kamera_(1,_4) + Kamera_(2,_4) + Kamera_(3,_4) + Kamera_(4,_4) >= 1
Pokrycie_kontenera_2_2: Kamera_(0,_2) + Kamera_(1,_2) + Kamera_(2,_0)
 + Kamera_(2,_1) + Kamera_(2,_3) + Kamera_(2,_4) + Kamera_(3,_2)
 + Kamera_(4,_2) >= 1
Pokrycie_kontenera_4_1: Kamera_(0,_1) + Kamera_(1,_1) + Kamera_(2,_1)
 + Kamera_(3,_1) + Kamera_(4,_0) + Kamera_(4,_2) + Kamera_(4,_4) >= 1
Pokrycie_kontenera_4_3: Kamera_(0,_3) + Kamera_(1,_3) + Kamera_(2,_3)
 + Kamera_(3,_3) + Kamera_(4,_0) + Kamera_(4,_2) + Kamera_(4,_4) >= 1
Binaries
Kamera_(0,_1)
Kamera_(0,_2)
Kamera_(0,_3)
Kamera_(1,_0)
Kamera_(1,_1)
Kamera_(1,_2)
Kamera_(1,_3)
Kamera_(1,_4)
Kamera_(2,_0)
Kamera_(2,_1)
Kamera_(2,_3)
Kamera_(2,_4)
Kamera_(3,_0)
Kamera_(3,_1)
Kamera_(3,_2)
Kamera_(3,_3)
Kamera_(3,_4)
Kamera_(4,_0)
Kamera_(4,_2)
Kamera_(4,_4)
End
