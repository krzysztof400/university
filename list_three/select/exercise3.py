import random
import subprocess
import os
import matplotlib.pyplot as plt
import re

def parse_output(output):
    comparisons_match = re.search(r'Comparisons: (\d+)', output)
    assignments_match = re.search(r'Assignments: (\d+)', output)
    
    if comparisons_match and assignments_match:
        comparisons = int(comparisons_match.group(1))
        assignments = int(assignments_match.group(1))
        return comparisons, assignments
    else:
        return None

def select_kth_element(generator, size, select_algo, kth, division):
    try:
        generator_result = subprocess.run(
            [generator, str(size)], 
            stdout=subprocess.PIPE, 
            text=True,
            check=True
        )
        generated_data = generator_result.stdout

        select_result = subprocess.run(
            [select_algo, str(kth), str(division)], 
            input=generated_data, 
            stdout=subprocess.PIPE, 
            text=True,
            check=True
        )
    except subprocess.CalledProcessError as e:
        print(f"Error running command: {e}")
        return None
    except Exception as e:
        print(f"Unexpected error: {e}")
        return None

    return parse_output(select_result.stdout.strip())

def main():
    generators = ["./generator_random"]
    algorithms = ["./select_k_divisions"]
    sizes = [100 * i for i in range(1, 101)]  # 100, 200, 300, 400
    divisions = [3, 5, 7, 9, 15, 20, 30, 50, 100]  # Different division sizes
    results = {}  # {(algorithm, generator, division): {'sizes': [], 'comparisons': [], 'assignments': []}}

    for algorithm in algorithms:
        for generator in generators:
            for division in divisions:
                key = (division)
                results[key] = {'sizes': [], 'comparisons': [], 'assignments': []}
                for size in sizes:
                    kth = random.randint(1, size)
                    total_comparisons = 0
                    total_assignments = 0
                    tries = 20
                    for _ in range(tries):
                        output = select_kth_element(generator, size, algorithm, kth, division)
                        if output:
                            comp, assign = output
                            total_comparisons += comp
                            total_assignments += assign
                    # Append averages for this size
                    results[key]['sizes'].append(size)
                    results[key]['comparisons'].append(total_comparisons / tries)
                    results[key]['assignments'].append(total_assignments / tries)

    # Plotting
    for metric in ['comparisons', 'assignments']:
        plt.figure(figsize=(12, 6))
        for (division), data in results.items():
            label = f"division={division}"
            plt.plot(data['sizes'], data[metric], label=label, marker='o')

        plt.title(f"{metric.capitalize()} vs Input Size")
        plt.xlabel("Input Size")
        plt.ylabel(metric.capitalize())
        plt.legend()
        plt.grid(True)
        plt.tight_layout()
        plt.savefig(f"{metric}_plot.png")  # Save to file
        plt.show()

if __name__ == "__main__":
    main()