import json
import pulp

def load_data(path="data.json"):
    with open(path, "r", encoding="utf-8") as f:
        return json.load(f)

def build_model(data):
    # Tworzymy problem
    sense = pulp.LpMaximize if data["objective"]["sense"].lower().startswith("m") else pulp.LpMinimize
    prob = pulp.LpProblem(data.get("problem_name","Problem"), sense)

    # Zmienne: słownik name -> LpVariable
    vars_ = {}
    for vname, vinfo in data["variables"].items():
        lb = vinfo.get("lb", None)
        ub = vinfo.get("ub", None)
        # PuLP oczekuje None lub numeric
        vars_[vname] = pulp.LpVariable(vname, lowBound=lb, upBound=ub)

    # Funkcja celu
    obj = sum(data["objective"]["coefs"].get(v, 0) * vars_[v] for v in vars_)
    prob += obj, "Objective"

    # Ograniczenia
    for c in data.get("constraints", []):
        lhs = sum(c["expr"].get(v, 0) * vars_[v] for v in vars_)
        rhs = c["rhs"]
        sense = c.get("sense", "<=")
        # dodajemy z etykietą
        if sense == "<=":
            prob += (lhs <= rhs), c.get("name", None)
        elif sense == ">=":
            prob += (lhs >= rhs), c.get("name", None)
        elif sense == "=" or sense == "==":
            prob += (lhs == rhs), c.get("name", None)
        else:
            raise ValueError("Unsupported sense: " + str(sense))

    return prob, vars_

def solve_and_save(prob, solver_name="CBC", lp_filename="model.lp", solver_msg=True):
    # Zapisz plik .lp (czytelny)
    prob.writeLP(lp_filename)

    # Wybór solvera
    if solver_name.upper() == "GLPK":
        solver = pulp.GLPK_CMD(msg=solver_msg)
    elif solver_name.upper() == "CBC":
        solver = pulp.PULP_CBC_CMD(msg=solver_msg)
    else:
        # fallback: domyślny solver PuLP (zwykle CBC)
        solver = None

    # solve (jeśli solver==None to pulp użyje domyślnego)
    prob.solve(solver)

    # Wypisz wyniki
    print("Status:", pulp.LpStatus[prob.status])
    for v in prob.variables():
        print(v.name, "=", v.varValue)
    print("Objective =", pulp.value(prob.objective))

if __name__ == "__main__":
    data = load_data("data.json")
    prob, vars_ = build_model(data)
    solve_and_save(prob, solver_name="CBC", lp_filename="farmer.lp", solver_msg=True)
