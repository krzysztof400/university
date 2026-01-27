\* MaxFlowProblem *\
Maximize
Max_Flow: f_0_1 + f_0_2 + f_0_4 + f_0_8
Subject To
Conservation_1: f_0_1 - f_1_3 - f_1_5 - f_1_9 = 0
Conservation_10: - f_10_11 - f_10_14 + f_2_10 + f_8_10 = 0
Conservation_11: f_10_11 - f_11_15 + f_3_11 + f_9_11 = 0
Conservation_12: - f_12_13 - f_12_14 + f_4_12 + f_8_12 = 0
Conservation_13: f_12_13 - f_13_15 + f_5_13 + f_9_13 = 0
Conservation_14: f_10_14 + f_12_14 - f_14_15 + f_6_14 = 0
Conservation_2: f_0_2 - f_2_10 - f_2_3 - f_2_6 = 0
Conservation_3: f_1_3 + f_2_3 - f_3_11 - f_3_7 = 0
Conservation_4: f_0_4 - f_4_12 - f_4_5 - f_4_6 = 0
Conservation_5: f_1_5 + f_4_5 - f_5_13 - f_5_7 = 0
Conservation_6: f_2_6 + f_4_6 - f_6_14 - f_6_7 = 0
Conservation_7: f_3_7 + f_5_7 + f_6_7 - f_7_15 = 0
Conservation_8: f_0_8 - f_8_10 - f_8_12 - f_8_9 = 0
Conservation_9: f_1_9 + f_8_9 - f_9_11 - f_9_13 = 0
Bounds
 f_0_1 <= 4
 f_0_2 <= 15
 f_0_4 <= 10
 f_0_8 <= 11
 f_10_11 <= 8
 f_10_14 <= 8
 f_11_15 <= 5
 f_12_13 <= 3
 f_12_14 <= 3
 f_13_15 <= 14
 f_14_15 <= 15
 f_1_3 <= 7
 f_1_5 <= 1
 f_1_9 <= 3
 f_2_10 <= 2
 f_2_3 <= 5
 f_2_6 <= 4
 f_3_11 <= 4
 f_3_7 <= 3
 f_4_12 <= 3
 f_4_5 <= 3
 f_4_6 <= 4
 f_5_13 <= 2
 f_5_7 <= 4
 f_6_14 <= 1
 f_6_7 <= 7
 f_7_15 <= 12
 f_8_10 <= 7
 f_8_12 <= 2
 f_8_9 <= 2
 f_9_11 <= 4
 f_9_13 <= 7
End
