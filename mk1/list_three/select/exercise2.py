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

    sizes = [500 * i for i in range(1, 101)]  # 100 to 1000

    results = {}  # {(algorithm, generator): {'sizes': [], 'comparisons': [], 'assignments': []}}

    for algorithm in algorithms:
        for generator in generators:
            key = (algorithm, generator)
            results[key] = {'sizes': [], 'comparisons': [], 'assignments': []}
            for size in sizes:
                kth = random.randint(1, size)
                total_comparisons = 0
                total_assignments = 0
                tries = 5
                for _ in range(tries):
                    output = select_kth_element(generator, size, algorithm, kth)
                    if output:
                        comp, assign = output
                        total_comparisons += comp
                        total_assignments += assign
                results[key]['sizes'].append(size)
                results[key]['comparisons'].append(total_comparisons / tries)
                results[key]['assignments'].append(total_assignments / tries)

    fig, axs = plt.subplots(2, 2, figsize=(14, 10))  # 2 rows, 2 columns

    metrics = ['comparisons', 'assignments']
    titles = {
        'comparisons': 'Comparisons vs Input Size',
        'assignments': 'Assignments vs Input Size',
        'comparisons/n': 'Comparisons / n vs Input Size',
        'assignments/n': 'Assignments / n vs Input Size'
    }

    # Top row: absolute values
    for idx, metric in enumerate(metrics):
        ax = axs[0, idx]
        for (algorithm, generator), data in results.items():
            label = f"{os.path.basename(algorithm)} | {os.path.basename(generator)}"
            ax.plot(data['sizes'], data[metric], label=label, marker='o')
        ax.set_title(titles[metric])
        ax.set_xlabel("Input Size")
        ax.set_ylabel(metric.capitalize())
        ax.grid(True)
        ax.legend()

    # Bottom row: metric / n
    for idx, metric in enumerate(metrics):
        ax = axs[1, idx]
        for (algorithm, generator), data in results.items():
            label = f"{os.path.basename(algorithm)} | {os.path.basename(generator)}"
            y_values = [v / n if n != 0 else 0 for v, n in zip(data[metric], data['sizes'])]
            ax.plot(data['sizes'], y_values, label=label, marker='o')
        ax.set_title(titles[f"{metric}/n"])
        ax.set_xlabel("Input Size")
        ax.set_ylabel(f"{metric.capitalize()} / n")
        ax.grid(True)
        ax.legend()

    plt.tight_layout()
    plt.savefig("all_metrics_plot.png")
    plt.show()



if __name__ == "__main__":
    main()