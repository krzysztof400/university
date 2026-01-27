\* MaxFlowProblem *\
Maximize
Max_Flow: f_0_1 + f_0_2 + f_0_4
Subject To
Conservation_1: f_0_1 - f_1_3 - f_1_5 = 0
Conservation_2: f_0_2 - f_2_3 - f_2_6 = 0
Conservation_3: f_1_3 + f_2_3 - f_3_7 = 0
Conservation_4: f_0_4 - f_4_5 - f_4_6 = 0
Conservation_5: f_1_5 + f_4_5 - f_5_7 = 0
Conservation_6: f_2_6 + f_4_6 - f_6_7 = 0
Bounds
 f_0_1 <= 6
 f_0_2 <= 8
 f_0_4 <= 1
 f_1_3 <= 1
 f_1_5 <= 3
 f_2_3 <= 1
 f_2_6 <= 1
 f_3_7 <= 8
 f_4_5 <= 4
 f_4_6 <= 1
 f_5_7 <= 1
 f_6_7 <= 2
End
