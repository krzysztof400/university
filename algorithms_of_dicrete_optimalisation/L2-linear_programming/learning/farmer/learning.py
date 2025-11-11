import pulp
from pulp import LpProblem, LpVariable, LpMaximize, LpStatus, value

model = LpProblem(name="Farmer", sense=LpMaximize)

ziemniaki = LpVariable(name="Ziemniaki", lowBound=0, upBound=500)
marchewki = LpVariable(name="Marchew", lowBound=0, upBound=200)

model += (ziemniaki * 1.5 + marchewki * 3, "zarobek")

model += (ziemniaki + marchewki <= 300, "dostępny nawóz")

status = model.solve()

print(f"\n\nProblem Status: {LpStatus[status]}")
print(f"Optimal number of Product A: {value(ziemniaki)}")
print(f"Optimal number of Product B: {value(marchewki)}")
print(f"Total Optimal Profit: ${value(model.objective)}")