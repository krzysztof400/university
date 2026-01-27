\* MaxFlowProblem *\
Maximize
Max_Flow: f_0_1 + f_0_2 + f_0_3 + f_0_4 + f_0_5 + f_0_6 + f_0_7 + f_0_8
Subject To
Conservation_1: f_0_1 - f_1_11 - f_1_15 = 0
Conservation_10: - f_10_17 + f_6_10 = 0
Conservation_11: - f_11_17 + f_1_11 + f_3_11 + f_8_11 = 0
Conservation_12: - f_12_17 = 0
Conservation_13: - f_13_17 + f_2_13 = 0
Conservation_14: - f_14_17 + f_4_14 + f_5_14 + f_8_14 = 0
Conservation_15: - f_15_17 + f_1_15 + f_2_15 + f_3_15 = 0
Conservation_16: - f_16_17 + f_5_16 + f_6_16 + f_7_16 = 0
Conservation_2: f_0_2 - f_2_13 - f_2_15 = 0
Conservation_3: f_0_3 - f_3_11 - f_3_15 = 0
Conservation_4: f_0_4 - f_4_14 - f_4_9 = 0
Conservation_5: f_0_5 - f_5_14 - f_5_16 = 0
Conservation_6: f_0_6 - f_6_10 - f_6_16 = 0
Conservation_7: f_0_7 - f_7_16 - f_7_9 = 0
Conservation_8: f_0_8 - f_8_11 - f_8_14 = 0
Conservation_9: f_4_9 + f_7_9 - f_9_17 = 0
Bounds
 f_0_1 <= 1
 f_0_2 <= 1
 f_0_3 <= 1
 f_0_4 <= 1
 f_0_5 <= 1
 f_0_6 <= 1
 f_0_7 <= 1
 f_0_8 <= 1
 f_10_17 <= 1
 f_11_17 <= 1
 f_12_17 <= 1
 f_13_17 <= 1
 f_14_17 <= 1
 f_15_17 <= 1
 f_16_17 <= 1
 f_1_11 <= 1
 f_1_15 <= 1
 f_2_13 <= 1
 f_2_15 <= 1
 f_3_11 <= 1
 f_3_15 <= 1
 f_4_14 <= 1
 f_4_9 <= 1
 f_5_14 <= 1
 f_5_16 <= 1
 f_6_10 <= 1
 f_6_16 <= 1
 f_7_16 <= 1
 f_7_9 <= 1
 f_8_11 <= 1
 f_8_14 <= 1
 f_9_17 <= 1
End
