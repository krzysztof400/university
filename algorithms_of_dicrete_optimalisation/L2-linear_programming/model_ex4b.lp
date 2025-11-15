\* Shortest_path_with_time_limit *\
Minimize
Total_path_cost: 100 Path_(1,_10) + 2 Path_(1,_2) + 3 Path_(1,_3)
 + 6 Path_(1,_5) + 3 Path_(2,_10) + 3 Path_(3,_4) + 4 Path_(4,_10)
 + 6 Path_(5,_10)
Subject To
Flow_end_10: Path_(1,_10) + Path_(2,_10) + Path_(4,_10) + Path_(5,_10) = 1
Flow_intermediate_2: Path_(1,_2) - Path_(2,_10) = 0
Flow_intermediate_3: Path_(1,_3) - Path_(3,_4) = 0
Flow_intermediate_4: Path_(3,_4) - Path_(4,_10) = 0
Flow_intermediate_5: Path_(1,_5) - Path_(5,_10) = 0
Flow_start_1: Path_(1,_10) + Path_(1,_2) + Path_(1,_3) + Path_(1,_5) = 1
Time_limit: Path_(1,_10) + 10 Path_(1,_2) + 3 Path_(1,_3) + 6 Path_(1,_5)
 + 10 Path_(2,_10) + 3 Path_(3,_4) + 4 Path_(4,_10) + 6 Path_(5,_10) <= 15
Binaries
Path_(1,_10)
Path_(1,_2)
Path_(1,_3)
Path_(1,_5)
Path_(2,_10)
Path_(3,_4)
Path_(4,_10)
Path_(5,_10)
End
