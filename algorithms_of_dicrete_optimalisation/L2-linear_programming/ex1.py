import json
import pulp
from pulp import LpProblem, LpVariable, LpMaximize, lpSum, LpStatus, value

def load_data(filename):
    with open(filename, 'r') as f:
        data = json.load(f)
    return data

def build_model(data):
    model = LpProblem(name="What companies to buy fuel from?", sense=LpMaximize)
    vars = {}
    prices = {}
    material_cost = {}
    machine_hours = {mname: 0 for mname in data["machines"]}

    for vname, vinfo in data["products"].items():
        vars[vname] = LpVariable(name=vname, lowBound=vinfo.get("lb", 0), upBound=vinfo.get("ub", None))
        prices[vname] = vinfo["price"]
        material_cost[vname] = vinfo["materials_cost"]
        for m_idx, mname in enumerate(data["machines"]):
            machine_hours[mname] += vars[vname] * data["production_time"][list(data["products"].keys()).index(vname)][m_idx]
        
    print(machine_hours)

    obj = sum(prices[v] * vars[v] for v in vars) - sum(material_cost[v] * vars[v] for v in vars) - sum((data["machines"][mname]["running_cost"]/60) * machine_hours[mname] for mname in data["machines"])

    model += obj, "Total_Profit"

    for aname, ainfo in data["airports"].items():
        model += (machine_hours[aname] <= minfo["demand"]), f"Airport_{aname}_Demand"

    return model, vars

def solve_and_save(model, solver_name="CBC", lp_filename="model.lp", solver_msg=True):
    model.writeLP(lp_filename)

    if solver_name.upper() == "GLPK":
        solver = pulp.GLPK_CMD(msg=solver_msg)
    elif solver_name.upper() == "CBC":
        solver = pulp.PULP_CBC_CMD(msg=solver_msg)
    else:
        solver = None

    model.solve(solver)

    print("Status:", LpStatus[model.status])
    for v in model.variables():
        print(v.name, "=", v.varValue)
    print("Objective =", value(model.objective))

if __name__ == "__main__":
    data = load_data("data_ex2.json")
    prob, vars_ = build_model(data)
    solve_and_save(prob, solver_name="CBC", lp_filename="ex2.lp", solver_msg=True)
