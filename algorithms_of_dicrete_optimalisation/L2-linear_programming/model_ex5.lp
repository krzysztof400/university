\* Minimize_number_of_patrol_cars *\
Minimize
Total_number_of_assignments: PatrolCars_p1_z1 + PatrolCars_p1_z2
 + PatrolCars_p1_z3 + PatrolCars_p2_z1 + PatrolCars_p2_z2 + PatrolCars_p2_z3
 + PatrolCars_p3_z1 + PatrolCars_p3_z2 + PatrolCars_p3_z3
Subject To
Min_total_district_p1: PatrolCars_p1_z1 + PatrolCars_p1_z2 + PatrolCars_p1_z3
 >= 10
Min_total_district_p2: PatrolCars_p2_z1 + PatrolCars_p2_z2 + PatrolCars_p2_z3
 >= 14
Min_total_district_p3: PatrolCars_p3_z1 + PatrolCars_p3_z2 + PatrolCars_p3_z3
 >= 13
Min_total_shift_z1: PatrolCars_p1_z1 + PatrolCars_p2_z1 + PatrolCars_p3_z1
 >= 10
Min_total_shift_z2: PatrolCars_p1_z2 + PatrolCars_p2_z2 + PatrolCars_p3_z2
 >= 20
Min_total_shift_z3: PatrolCars_p1_z3 + PatrolCars_p2_z3 + PatrolCars_p3_z3
 >= 18
Bounds
 2 <= PatrolCars_p1_z1 <= 3
 4 <= PatrolCars_p1_z2 <= 7
 3 <= PatrolCars_p1_z3 <= 5
 3 <= PatrolCars_p2_z1 <= 5
 6 <= PatrolCars_p2_z2 <= 7
 5 <= PatrolCars_p2_z3 <= 10
 5 <= PatrolCars_p3_z1 <= 8
 7 <= PatrolCars_p3_z2 <= 12
 6 <= PatrolCars_p3_z3 <= 10
Generals
PatrolCars_p1_z1
PatrolCars_p1_z2
PatrolCars_p1_z3
PatrolCars_p2_z1
PatrolCars_p2_z2
PatrolCars_p2_z3
PatrolCars_p3_z1
PatrolCars_p3_z2
PatrolCars_p3_z3
End
