\* Minimize_fuel_costs *\
Minimize
Total_delivery_cost: 10 Fuel_('F1',_'L1') + 10 Fuel_('F1',_'L2')
 + 9 Fuel_('F1',_'L3') + 11 Fuel_('F1',_'L4') + 7 Fuel_('F2',_'L1')
 + 11 Fuel_('F2',_'L2') + 12 Fuel_('F2',_'L3') + 13 Fuel_('F2',_'L4')
 + 8 Fuel_('F3',_'L1') + 14 Fuel_('F3',_'L2') + 4 Fuel_('F3',_'L3')
 + 9 Fuel_('F3',_'L4')
Subject To
Airport_demand_L1: Fuel_('F1',_'L1') + Fuel_('F2',_'L1') + Fuel_('F3',_'L1')
 = 110000
Airport_demand_L2: Fuel_('F1',_'L2') + Fuel_('F2',_'L2') + Fuel_('F3',_'L2')
 = 220000
Airport_demand_L3: Fuel_('F1',_'L3') + Fuel_('F2',_'L3') + Fuel_('F3',_'L3')
 = 330000
Airport_demand_L4: Fuel_('F1',_'L4') + Fuel_('F2',_'L4') + Fuel_('F3',_'L4')
 = 440000
Supply_supplier_F1: Fuel_('F1',_'L1') + Fuel_('F1',_'L2') + Fuel_('F1',_'L3')
 + Fuel_('F1',_'L4') <= 275000
Supply_supplier_F2: Fuel_('F2',_'L1') + Fuel_('F2',_'L2') + Fuel_('F2',_'L3')
 + Fuel_('F2',_'L4') <= 550000
Supply_supplier_F3: Fuel_('F3',_'L1') + Fuel_('F3',_'L2') + Fuel_('F3',_'L3')
 + Fuel_('F3',_'L4') <= 660000
End
