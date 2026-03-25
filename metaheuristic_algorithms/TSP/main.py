"""
Usage:
    python main.py <path_to_file.tsp>
"""

import sys

from graph import Graph


def separator(title: str = ""):
    line = "=" * 60
    if title:
        print(f"\n{line}")
        print(f"  {title}")
        print(line)
    else:
        print(line)


def run(path: str):
    print(f"\nLoading: {path}")
    g = Graph(path)
    print(f"  Cities loaded: {len(g.cities)}")
    separator()

    # ------------------------------------------------------------------
    # Task 1 – Random permutation sampling
    # ------------------------------------------------------------------
    separator("TASK 1 – Random Permutation Sampling (1000 draws)")

    results = g.sample_random_permutations(n_total=1000)

    print(
        f"  (a) Avg of minimums over groups of 10  (100 groups): "
        f"{results['avg_min_10']:.2f}"
    )
    print(
        f"  (b) Avg of minimums over groups of 50  ( 20 groups): "
        f"{results['avg_min_50']:.2f}"
    )
    print(
        f"  (c) Global minimum over all 1000 draws:              "
        f"{results['global_min']:.2f}"
    )
    print(
        f"      Best path (first 10 cities shown): "
        f"{results['best_path'][:10]}{'...' if len(results['best_path']) > 10 else ''}"
    )

    # ------------------------------------------------------------------
    # Task 2 – Visualise best random tour
    # ------------------------------------------------------------------
    separator("TASK 2 – Visualisation (best random tour)")
    g.visualize(filename="plot")  # saves plot_tsp_<name>.png

    # ------------------------------------------------------------------
    # Task 3 – Minimum Spanning Tree
    # ------------------------------------------------------------------
    separator("TASK 3 – Minimum Spanning Tree")

    g.compute_mst()
    mst_w = g.mst_total_weight()
    tsp_best = results["global_min"]

    print(f"  MST total weight:              {mst_w:.2f}")
    print(f"  Best random TSP distance:      {tsp_best:.2f}")
    print(f"  MST < TSP?  {'YES ✓' if mst_w < tsp_best else 'NO (need more draws?)'}")
    print()
    print("  Why MST weight ≤ optimal TSP distance:")
    print("  Removing any one edge from an optimal TSP tour leaves a")
    print("  spanning tree, which cannot be lighter than the MST.")
    print("  Therefore: w(MST) ≤ w(optimal TSP tour).")

    # visualise MST
    g.tsp_path = []  # clear tsp so only MST is drawn
    g.visualize(filename="plot")

    # ------------------------------------------------------------------
    # Task 4 – TSP tour derived from MST (DFS pre-order, 2-approximation)
    # ------------------------------------------------------------------
    separator("TASK 4 – MST-based TSP (2-approximation via DFS pre-order)")

    g.compute_tsp_from_mst()
    mst_tsp_dist = g.tsp_total_distance()

    print(f"  MST weight:                    {mst_w:.2f}")
    print(f"  MST-based TSP tour distance:   {mst_tsp_dist:.2f}")
    print(
        f"  Ratio MST-TSP / MST:           {mst_tsp_dist / mst_w:.4f}  "
        f"(should be ≤ 2.0)"
    )
    print(f"  Best random TSP distance:      {tsp_best:.2f}")
    print(
        f"  MST-TSP ≤ 2 × MST?  "
        f"{'YES ✓' if mst_tsp_dist <= 2 * mst_w + 1e-6 else 'NO – check implementation'}"
    )

    # visualise MST-based tour
    g.visualize(filename="plot")

    separator()
    print("All tasks completed.\n")


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python main.py <path_to_tsp_file>")
        print("Example: python main.py wi29.tsp")
        sys.exit(1)
    run(sys.argv[1])
