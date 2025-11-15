import json
import pulp
from pulp import LpProblem, LpVariable, LpMaximize, LpMinimize, lpSum, LpStatus, value
import sys

def load_data(filename):
    with open(filename, 'r', encoding='utf-8') as f:
        data = json.load(f)
    return data

def solve_and_save(model, solver_name="CBC", lp_filename="model.lp", solver_msg=True):
    model.writeLP(lp_filename)

    if solver_name.upper() == "GLPK":
        solver = pulp.GLPK_CMD(msg=solver_msg)
    elif solver_name.upper() == "CBC":
        solver = pulp.PULP_CBC_CMD(msg=solver_msg)
    else:
        solver = None

    print(f"--- Solving model {model.name} ---")
    model.solve(solver)

    print("Status:", LpStatus[model.status])
    print("\nVariables:")
    for v in model.variables():
        if v.varValue > 1e-6:
            print(v.name, "=", v.varValue)
    
    print("\nResult (objective):")
    print(f"{model.objective.name} = {value(model.objective)}")
    print(f"--- End of model {model.name} ---")

def build_model_task1(data):
    model = LpProblem(name="Minimize_fuel_costs", sense=LpMinimize)

    suppliers = data["suppliers"]
    airports = data["airports"]
    costs = data["costs"]
    supply = data["supply"]
    demand = data["demand"]

    zmienne = LpVariable.dicts("Fuel", 
                               [(d, l) for d in suppliers for l in airports], 
                               lowBound=0, 
                               cat='Continuous')

    obj = lpSum(zmienne[(d, l)] * costs[l][d] 
                for d in suppliers for l in airports)
    model += obj, "Total_delivery_cost"

    for d in suppliers:
        model += lpSum(zmienne[(d, l)] for l in airports) <= supply[d], f"Supply_supplier_{d}"

    for l in airports:
        model += lpSum(zmienne[(d, l)] for d in suppliers) == demand[l], f"Airport_demand_{l}"

    return model, zmienne

def build_model_task2(data):
    model = LpProblem(name="Maximize_production_profit", sense=LpMaximize)
    
    produkty = data["products"]
    maszyny = data["machines"]
    czas_produkcji = data["production_time"]

    nazwy_produktow = list(produkty.keys())
    nazwy_maszyn = list(maszyny.keys())

    vars = {}
    for p_nazwa, p_info in produkty.items():
        vars[p_nazwa] = LpVariable(name=p_nazwa, 
                                   lowBound=p_info.get("lb", 0), 
                                   upBound=p_info.get("ub", None), 
                                   cat='Continuous')

    czas_pracy_maszyn = {m_nazwa: 0 for m_nazwa in nazwy_maszyn}
    for m_idx, m_nazwa in enumerate(nazwy_maszyn):
        czas_pracy_maszyn[m_nazwa] = lpSum(
            vars[p_nazwa] * czas_produkcji[p_idx][m_idx] 
            for p_idx, p_nazwa in enumerate(nazwy_produktow)
        )

    zysk_jednostkowy = {p: p_info["price"] - p_info["materials_cost"] 
                        for p, p_info in produkty.items()}

    calkowity_koszt_maszyn = lpSum(
        (maszyny[m_nazwa]["running_cost"] / 60.0) * czas_pracy_maszyn[m_nazwa] 
        for m_nazwa in nazwy_maszyn
    )

    obj = lpSum(zysk_jednostkowy[p] * vars[p] for p in nazwy_produktow) - calkowity_koszt_maszyn
    model += obj, "Total_Profit"

    for m_nazwa, m_info in maszyny.items():
        model += czas_pracy_maszyn[m_nazwa] <= m_info["availability_time"], f"Machine_work_time_{m_nazwa}"

    return model, vars

def build_model_task3(data):
    model = LpProblem(name="Minimize_production_and_inventory_costs", sense=LpMinimize)

    periods = data["periods"]
    period_data = data["period_data"]
    
    inventory_capacity = data["inventory_capacity"]
    holding_cost_per_unit = data["holding_cost_per_unit"]
    initial_inventory = data["initial_inventory"]
    normal_production_capacity = data["normal_production_capacity"]


    prod_normalna = LpVariable.dicts("NormalProduction", periods, lowBound=0, cat='Continuous')
    prod_nadwym = LpVariable.dicts("OvertimeProduction", periods, lowBound=0, cat='Continuous')
    magazyn = LpVariable.dicts("Inventory", [0] + periods, lowBound=0, cat='Continuous')

    model += magazyn[0] == initial_inventory, "Inventory_start"

    koszt_prod = lpSum(
        prod_normalna[j] * period_data[str(j)]["normal_cost"] + 
        prod_nadwym[j] * period_data[str(j)]["overtime_cost"]
        for j in periods
    )
    koszt_magaz = lpSum(
        magazyn[j] * holding_cost_per_unit
        for j in periods
    )
    
    model += koszt_prod + koszt_magaz, "Total_cost"

    for j in periods:
        js = str(j)

        model += (magazyn[j-1] + prod_normalna[j] + prod_nadwym[j] - period_data[js]["demand"] 
                  == magazyn[j]), f"Inventory_balance_period_{j}"

        model += prod_normalna[j] <= normal_production_capacity, f"Normal_prod_limit_{j}"

        model += prod_nadwym[j] <= period_data[js]["overtime_limit"], f"Overtime_prod_limit_{j}"
        
        model += magazyn[j] <= inventory_capacity, f"Inventory_limit_{j}"

    return model, {"prod_normalna": prod_normalna, "prod_nadwym": prod_nadwym, "magazyn": magazyn}

def build_model_task4(data):
    model = LpProblem(name="Shortest_path_with_time_limit", sense=LpMinimize)

    nodes = data["nodes"]
    arcs = data["arcs"]
    start = data["start"]
    koniec = data["end"]
    time_limit = data["time_limit"]

    arc_data = {}
    for arc in arcs:
        if len(arc) == 4:
            from_node, to_node, cost, time = arc
            arc_data[(from_node, to_node)] = {"cost": cost, "time": time}
        elif len(arc) == 5:
            _name, from_node, to_node, cost, time = arc
            arc_data[(from_node, to_node)] = {"cost": cost, "time": time}

    x = LpVariable.dicts("Path", arc_data.keys(), cat='Binary')

    model += lpSum(arc_data[luk]["cost"] * x[luk] for luk in arc_data), "Total_path_cost"

    model += lpSum(arc_data[luk]["time"] * x[luk] for luk in arc_data) <= time_limit, "Time_limit"

    for w in nodes:
        flow_in = lpSum(x[(i, j)] for (i, j) in arc_data if j == w)
        flow_out = lpSum(x[(i, j)] for (i, j) in arc_data if i == w)

        if w == start:
            model += flow_out - flow_in == 1, f"Flow_start_{w}"
        elif w == koniec:
            model += flow_in - flow_out == 1, f"Flow_end_{w}"
        else:
            model += flow_in - flow_out == 0, f"Flow_intermediate_{w}"

    return model, x

def build_model_task5(data):
    model = LpProblem(name="Minimize_number_of_patrol_cars", sense=LpMinimize)

    districts = data["districts"]
    shifts = data["shifts"]

    zmienne = {}
    for d in districts:
        for z in shifts:
            min_val = data["min_cars"][d][z]
            max_val = data["max_cars"][d][z]
            zmienne[(d, z)] = LpVariable(name=f"PatrolCars_{d}_{z}",
                                         lowBound=min_val,
                                         upBound=max_val,
                                         cat='Integer')

    model += lpSum(zmienne[(d, z)] for d in districts for z in shifts), "Total_number_of_assignments"

    for z in shifts:
        model += lpSum(zmienne[(d, z)] for d in districts) >= data["min_total_shift"][z], f"Min_total_shift_{z}"
    
    for d in districts:
        model += lpSum(zmienne[(d, z)] for z in shifts) >= data["min_total_district"][d], f"Min_total_district_{d}"

    return model, zmienne

def build_model_task6(data, k):
    m = data["m"]
    n = data["n"]
    kontenery = [tuple(k) for k in data["containers"]]
    
    model = LpProblem(name=f"Camera_placement_k={k}", sense=LpMinimize)

    positions = [(r, c) for r in range(m) for c in range(n)]
    camera_positions = [p for p in positions if p not in kontenery]
    
    cam = LpVariable.dicts("Camera", camera_positions, cat='Binary')

    model += lpSum(cam[p] for p in camera_positions), "Number_of_cameras"

    for (kr, kc) in kontenery:
        visible_cameras = []
        for (pr, pc) in camera_positions:
            if (pr == kr and abs(pc - kc) <= k) or (pc == kc and abs(pr - kr) <= k):
                visible_cameras.append(cam[(pr, pc)])
        
        if visible_cameras:
            model += lpSum(visible_cameras) >= 1, f"Container_coverage_{kr}_{kc}"
        else:
            print(f"WARNING: Container at ({kr},{kc}) cannot be seen by any camera!")
            model += 1 == 0, f"Unfeasible_coverage_{kr}_{kc}"

    return model, cam

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python solver.py <task_number>")
        print("Available tasks: 1, 2, 3, 4a, 4b, 5, 6")
        sys.exit(1)

    numer_zadania_str = sys.argv[1]
    
    if numer_zadania_str == "1":
        data_file = "data_ex1.json"
        data = load_data(data_file)
        prob, vars_ = build_model_task1(data)
        nazwa_pliku_lp = "model_ex1.lp"

    elif numer_zadania_str == "2":
        data_file = "data_ex2.json"
        data = load_data(data_file)
        prob, vars_ = build_model_task2(data)
        nazwa_pliku_lp = "model_ex2.lp"

    elif numer_zadania_str == "3":
        data_file = "data_ex3.json"
        data = load_data(data_file)
        prob, vars_ = build_model_task3(data)
        nazwa_pliku_lp = "model_ex3.lp"

    elif numer_zadania_str == "4a":
        data_file = "data_ex4a.json"
        data = load_data(data_file)
        prob, vars_ = build_model_task4(data)
        nazwa_pliku_lp = "model_ex4a.lp"
        
    elif numer_zadania_str == "4b":
        data_file = "data_ex4b.json"
        data = load_data(data_file)
        prob, vars_ = build_model_task4(data)
        nazwa_pliku_lp = "model_ex4b.lp"
        
    elif numer_zadania_str == "5":
        data_file = "data_ex5.json"
        data = load_data(data_file)
        prob, vars_ = build_model_task5(data)
        nazwa_pliku_lp = "model_ex5.lp"
        
    elif numer_zadania_str == "6":
        data_file = "data_ex6.json"
        data = load_data(data_file)
        k_zasięg = 1
        if len(sys.argv) >= 3:
            try:
                k_zasięg = int(sys.argv[2])
            except ValueError:
                print("Second argument (range k) must be an integer.")
                sys.exit(1)
        prob, vars_ = build_model_task6(data, k=k_zasięg)
        nazwa_pliku_lp = f"model_ex6_k={k_zasięg}.lp"
    else:
        print(f"Unknown task number: {numer_zadania_str}")
        sys.exit(1)

    solve_and_save(prob, solver_name="CBC", lp_filename=nazwa_pliku_lp, solver_msg=True)
