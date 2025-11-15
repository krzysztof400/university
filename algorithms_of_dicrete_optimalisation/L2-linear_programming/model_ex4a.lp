\* Shortest_path_with_time_limit *\
Minimize
Total_path_cost: 3 Path_(1,_2) + 4 Path_(1,_3) + 7 Path_(1,_4) + 8 Path_(1,_5)
 + 2 Path_(2,_3) + 6 Path_(3,_10) + 4 Path_(3,_4) + 2 Path_(3,_5)
 + Path_(4,_5) + 3 Path_(4,_7) + 5 Path_(5,_10) + 5 Path_(5,_6)
 + 3 Path_(5,_7) + 5 Path_(6,_1) + 7 Path_(6,_10) + 2 Path_(6,_7)
 + 4 Path_(7,_3) + 3 Path_(7,_8) + Path_(7,_9) + Path_(8,_9) + 2 Path_(9,_10)
Subject To
Flow_end_10: Path_(3,_10) + Path_(5,_10) + Path_(6,_10) + Path_(9,_10) = 1
Flow_intermediate_2: Path_(1,_2) - Path_(2,_3) = 0
Flow_intermediate_3: Path_(1,_3) + Path_(2,_3) - Path_(3,_10) - Path_(3,_4)
 - Path_(3,_5) + Path_(7,_3) = 0
Flow_intermediate_4: Path_(1,_4) + Path_(3,_4) - Path_(4,_5) - Path_(4,_7) = 0
Flow_intermediate_5: Path_(1,_5) + Path_(3,_5) + Path_(4,_5) - Path_(5,_10)
 - Path_(5,_6) - Path_(5,_7) = 0
Flow_intermediate_6: Path_(5,_6) - Path_(6,_1) - Path_(6,_10) - Path_(6,_7)
 = 0
Flow_intermediate_7: Path_(4,_7) + Path_(5,_7) + Path_(6,_7) - Path_(7,_3)
 - Path_(7,_8) - Path_(7,_9) = 0
Flow_intermediate_8: Path_(7,_8) - Path_(8,_9) = 0
Flow_intermediate_9: Path_(7,_9) + Path_(8,_9) - Path_(9,_10) = 0
Flow_start_1: Path_(1,_2) + Path_(1,_3) + Path_(1,_4) + Path_(1,_5)
 - Path_(6,_1) = 1
Time_limit: 4 Path_(1,_2) + 9 Path_(1,_3) + 10 Path_(1,_4) + 12 Path_(1,_5)
 + 3 Path_(2,_3) + 11 Path_(3,_10) + 6 Path_(3,_4) + 2 Path_(3,_5)
 + Path_(4,_5) + 5 Path_(4,_7) + 8 Path_(5,_10) + 6 Path_(5,_6)
 + 3 Path_(5,_7) + 8 Path_(6,_1) + 11 Path_(6,_10) + 2 Path_(6,_7)
 + 6 Path_(7,_3) + 5 Path_(7,_8) + Path_(7,_9) + 2 Path_(8,_9)
 + 2 Path_(9,_10) <= 15
Binaries
Path_(1,_2)
Path_(1,_3)
Path_(1,_4)
Path_(1,_5)
Path_(2,_3)
Path_(3,_10)
Path_(3,_4)
Path_(3,_5)
Path_(4,_5)
Path_(4,_7)
Path_(5,_10)
Path_(5,_6)
Path_(5,_7)
Path_(6,_1)
Path_(6,_10)
Path_(6,_7)
Path_(7,_3)
Path_(7,_8)
Path_(7,_9)
Path_(8,_9)
Path_(9,_10)
End
