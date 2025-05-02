import random
import subprocess
import os
import matplotlib.pyplot as plt
import re

def parse_output(output):
    comparisons_match = re.search(r'Comparisons: (\d+)', output)
    swaps_match = re.search(r'Assignments: (\d+)', output)
    
    if comparisons_match and swaps_match:
        comparisons = int(comparisons_match.group(1))
        swaps = int(swaps_match.group(1))
        return comparisons, swaps
    else:
        return None

def select_kth_element(generator, size, select_algo, kth):
    try:
        generator_result = subprocess.run(
            [generator, str(size)], 
            stdout=subprocess.PIPE, 
            text=True,
            check=True
        )
        generated_data = generator_result.stdout

        select_result = subprocess.run(
            [select_algo, str(kth)], 
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
    generators = ["./generator_random", "./generator_ascending", "./generator_descending"]
    algorithms = ["./select", "./randomized_select"]

    sizes = [10000 * i for i in range(1, 11)]  # 100 to 1000

    results = {}  # {(algorithm, generator): {'sizes': [], 'comparisons': [], 'assignments': []}}

    for algorithm in algorithms:
        for generator in generators:
            key = (algorithm, generator)
            results[key] = {'sizes': [], 'comparisons': [], 'assignments': []}
            for size in sizes:
                kth = random.randint(1, size)
                total_comparisons = 0
                total_assignments = 0
                tries = 10
                for _ in range(tries):
                    output = select_kth_element(generator, size, algorithm, kth)
                    if output:
                        comp, assign = output
                        total_comparisons += comp
                        total_assignments += assign
                results[key]['sizes'].append(size)
                results[key]['comparisons'].append(total_comparisons / tries)
                results[key]['assignments'].append(total_assignments / tries)

    # Plotting
    for metric in ['comparisons', 'assignments']:
        plt.figure(figsize=(12, 6))
        for (algorithm, generator), data in results.items():
            label = f"{os.path.basename(algorithm)} | {os.path.basename(generator)}"
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