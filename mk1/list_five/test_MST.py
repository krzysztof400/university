import subprocess
import re
import matplotlib.pyplot as plt
import numpy as np
import os
import time
from collections import defaultdict

def run_piped_command(command_template, num_vertices):
    """
    Executes a piped shell command, measures its execution time,
    and extracts 'Assignments' and 'Comparisons' from the final command's output.

    Args:
        command_template (str): The command string with '{n}' as a placeholder.
        num_vertices (int): The current number of vertices (N) for the experiment.

    Returns:
        tuple: (assignments, comparisons, elapsed_time) if successful,
               otherwise (None, None, None).
    """
    # Format the command string by replacing the placeholder with the actual value
    formatted_command = command_template.format(n=num_vertices)
    
    # Split the command string by the pipe character to get individual commands
    commands_list = formatted_command.split('|')
    
    current_stdin = None
    last_stdout_output = "" # To store the stdout of the last command in the pipe

    start_time = time.perf_counter() # Start the timer

    try:
        # Iterate through the commands to set up the pipe
        for i, cmd_str in enumerate(commands_list):
            # Split the command string into executable and its arguments
            parts = cmd_str.strip().split()
            if not parts:
                continue # Skip empty parts

            executable = parts[0]
            args = parts[1:]

            # Use subprocess.Popen for piping
            process = subprocess.Popen(
                [executable] + args,
                stdin=current_stdin,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE, # Capture stderr for debugging
                text=True # Decode stdout/stderr as text
            )

            # The previous process's stdout is now closed by Popen, which is fine
            if i < len(commands_list) - 1:
                current_stdin = process.stdout
            else:
                # For the last command, wait for it to complete and capture its output
                last_stdout_output, stderr_output = process.communicate()
                
                if process.returncode != 0:
                    print(f"Error executing final command '{cmd_str}' in pipe '{formatted_command}':")
                    print(f"Stderr: {stderr_output}")
                    return None, None, None
        
        end_time = time.perf_counter() # Stop the timer
        elapsed_time = end_time - start_time

        # Parse the last command's stdout for Assignments and Comparisons
        assignments_match = re.search(r"Assignments: (\d+)", last_stdout_output)
        comparisons_match = re.search(r"Comparisons: (\d+)", last_stdout_output)

        assignments = int(assignments_match.group(1)) if assignments_match else 0
        comparisons = int(comparisons_match.group(1)) if comparisons_match else 0
        
        return assignments, comparisons, elapsed_time
    
    except FileNotFoundError as e:
        print(f"Error: Executable '{executable}' not found in command '{cmd_str}' (part of '{formatted_command}').")
        print("Please ensure your C++ executables are compiled and in the same directory or in your system's PATH.")
        return None, None, None
    except Exception as e:
        print(f"An unexpected error occurred during command execution ('{formatted_command}'): {e}")
        return None, None, None

def main():
    # --- Configuration for experiments ---
    n_min = 100    # Minimum number of vertices
    n_max = 1001   # Maximum number of vertices
    step = 100     # Increment for the number of vertices
    rep = 5        # Number of repetitions for each graph size

    # --- Command templates for the algorithms ---
    # Assumes the C++ executables are in the current directory
    prim_command_template = "./gen_complete_tree {n} | ./algo_prima {n}"
    kruskal_command_template = "./gen_complete_tree {n} | ./algo_kruskal {n}"

    # --- Data storage ---
    results_prim_assignments = defaultdict(list)
    results_prim_comparisons = defaultdict(list)
    results_prim_time = defaultdict(list)
    results_kruskal_assignments = defaultdict(list)
    results_kruskal_comparisons = defaultdict(list)
    results_kruskal_time = defaultdict(list)

    # --- Run Experiments ---
    print("Starting experiments...")
    for n in range(n_min, n_max + 1, step):
        print(f"Testing for N={n}, (repetitions: {rep})...")
        
        for r in range(rep):
            # Run Prim's algorithm
            prim_assign, prim_comp, prim_t = run_piped_command(prim_command_template, n)
            if all(v is not None for v in [prim_assign, prim_comp, prim_t]):
                results_prim_assignments[n].append(prim_assign)
                results_prim_comparisons[n].append(prim_comp)
                results_prim_time[n].append(prim_t)
            else:
                print(f"  Warning: Prim's experiment for N={n}, repetition {r+1} failed. Skipping.")
            
            # Run Kruskal's algorithm
            kruskal_assign, kruskal_comp, kruskal_t = run_piped_command(kruskal_command_template, n)
            if all(v is not None for v in [kruskal_assign, kruskal_comp, kruskal_t]):
                results_kruskal_assignments[n].append(kruskal_assign)
                results_kruskal_comparisons[n].append(kruskal_comp)
                results_kruskal_time[n].append(kruskal_t)
            else:
                print(f"  Warning: Kruskal's experiment for N={n}, repetition {r+1} failed. Skipping.")

    # --- Process Results (Calculate Averages) ---
    all_n_values = sorted(list(set(results_prim_assignments.keys()).union(results_kruskal_assignments.keys())))

    prim_n_values_with_data = [n for n in all_n_values if n in results_prim_assignments and results_prim_assignments[n]]
    kruskal_n_values_with_data = [n for n in all_n_values if n in results_kruskal_assignments and results_kruskal_assignments[n]]

    # Calculate averages for Prim's
    avg_prim_assignments = [np.mean(results_prim_assignments[n]) for n in prim_n_values_with_data]
    avg_prim_comparisons = [np.mean(results_prim_comparisons[n]) for n in prim_n_values_with_data]
    avg_prim_time = [np.mean(results_prim_time[n]) for n in prim_n_values_with_data]

    # Calculate averages for Kruskal's
    avg_kruskal_assignments = [np.mean(results_kruskal_assignments[n]) for n in kruskal_n_values_with_data]
    avg_kruskal_comparisons = [np.mean(results_kruskal_comparisons[n]) for n in kruskal_n_values_with_data]
    avg_kruskal_time = [np.mean(results_kruskal_time[n]) for n in kruskal_n_values_with_data]

    # --- Visualize Results ---
    plt.style.use('seaborn-v0_8-darkgrid')

    # Plot for Average Assignments
    plt.figure(figsize=(12, 7))
    if prim_n_values_with_data:
        plt.plot(prim_n_values_with_data, avg_prim_assignments, marker='o', linestyle='-', color='blue', label="Prim's (Assignments)")
    if kruskal_n_values_with_data:
        plt.plot(kruskal_n_values_with_data, avg_kruskal_assignments, marker='x', linestyle='--', color='red', label="Kruskal's (Assignments)")
    
    plt.title('Average Assignments vs. Number of Vertices (N)')
    plt.xlabel('Number of Vertices (N)')
    plt.ylabel('Average Assignments')
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.show()

    # Plot for Average Comparisons
    plt.figure(figsize=(12, 7))
    if prim_n_values_with_data:
        plt.plot(prim_n_values_with_data, avg_prim_comparisons, marker='o', linestyle='-', color='green', label="Prim's (Comparisons)")
    if kruskal_n_values_with_data:
        plt.plot(kruskal_n_values_with_data, avg_kruskal_comparisons, marker='x', linestyle='--', color='purple', label="Kruskal's (Comparisons)")
    
    plt.title('Average Comparisons vs. Number of Vertices (N)')
    plt.xlabel('Number of Vertices (N)')
    plt.ylabel('Average Comparisons')
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.show()

    # Plot for Average Time
    plt.figure(figsize=(12, 7))
    if prim_n_values_with_data:
        plt.plot(prim_n_values_with_data, avg_prim_time, marker='o', linestyle='-', color='orange', label="Prim's (Time)")
    if kruskal_n_values_with_data:
        plt.plot(kruskal_n_values_with_data, avg_kruskal_time, marker='x', linestyle='--', color='cyan', label="Kruskal's (Time)")

    plt.title('Average Execution Time vs. Number of Vertices (N)')
    plt.xlabel('Number of Vertices (N)')
    plt.ylabel('Average Time (seconds)')
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.show()

    print("\nExperiment complete. Plots displayed.")

if __name__ == "__main__":
    main()
