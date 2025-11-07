import math
import numpy as np
import matplotlib.pyplot as plt

def f(x):
    """
    Defines the function f(x) = exp(x) * log(1 + exp(-x)).
    """
    try:
        return math.exp(x) * math.log(1.0 + math.exp(-x))
    except (ValueError, OverflowError):
        return float('nan')


xs = np.arange(0.0, 40.01, 0.1)
ys = [f(x) for x in xs]

print("Checking for underflow (f(x) = 0.0)...")
for x in np.arange(30.0, 40.01, 0.1):
    val = f(x)
    if val == 0.0:
        print(f"f(x) staje się 0.0 dla x ~= {x:.1f}")
        break

print("\nChecking for loss of stability (deviation from 1.0)...")
for x in np.arange(20.0, 40.01, 0.1):
    val = f(x)
    if abs(val - 1) > 0.01:
        print(f"f(x) traci stabilność dla x ~= {x:.1f}")
        break

fig, ax = plt.subplots(figsize=(10, 6))

ax.plot(xs, ys, label="f(x) = exp(x) * log(1 + exp(-x))")

ax.axhline(1.0, 
           label="Granica analityczna y = 1", 
           linestyle="--",
           color="red")

ax.set_xlabel("x")
ax.set_ylabel("f(x)")
ax.set_title("Zad. 2: Błąd anulowania (Float64)")
ax.legend(loc="lower left")

ax.grid(True, linestyle=':', alpha=0.7)

fig.savefig("zad2_plot.png")
