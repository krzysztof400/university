import matplotlib.pyplot as plt
import re
import subprocess

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

generators = ["./random_generator", "./descending_generator", "./ascending_generator"]
sorts = ["./quick_sort", "./insertion_sort"]

comparisons = []
swaps = []
k = 100
print("We're checking sorting algorithms for arrays of size n from 1 to 40 with k=100 iterations.")
print("Comparing sorting algorithms...")

for i in range(1, 40):  # Loop up to 1000
    n = i
    for generator in generators:
        for sort in sorts:
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

# Filter results for random generator and plot
random_comparisons = [c for c in comparisons if c[1] == "./random_generator"]
random_swaps = [s for s in swaps if s[1] == "./random_generator"]

# Separate data for quick sort and insertion sort
quick_sort_comparisons = [c for c in random_comparisons if c[2] == "./quick_sort"]
insertion_sort_comparisons = [c for c in random_comparisons if c[2] == "./insertion_sort"]

# Check where insertion sort is faster
n
for i in range(len(insertion_sort_comparisons)):
    if insertion_sort_comparisons[i][3] < quick_sort_comparisons[i][3]:
        n = insertion_sort_comparisons[i][0]
print(f"Insertion sort is faster for n={n} for random generator")

# Plot comparisons
plt.figure(figsize=(10, 5))
plt.plot(
    [c[0] for c in quick_sort_comparisons], 
    [c[3] for c in quick_sort_comparisons], 
    label="Quick Sort Comparisons"
)
plt.plot(
    [c[0] for c in insertion_sort_comparisons], 
    [c[3] for c in insertion_sort_comparisons], 
    label="Insertion Sort Comparisons"
)
plt.xlabel("Number of Elements (n)")
plt.ylabel("Number of Comparisons")
plt.title("Comparisons for Random Generator")
plt.legend()
plt.grid()
plt.savefig("comparisons_beetween_quick_and_insertion_sort.png")
plt.show()