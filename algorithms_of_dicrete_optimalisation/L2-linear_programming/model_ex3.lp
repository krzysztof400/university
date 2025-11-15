\* Minimize_production_and_inventory_costs *\
Minimize
Total_cost: 1500 Inventory_1 + 1500 Inventory_2 + 1500 Inventory_3
 + 1500 Inventory_4 + 6000 NormalProduction_1 + 4000 NormalProduction_2
 + 8000 NormalProduction_3 + 9000 NormalProduction_4
 + 8000 OvertimeProduction_1 + 6000 OvertimeProduction_2
 + 10000 OvertimeProduction_3 + 11000 OvertimeProduction_4
Subject To
Inventory_balance_period_1: Inventory_0 - Inventory_1 + NormalProduction_1
 + OvertimeProduction_1 = 130
Inventory_balance_period_2: Inventory_1 - Inventory_2 + NormalProduction_2
 + OvertimeProduction_2 = 80
Inventory_balance_period_3: Inventory_2 - Inventory_3 + NormalProduction_3
 + OvertimeProduction_3 = 125
Inventory_balance_period_4: Inventory_3 - Inventory_4 + NormalProduction_4
 + OvertimeProduction_4 = 195
Inventory_limit_1: Inventory_1 <= 70
Inventory_limit_2: Inventory_2 <= 70
Inventory_limit_3: Inventory_3 <= 70
Inventory_limit_4: Inventory_4 <= 70
Inventory_start: Inventory_0 = 15
Normal_prod_limit_1: NormalProduction_1 <= 100
Normal_prod_limit_2: NormalProduction_2 <= 100
Normal_prod_limit_3: NormalProduction_3 <= 100
Normal_prod_limit_4: NormalProduction_4 <= 100
Overtime_prod_limit_1: OvertimeProduction_1 <= 60
Overtime_prod_limit_2: OvertimeProduction_2 <= 65
Overtime_prod_limit_3: OvertimeProduction_3 <= 70
Overtime_prod_limit_4: OvertimeProduction_4 <= 60
End
