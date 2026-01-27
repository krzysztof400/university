import subprocess
import matplotlib.pyplot as plt
import numpy as np
import os
import re

def run_task1(k_range, repetitions=3, algorithm="edmonds"):
    """Conducts experiments for Task 1: Hypercube Max Flow."""
    print(f"Running Task 1 experiments ({algorithm}) for k in {k_range}...")
    results = {k: {"flow": [], "paths": [], "time": []} for k in k_range}
    
    for k in k_range:
        # Fewer repetitions for very large k to save time
        current_reps = 1 if k >= 14 else repetitions
        for _ in range(current_reps):
            process = subprocess.Popen(
                ["python3", "hypercube_flow.py", "--size", str(k), "--algorithm", algorithm],
                stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True
            )
            stdout, stderr = process.communicate()
            
            # Parse stdout: "Max Flow Value: 123"
            flow_match = re.search(r"Max Flow Value:\s+(\d+)", stdout)
            # Parse stderr: "Execution Time: 0.1234s", "Augmenting Paths Found: 5"
            time_match = re.search(r"Execution Time:\s+([\d.]+)s", stderr)
            paths_match = re.search(r"Augmenting Paths Found:\s+(\d+)", stderr)
            
            if flow_match and time_match and paths_match:
                results[k]["flow"].append(int(flow_match.group(1)))
                results[k]["time"].append(float(time_match.group(1)))
                results[k]["paths"].append(int(paths_match.group(1)))
        print(f"  k={k} completed.")
    return results

def run_task2(k_range, repetitions=3):
    """Conducts experiments for Task 2: Bipartite Matching."""
    print(f"\nRunning Task 2 experiments for k in {k_range}...")
    results = [] # List of dicts for easier plotting with different 'i' values
    
    for k in k_range:
        for i in range(1, k + 1):
            matches, times = [], []
            for _ in range(repetitions):
                process = subprocess.Popen(
                    ["python3", "bipartite_matching.py", "--size", str(k), "--degree", str(i)],
                    stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True
                )
                stdout, stderr = process.communicate()
                
                match_res = re.search(r"Max Matching Size:\s+(\d+)", stdout)
                time_res = re.search(r"Execution Time:\s+([\d.]+)s", stderr)
                
                if match_res and time_res:
                    matches.append(int(match_res.group(1)))
                    times.append(float(time_res.group(1)))
            
            results.append({
                "k": k, "i": i, 
                "avg_match": np.mean(matches), 
                "avg_time": np.mean(times)
            })
        print(f"  k={k} completed.")
    return results

def plot_task1(data):
    """Generates charts for Task 1: Flow, Paths, and Time vs k."""
    ks = sorted(data.keys())
    avg_flows = [np.mean(data[k]["flow"]) for k in ks]
    avg_paths = [np.mean(data[k]["paths"]) for k in ks]
    avg_times = [np.mean(data[k]["time"]) for k in ks]

    fig, axs = plt.subplots(1, 3, figsize=(18, 5))
    
    axs[0].plot(ks, avg_flows, marker='o', color='b')
    axs[0].set_title('Avg Max Flow vs k')
    axs[0].set_xlabel('Dimension k')
    axs[0].set_ylabel('Flow Value')

    axs[1].plot(ks, avg_paths, marker='s', color='r')
    axs[1].set_title('Avg Augmenting Paths vs k')
    axs[1].set_xlabel('Dimension k')
    axs[1].set_ylabel('Path Count')

    axs[2].plot(ks, avg_times, marker='^', color='g')
    axs[2].set_title('Avg Execution Time vs k')
    axs[2].set_xlabel('Dimension k')
    axs[2].set_ylabel('Time (s)')
    
    plt.tight_layout()
    plt.savefig('task1_plots.png')
    print("Saved task1_plots.png")

def plot_task2(data):
    """Generates charts for Task 2: Matching vs i and Time vs k."""
    # 1. Max matching vs i for each k
    plt.figure(figsize=(10, 6))
    ks = sorted(list(set(d['k'] for d in data)))
    for k in ks:
        subset = [d for d in data if d['k'] == k]
        i_vals = [d['i'] for d in subset]
        m_vals = [d['avg_match'] for d in subset]
        plt.plot(i_vals, m_vals, label=f'k={k}', marker='o')
    
    plt.title('Max Matching Size vs Degree (i) for different k')
    plt.xlabel('Degree i')
    plt.ylabel('Matching Size')
    plt.legend()
    plt.grid(True)
    plt.savefig('task2_matching_vs_i.png')

    # 2. Execution time vs k for each i
    plt.figure(figsize=(10, 6))
    max_i = max(d['i'] for d in data)
    for i in range(1, max_i + 1):
        subset = [d for d in data if d['i'] == i]
        if not subset: continue
        k_vals = [d['k'] for d in subset]
        t_vals = [d['avg_time'] for d in subset]
        plt.plot(k_vals, t_vals, label=f'i={i}', marker='s')
    
    plt.title('Execution Time vs k for different i')
    plt.xlabel('Dimension k')
    plt.ylabel('Time (s)')
    plt.yscale('log')
    plt.legend()
    plt.grid(True)
    plt.savefig('task2_time_vs_k.png')
    print("Saved task2 plots.")

def run_glpk_tests():
    """Runs tests with GLPK verificaton for small k."""
    print("\n=== Running GLPK Verifications ===")
    
    # Task 1
    print("\n[Task 1 Hypercube]")
    for k in [2, 3, 4]:
        lp_file = f"hypercube_{k}.lp"
        print(f"Testing k={k}...")
        subprocess.run(["python3", "hypercube_flow.py", "--size", str(k), "--glpk", lp_file], check=True)
        # Cleanup optionally or keep
    
    # Task 2
    print("\n[Task 2 Bipartite Matching]")
    for k in [2, 3, 4]:
        i = 2 # degree
        lp_file = f"matching_{k}_{i}.lp"
        print(f"Testing k={k}, i={i}...")
        subprocess.run(["python3", "bipartite_matching.py", "--size", str(k), "--degree", str(i), "--glpk", lp_file], check=True)

def compare_and_plot_task1(k_range, repetitions=3):
    """Runs both Edmonds-Karp and SAP algorithms and compares performance."""
    print("\n=== Running Comparison Tests: Edmonds-Karp vs SAP ===")
    
    print("Collecting data for Edmonds-Karp...")
    res_edmonds = run_task1(k_range, repetitions, algorithm="edmonds")
    
    print("Collecting data for Shortest Augmenting Path (SAP)...")
    res_sap = run_task1(k_range, repetitions, algorithm="sap")
    
    ks = sorted(res_edmonds.keys())
    
    # Process data
    time_edmonds = [np.mean(res_edmonds[k]["time"]) for k in ks]
    time_sap = [np.mean(res_sap[k]["time"]) for k in ks]
    
    paths_edmonds = [np.mean(res_edmonds[k]["paths"]) for k in ks]
    paths_sap = [np.mean(res_sap[k]["paths"]) for k in ks]
    
    # Plotting
    fig, axs = plt.subplots(1, 2, figsize=(14, 6))
    
    # Time plot
    axs[0].plot(ks, time_edmonds, marker='o', label='Edmonds-Karp', color='b')
    axs[0].plot(ks, time_sap, marker='s', label='SAP', color='r')
    axs[0].set_title('Avg Execution Time vs k')
    axs[0].set_xlabel('Dimension k')
    axs[0].set_ylabel('Time (s)')
    axs[0].set_yscale('log')
    axs[0].legend()
    axs[0].grid(True)
    
    # Paths plot
    axs[1].plot(ks, paths_edmonds, marker='o', label='Edmonds-Karp', color='b')
    axs[1].plot(ks, paths_sap, marker='s', label='SAP', color='r')
    axs[1].set_title('Avg Augmenting Paths vs k')
    axs[1].set_xlabel('Dimension k')
    axs[1].set_ylabel('Path Count')
    axs[1].set_yscale('log')
    axs[1].legend()
    axs[1].grid(True)
    
    plt.tight_layout()
    plt.savefig('comparison_task1.png')
    print("Saved comparison_task1.png")

if __name__ == "__main__":
    compare_and_plot_task1(range(1, 13), repetitions=5)
    
    t1_results = run_task1(range(1, 17), repetitions=5) 
    plot_task1(t1_results)

    t2_results = run_task2(range(3, 7), repetitions=5)
    plot_task2(t2_results)
    
    run_glpk_tests()