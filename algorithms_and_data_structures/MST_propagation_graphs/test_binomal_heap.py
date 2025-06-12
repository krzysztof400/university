import subprocess
import random
import matplotlib.pyplot as plt

def run_experiment(n, num_experiments=5):
    results = []
    for experiment in range(num_experiments):
        comparisons = []
        # Step 1: Create two heaps (Make-Heap)
        comparisons.append(0)  # No comparisons for Make-Heap

        # Step 2: Insert random elements into both heaps
        heap1 = [random.randint(1, 100000) for _ in range(n)]
        heap2 = [random.randint(1, 100000) for _ in range(n)]

        for value in heap1:
            comparisons.append(run_cpp_command(f"1 {value}"))

        for value in heap2:
            comparisons.append(run_cpp_command(f"2 {value}"))

        # Step 3: Merge the two heaps (Heap-Union)
        comparisons.append(run_cpp_command("3"))

        # Step 4: Extract-Min until the heap is empty
        for _ in range(2 * n):
            comparisons.append(run_cpp_command("4"))

        results.append(comparisons)

    return results

def run_cpp_command(command):
    """
    Runs the C++ program with the given command and returns the number of comparisons.
    """
    process = subprocess.Popen(
        ["./exe_binomial_heap"],  # Path to the compiled C++ program
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True
    )
    stdout, _ = process.communicate(input=command)
    for line in stdout.splitlines():
        if "Comparisons during operation:" in line:
            return int(line.split(":")[1].strip())
    return 0

def plot_results(results, n_values):
    """
    Plots the results of the experiments.
    """
    # Historical plot for each experiment
    for experiment, comparisons in enumerate(results):
        plt.figure()
        plt.plot(range(1, len(comparisons) + 1), comparisons, label=f"Experiment {experiment + 1}")
        plt.xlabel("Operation Index")
        plt.ylabel("Number of Comparisons")
        plt.title(f"Historical Plot of Comparisons (n={n_values[experiment]})")
        plt.legend()
        plt.show()

    # Average comparisons vs n
    avg_comparisons = [sum(exp) / len(exp) for exp in results]
    plt.figure()
    plt.plot(n_values, avg_comparisons, marker="o")
    plt.xlabel("n (Number of Elements)")
    plt.ylabel("Average Comparisons per Operation")
    plt.title("Average Comparisons vs n")
    plt.show()

if __name__ == "__main__":
    # Perform experiments for n in {100, 200, ..., 10000}
    n_values = list(range(100, 10001, 100))
    all_results = []

    for n in n_values:
        print(f"Running experiment for n={n}...")
        results = run_experiment(n)
        all_results.append(results)

    # Plot the results
    plot_results(all_results, n_values)