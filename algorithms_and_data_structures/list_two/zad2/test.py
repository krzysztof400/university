import matplotlib.pyplot as plt
import re
import subprocess
import sys


def parse_output(output):
    comparisons_match = re.search(r'Number of comparisons: (\d+)', output)
    swaps_match = re.search(r'Number of swaps: (\d+)', output)
    
    if comparisons_match and swaps_match:
        comparisons = int(comparisons_match.group(1))
        swaps = int(swaps_match.group(1))
        return comparisons, swaps
    else:
        return None

def sort_n_elements(n, generator_path, sort_path):
    cpp_output = subprocess.run(
        [generator_path, str(n)], 
        stdout=subprocess.PIPE, 
        text=True
    ).stdout
    cpp_output += subprocess.run(
        [sort_path], 
        input=cpp_output, 
        stdout=subprocess.PIPE, 
        text=True
    ).stdout

    result = parse_output(cpp_output)
    if result:
        comparisons, swaps = result
        return comparisons, swaps
    else:
        print("Failed to parse output.")

if len(sys.argv) > 1:
    k = int(sys.argv[1])
else:
    k = 10

generators = ["./random_generator", "./descending_generator", "./ascending_generator"]
sorts = ["./quick_sort", "./insertion_sort"]

comparisons = []
swaps = []

for i in range(1, 100):  # Loop up to 1000
    n = i * 10
    for generator in generators:
        for sort in sorts:
            if n > 100 and sort == "./insertion_sort":
                continue
            result = (0, 0)
            for i in range(k+1):
                comparisons_swaps = sort_n_elements(n, generator, sort)
                if comparisons_swaps:
                    result = (result[0] + comparisons_swaps[0], result[1] + comparisons_swaps[1])
            result = (result[0] / k, result[1] / k)
            if result:
                comp, swp = result
                comparisons.append((n, generator, sort, comp))
                swaps.append((n, generator, sort, swp))

# Convert data to separate lists for plotting
x_vals = [c[0] for c in comparisons]  # Number of elements
y_vals = [c[3] for c in comparisons]  # Comparisons

plt.figure(figsize=(10, 5))

# Subplot 1: Comparisons
plt.subplot(1, 2, 1)
plt.title("Number of comparisons")
plt.xlabel("Number of elements")
plt.ylabel("Comparisons")

# Plot each generator/sort combination
for sort in sorts:
    for generator in generators:
        x_plot = [c[0] for c in comparisons if c[1] == generator and c[2] == sort]
        y_plot = [c[3] for c in comparisons if c[1] == generator and c[2] == sort]
        plt.plot(x_plot, y_plot, label=f"{sort} - {generator}")

plt.legend(loc='upper left', bbox_to_anchor=(1, 1))

plt.show()
