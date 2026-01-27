import sys
import random
import time
import argparse
import subprocess
import re
from flow_utils import FlowNetwork

def solve_task2():
    parser = argparse.ArgumentParser(description="Task 2: Bipartite Matching")
    parser.add_argument('--size', type=int, required=True, help="k (2^k nodes per side)")
    parser.add_argument('--degree', type=int, required=True, help="i neighbors per node")
    parser.add_argument('--printMatching', action='store_true', help="Print list of matched edges")
    parser.add_argument('--glpk', type=str, help="Generate GLPK LP model file and solve it")
    args = parser.parse_args()

    k, i_deg = args.size, args.degree
    n_side = 2**k
    # Source (0), V1 (1 to n), V2 (n+1 to 2n), Sink (2n+1)
    source = 0
    sink = 2 * n_side + 1
    fn = FlowNetwork(sink + 1)

    start_time = time.time()

    # connect source to left partition and right partition to sink
    for node in range(1, n_side + 1):
        fn.add_edge(source, node, 1)
        fn.add_edge(node + n_side, sink, 1)

    # Random bipartite edges V1 -> V2
    for u in range(1, n_side + 1):
        targets = random.sample(range(1, n_side + 1), i_deg)
        for v_idx in targets:
            fn.add_edge(u, v_idx + n_side, 1)

    max_matching = fn.edmonds_karp(source, sink)
    end_time = time.time()

    print(f"Max Matching Size: {max_matching}")
    if args.printMatching:
        print("Matching Edges (V1 - V2):")
        for u, v, flow in fn.get_flow_on_edges():
            # Only edges between V1 and V2 with flow indicate matching
            if 1 <= u <= n_side and n_side < v <= 2 * n_side and flow == 1:
                print(f"{u} - {v - n_side}")

    sys.stderr.write(f"Execution Time: {end_time - start_time:.4f}s\n")

    if args.glpk:
        print(f"\n--- GLPK Solver ---")
        lp_filename = args.glpk
        fn.export_lp_model(lp_filename, source, sink)
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
        glpk_obj = 0.0
        glpk_edge_flows = {}
        
        with open(term_out_filename, 'r') as f:
            content = f.read()
            match = re.search(r"Objective:\s+\w+\s+=\s+([0-9\.e\+\-]+)", content)
            if match:
                glpk_obj = float(match.group(1))
            
            lines = content.splitlines()
            read_cols = False
            for line in lines:
                if line.strip().startswith("No.") and "Column name" in line:
                    read_cols = True
                    continue
                if read_cols:
                    if "------" in line: continue
                    parts = line.split()
                    if len(parts) >= 3:
                        name = parts[1]
                        if name.startswith("f_"):
                            try:
                                activity = float(parts[2])
                                glpk_edge_flows[name] = activity
                            except ValueError:
                                pass

        print(f"GLPK Max Matching: {glpk_obj}")
        print(f"GLPK Time: {end_glpk - start_glpk:.4f}s")
        
        if abs(glpk_obj - max_matching) < 1e-5:
            print("Status: Matching sizes MATCH.")
        else:
            print("Status: Matching sizes MISMATCH!")

        if k <= 4:
            print("Checking matching edges for k <= 4...")
            match_all = True
            
            # Check edges between V1 and V2
            for u in range(1, n_side + 1):
                # Find neighbors
                # Iterate original edges from u
                for v, cap, rev, orig_cap in fn.adj[u]:
                     if orig_cap > 0 and n_side < v <= 2 * n_side:
                         # This is a matching edge candidate
                         var_name = f"f_{u}_{v}"
                         # In case of parallel edges (not in this problem setup but generally), might need index.
                         # Bipartite matching here adds simple edges.
                         
                         # Check our flow
                         our_flow = orig_cap - cap # flow = capacity - residual
                         glpk_flow = glpk_edge_flows.get(var_name, 0.0)
                         
                         if abs(our_flow - glpk_flow) > 1e-5:
                             print(f"  Difference on edge {u}->{v}: My={our_flow}, GLPK={glpk_flow}")
                             match_all = False
            
            if match_all:
                print("  All flow assignments match (Matching is identical).")
            else:
                print("  Flow assignments differ (Matching logic might found different valid matching).")


if __name__ == "__main__":
    solve_task2()