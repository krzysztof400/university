import sys
import random
import time
import argparse
import subprocess
import re
from flow_utils import FlowNetwork

def hamming_weight(n):
    return bin(n).count('1')

def solve_task1():
    parser = argparse.ArgumentParser(description="Task 1: Max Flow in Hypercube")
    parser.add_argument('--size', type=int, required=True, help="Dimension k")
    parser.add_argument('--algorithm', type=str, choices=['edmonds', 'sap'], default='edmonds', help="Max flow algorithm to use (edmonds or sap)")
    parser.add_argument('--printFlow', action='store_true', help="Print flow on each edge")
    parser.add_argument('--glpk', type=str, help="Generate GLPK LP model file and solve it")
    args = parser.parse_args()

    k = args.size
    num_nodes = 2**k
    fn = FlowNetwork(num_nodes)
    
    start_time = time.time()
    
    # Generate hypercube edges
    for u in range(num_nodes):
        h_u = hamming_weight(u)
        z_u = k - h_u
        for bit in range(k):
            # If bit is 0, setting it to 1 increases Hamming weight by 1 [cite: 10]
            if not (u & (1 << bit)):
                v = u | (1 << bit)
                h_v = h_u + 1
                z_v = k - h_v
                # Capacity range defined by max of weights and zeros
                l = max(h_u, z_u, h_v, z_v)
                capacity = random.randint(1, 2**l)
                fn.add_edge(u, v, capacity)

    # Calculate max flow from source 0 to sink 2^k - 1 [cite: 16]
    if args.algorithm == 'sap':
        max_f = fn.shortest_augmenting_path(0, num_nodes - 1)
    else:
        max_f = fn.edmonds_karp(0, num_nodes - 1)
    end_time = time.time()

    print(f"Max Flow Value: {max_f}")
    if args.printFlow:
        print("Edge Flows (u -> v : flow):")
        for u, v, flow in fn.get_flow_on_edges():
            print(f"{u} -> {v} : {flow}")
    sys.stderr.write(f"Execution Time: {end_time - start_time:.4f}s\n")
    sys.stderr.write(f"Augmenting Paths Found: {fn.paths_found}\n")

    if args.glpk:
        print(f"\n--- GLPK Solver ---")
        lp_filename = args.glpk
        fn.export_lp_model(lp_filename, 0, num_nodes - 1)
        print(f"LP model saved to {lp_filename}")

        # Run glpsol
        term_out_filename = lp_filename + ".out"
        start_glpk = time.time()
        try:
            subprocess.run(["glpsol", "--lp", lp_filename, "-o", term_out_filename], check=True, stdout=subprocess.DEVNULL)
        except FileNotFoundError:
            print("Error: glpsol not found. Please install glpk-utils.")
            return
        except subprocess.CalledProcessError as e:
            print(f"Error running glpsol: {e}")
            return
        end_glpk = time.time()
        
        # Parse output
        glpk_flow = 0.0
        glpk_edge_flows = {}
        
        with open(term_out_filename, 'r') as f:
            content = f.read()
            
            # Find objective value
            # "Objective:  MaxFlowProblem = 2.000000000e+01 (MAXIMUM)"
            match = re.search(r"Objective:\s+\w+\s+=\s+([0-9\.e\+\-]+)", content)
            if match:
                glpk_flow = float(match.group(1))
            
            # Parse variable values (Columns section)
            # This is tricky as GLPK output format is fixed width. 
            # We look for lines containing variable names like 'f_0_1'.
            
            # Typical GLPK "Columns" section format:
            #    No. Column name       Activity     Lower bound   Upper bound    Marginal
            # ------ ------------    ------------- ------------- ------------- -------------
            #      1 f_0_1            10             .            15 
            
            lines = content.splitlines()
            read_cols = False
            for line in lines:
                if line.strip().startswith("No.") and "Column name" in line:
                    read_cols = True
                    continue
                if read_cols and not line.strip(): # End of table
                   # Might be separated by empty lines or dashes
                   if "------" not in line: # safeguard
                       pass
                       
                if read_cols:
                    if "------" in line: continue
                    parts = line.split()
                    if len(parts) >= 3:
                        # parts[1] is name, parts[2] is activity
                        name = parts[1]
                        if name.startswith("f_"):
                            try:
                                activity = float(parts[2])
                                glpk_edge_flows[name] = activity
                            except ValueError:
                                pass # formatting might be different if activity is not number
        
        print(f"GLPK Max Flow: {glpk_flow}")
        print(f"GLPK Time: {end_glpk - start_glpk:.4f}s")
        
        if abs(glpk_flow - max_f) < 1e-5:
            print("Status: Max Flow values MATCH.")
        else:
            print("Status: Max Flow values MISMATCH!")

        if k <= 4:
            print("Checking edge flows for k <= 4...")
            match_all = True
            # Get internal flows
            my_flows = {}
            for u, v, flow in fn.get_flow_on_edges():
                # Construct possible variable names
                # In export_lp_model: f_{u}_{v} or f_{u}_{v}_{i}
                # Since we don't know the exact index allocation here trivially without replicating logic, 
                # we should probably have stored the mapping or just check sums for (u,v).
                # But hypercube has no parallel edges. So f_{u}_{v} is unique.
                my_flows[f"f_{u}_{v}"] = flow
            
            for name, val in my_flows.items():
                glpk_val = glpk_edge_flows.get(name, 0)
                if abs(val - glpk_val) > 1e-5:
                    print(f"  Difference on {name}: My Algo = {val}, GLPK = {glpk_val}")
                    match_all = False
            
            if match_all:
                print("  All edge flows match.")
            else:
                print("  Some edge flows differ (Note: Max flow edge assignments may not be unique).")

if __name__ == "__main__":
    solve_task1()