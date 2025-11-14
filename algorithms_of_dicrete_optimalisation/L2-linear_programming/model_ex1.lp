\* Minimalizacja_kosztow_paliwa *\
Minimize
Calkowity_koszt_dostaw: 10 Paliwo_('F1',_'L1') + 10 Paliwo_('F1',_'L2')
 + 9 Paliwo_('F1',_'L3') + 11 Paliwo_('F1',_'L4') + 7 Paliwo_('F2',_'L1')
 + 11 Paliwo_('F2',_'L2') + 12 Paliwo_('F2',_'L3') + 13 Paliwo_('F2',_'L4')
 + 8 Paliwo_('F3',_'L1') + 14 Paliwo_('F3',_'L2') + 4 Paliwo_('F3',_'L3')
 + 9 Paliwo_('F3',_'L4')
Subject To
Podaż_dostawcy_F1: Paliwo_('F1',_'L1') + Paliwo_('F1',_'L2')
 + Paliwo_('F1',_'L3') + Paliwo_('F1',_'L4') <= 275000
Podaż_dostawcy_F2: Paliwo_('F2',_'L1') + Paliwo_('F2',_'L2')
 + Paliwo_('F2',_'L3') + Paliwo_('F2',_'L4') <= 550000
Podaż_dostawcy_F3: Paliwo_('F3',_'L1') + Paliwo_('F3',_'L2')
 + Paliwo_('F3',_'L3') + Paliwo_('F3',_'L4') <= 660000
Popyt_lotniska_L1: Paliwo_('F1',_'L1') + Paliwo_('F2',_'L1')
 + Paliwo_('F3',_'L1') = 110000
Popyt_lotniska_L2: Paliwo_('F1',_'L2') + Paliwo_('F2',_'L2')
 + Paliwo_('F3',_'L2') = 220000
Popyt_lotniska_L3: Paliwo_('F1',_'L3') + Paliwo_('F2',_'L3')
 + Paliwo_('F3',_'L3') = 330000
Popyt_lotniska_L4: Paliwo_('F1',_'L4') + Paliwo_('F2',_'L4')
 + Paliwo_('F3',_'L4') = 440000
End
