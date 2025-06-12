import matplotlib.pyplot as plt
import re
import subprocess
import sys
import math

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
    try:
        # Run generator and capture its output
        generator_result = subprocess.run(
            [generator_path, str(n)], 
            stdout=subprocess.PIPE, 
            text=True,
            check=True
        )
        generated_data = generator_result.stdout
        
        # Pass the generated data to the sorting algorithm
        sort_result = subprocess.run(
            [sort_path], 
            input=generated_data, 
            stdout=subprocess.PIPE, 
            text=True,
            check=True
        )
        
        # Combine outputs for parsing
        combined_output = generated_data + sort_result.stdout
        
        result = parse_output(combined_output)
        return result
    except subprocess.CalledProcessError as e:
        print(f"Error running subprocess: {e}")
        return None
    except Exception as e:
        print(f"Unexpected error: {e}")
        return None

sorts = ["./quick_sort", "./DP_quick_sort"]

k = int(input("Enter the number of repetitions (k): "))
n = int(input("Enter the number of elements to sort (n): "))

generator = "./random_generator"
comparisons = []
swaps = []

for sort in sorts:
    total_comparisons = 0
    total_swaps = 0
    successful_runs = 0
    for j in range(k):
        comparisons_swaps = sort_n_elements(n, generator, sort)
        if comparisons_swaps:
            total_comparisons += comparisons_swaps[0]
            total_swaps += comparisons_swaps[1]
            successful_runs += 1
    
    # Calculate averages if we had successful runs
    if successful_runs > 0:
        avg_comparisons = total_comparisons / successful_runs
        avg_swaps = total_swaps / successful_runs
        comparisons.append((n, generator, sort, avg_comparisons))
        swaps.append((n, generator, sort, avg_swaps))
    else:
        print(f"Warning: No successful runs for n={n}, {generator}, {sort}")

# Print the results
print("Results:")
for n, generator, sort, avg in swaps:
    print(f"Generator: {generator}, Sort: {sort}, Avg Swaps: {avg}")
    print(f"Constant: {avg / (n * math.log2(n))}")

