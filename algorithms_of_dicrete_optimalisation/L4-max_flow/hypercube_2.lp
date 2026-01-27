\* MaxFlowProblem *\
Maximize
Max_Flow: f_0_1 + f_0_2
Subject To
Conservation_1: f_0_1 - f_1_3 = 0
Conservation_2: f_0_2 - f_2_3 = 0
Bounds
 f_0_1 <= 4
 f_0_2 <= 1
 f_1_3 <= 3
 f_2_3 <= 3
End
