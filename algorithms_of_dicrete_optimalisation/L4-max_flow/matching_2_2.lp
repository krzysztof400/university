\* MaxFlowProblem *\
Maximize
Max_Flow: f_0_1 + f_0_2 + f_0_3 + f_0_4
Subject To
Conservation_1: f_0_1 - f_1_5 - f_1_8 = 0
Conservation_2: f_0_2 - f_2_6 - f_2_7 = 0
Conservation_3: f_0_3 - f_3_5 - f_3_8 = 0
Conservation_4: f_0_4 - f_4_5 - f_4_6 = 0
Conservation_5: f_1_5 + f_3_5 + f_4_5 - f_5_9 = 0
Conservation_6: f_2_6 + f_4_6 - f_6_9 = 0
Conservation_7: f_2_7 - f_7_9 = 0
Conservation_8: f_1_8 + f_3_8 - f_8_9 = 0
Bounds
 f_0_1 <= 1
 f_0_2 <= 1
 f_0_3 <= 1
 f_0_4 <= 1
 f_1_5 <= 1
 f_1_8 <= 1
 f_2_6 <= 1
 f_2_7 <= 1
 f_3_5 <= 1
 f_3_8 <= 1
 f_4_5 <= 1
 f_4_6 <= 1
 f_5_9 <= 1
 f_6_9 <= 1
 f_7_9 <= 1
 f_8_9 <= 1
End
