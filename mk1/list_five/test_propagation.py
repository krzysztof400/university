import subprocess
import random
import re
import os
import sys
import numpy as np
import matplotlib.pyplot as plt
from pathlib import Path

# --- Configuration ---

# Paths to the user's executables.
# Assumes they are in the same directory as this script or in the system's PATH.
GEN_TREE_EXEC = "./gen_complete_tree"
PRIMA_EXEC = "./algo_prima"
PROPAGATION_EXEC = "./algo_propagation"

# Range of graph sizes (number of nodes 'n') to test.
NODE_SIZES = range(10, 1001, 10)  # Test for n = 10, 20, 30, ..., 100

# Number of random root nodes to test for each graph size to get a good average.
ITERATIONS_PER_SIZE = 30

# --- Helper Functions ---

def check_executables_exist():
    """Checks if all required C++ executables exist before running."""
    required_execs = [GEN_TREE_EXEC, PRIMA_EXEC, PROPAGATION_EXEC]
    missing_files = []
    for exec_path in required_execs:
        if not Path(exec_path).is_file():
            missing_files.append(exec_path)

    if missing_files:
        print("Error: The following required executables were not found:")
        for f in missing_files:
            print(f"  - {f}")
        print("\nPlease compile the C++ programs using your makefile before running this script.")
        return False
    
    print("All required executables found.")
    return True

def generate_mst(n):
    """
    Generates an MST for a complete graph of size n by chaining
    gen_complete_tree and algo_prima.
    """
    try:
        # Step 1: Run gen_complete_tree
        gen_proc = subprocess.Popen([GEN_TREE_EXEC, str(n)], stdout=subprocess.PIPE, text=True)

        # Step 2: Pipe its output to algo_prima
        prima_proc = subprocess.Popen([PRIMA_EXEC, str(n)], stdin=gen_proc.stdout, stdout=subprocess.PIPE, text=True)

        # Allow gen_proc to receive a SIGPIPE if prima_proc exits.
        gen_proc.stdout.close()

        # Step 3: Capture the output from algo_prima
        mst_output, stderr = prima_proc.communicate()

        if prima_proc.returncode != 0:
            print(f"Error running MST generation for n={n}.")
            print(f"Stderr: {stderr}")
            return None
        return mst_output

    except FileNotFoundError as e:
        print(f"Error: Command not found - {e.filename}. Make sure it's compiled and in your PATH.")
        return None
    except Exception as e:
        print(f"An unexpected error occurred during MST generation: {e}")
        return None

def run_propagation_analysis(n, mst_data, root_node):
    """
    Runs the C++ propagation program for a given MST and root node,
    and parses the number of rounds from its output.
    """
    # The C++ program expects input starting with a specific header
    # and the root/n values.
    propagation_input = f"MST (Prim):\n{root_node} {n}\n{mst_data}"

    try:
        result = subprocess.run(
            [PROPAGATION_EXEC],
            input=propagation_input,
            capture_output=True,
            text=True,
            check=True  # Throws an exception for non-zero exit codes
        )

        # Parse the output to find the number of rounds
        output_lines = result.stdout.splitlines()
        for line in output_lines:
            match = re.search(r"Optimal Propagation Rounds:\s*(\d+)", line)
            if match:
                return int(match.group(1))

        print("Error: Could not parse propagation rounds from C++ program output.")
        print("--- C++ stdout ---")
        print(result.stdout)
        return None

    except FileNotFoundError:
        print(f"Error: The program '{PROPAGATION_EXEC}' was not found.")
        return None
    except subprocess.CalledProcessError as e:
        print(f"Error running '{PROPAGATION_EXEC}' for n={n}, root={root_node}.")
        print(f"Stderr: {e.stderr}")
        return None
    except Exception as e:
        print(f"An unexpected error occurred during propagation analysis: {e}")
        return None


def plot_results(results):
    """
    Generates and displays a plot of the results using matplotlib.
    """
    if not results:
        print("No results to plot.")
        return

    n_values = sorted(results.keys())
    min_rounds = [results[n]['min'] for n in n_values]
    avg_rounds = [results[n]['avg'] for n in n_values]
    max_rounds = [results[n]['max'] for n in n_values]

    # FIX: Changed the style to a more common one to avoid FileNotFoundError
    plt.style.use('seaborn-v0_8-whitegrid')
    fig, ax = plt.subplots(figsize=(12, 8))

    # Plotting the data
    ax.plot(n_values, avg_rounds, 'o-', label='Average Rounds', color='royalblue', linewidth=2, markersize=8)
    ax.plot(n_values, min_rounds, '^-', label='Minimum Rounds', color='forestgreen', linestyle='--', markersize=6)
    ax.plot(n_values, max_rounds, 's-', label='Maximum Rounds', color='indianred', linestyle='--', markersize=6)

    # Filling the area between min and max
    ax.fill_between(n_values, min_rounds, max_rounds, color='orange', alpha=0.2, label='Min-Max Range')

    # Adding titles and labels
    ax.set_title('Optimal Propagation Rounds vs. Number of Nodes', fontsize=16, fontweight='bold')
    ax.set_xlabel('Number of Nodes (n)', fontsize=12)
    ax.set_ylabel('Number of Rounds', fontsize=12)
    ax.legend(fontsize=10)
    ax.grid(True, which='both', linestyle='--', linewidth=0.5)

    # Setting ticks
    ax.set_xticks(n_values)
    plt.xticks(rotation=45)
    plt.tight_layout()

    # Save the plot to a file
    plot_filename = "propagation_analysis_plot.png"
    plt.savefig(plot_filename)
    print(f"\nPlot saved as '{plot_filename}'")

    # Display the plot
    plt.show()


# --- Main Execution ---

def main():
    """Main function to run the entire test suite."""
    if not check_executables_exist():
        sys.exit(1) # Exit if executables are missing

    all_results = {}

    print("\n--- Starting Propagation Analysis ---")
    for n in NODE_SIZES:
        print(f"\nProcessing for n = {n}...")

        # Generate the MST once for this size 'n'
        mst_data = generate_mst(n)
        if mst_data is None:
            print(f"Skipping n={n} due to MST generation error.")
            continue

        rounds_for_n = []
        for i in range(ITERATIONS_PER_SIZE):
            # Select a different random root for each iteration
            random_root = random.randint(0, n - 1)
            
            rounds = run_propagation_analysis(n, mst_data, random_root)

            if rounds is not None:
                rounds_for_n.append(rounds)
            
            # Simple progress indicator
            print(f"\r  Iteration {i+1}/{ITERATIONS_PER_SIZE} (root={random_root})...", end="")
        
        print("\n  Done.")

        if rounds_for_n:
            all_results[n] = {
                'min': np.min(rounds_for_n),
                'max': np.max(rounds_for_n),
                'avg': np.mean(rounds_for_n)
            }
            print(f"  Results for n={n}: Min={all_results[n]['min']}, "
                  f"Avg={all_results[n]['avg']:.2f}, Max={all_results[n]['max']}")
        else:
            print(f"  No valid results obtained for n={n}.")

    print("\n--- Analysis Complete ---")
    plot_results(all_results)


if __name__ == "__main__":
    main()