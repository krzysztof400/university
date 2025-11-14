\* Which_products_to_produce? *\
Maximize
Total_Profit: 4.2 p1 + 5.5 p2 + 4.55 p3 + 3.75 p4
Subject To
Machine_m1_Capacity: 5 p1 + 3 p2 + 4 p3 + 4 p4 <= 3600
Machine_m2_Capacity: 10 p1 + 6 p2 + 5 p3 + 2 p4 <= 3600
Machine_m3_Capacity: 6 p1 + 4 p2 + 3 p3 + p4 <= 3600
Bounds
 p1 <= 400
 p2 <= 100
 p3 <= 150
 p4 <= 500
End
