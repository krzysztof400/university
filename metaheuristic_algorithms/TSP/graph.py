import math
import random
from typing import Optional

import matplotlib.pyplot as plt


class Graph:
    def __init__(self, path: str):
        self.cities: list[tuple[int, tuple[float, float]]] = []
        self.distances: list[list[float]] = []
        self.mst: list[tuple[int, int]] = []
        self.tsp_path: list[int] = []
        self.name: str = ""

        self._load(path)
        self._compute_distances()

    def _load(self, path: str):
        with open(path, "r") as f:
            for line in f:
                if line.startswith("NAME"):
                    self.name = line.split(":")[-1].strip()
                if line.strip() == "NODE_COORD_SECTION":
                    break
            for line in f:
                line = line.strip()
                if not line or line == "EOF":
                    break
                city_id, x, y = line.split()
                self.cities.append((int(city_id), (float(x), float(y))))

    def _compute_distances(self):
        n = len(self.cities)
        self.distances = [[0.0] * n for _ in range(n)]
        for i in range(n):
            for j in range(i + 1, n):
                dx = self.cities[i][1][0] - self.cities[j][1][0]
                dy = self.cities[i][1][1] - self.cities[j][1][1]
                d = math.sqrt(dx**2 + dy**2)
                # TSPLIB EUC_2D rounding convention
                d = int(d + 0.5)
                self.distances[i][j] = d
                self.distances[j][i] = d

    # ------------------------------------------------------------------
    # Task 1 – Random permutation sampling
    # ------------------------------------------------------------------

    def sample_random_permutations(self, n_total: int = 1000) -> dict:
        """
        Draw n_total random permutations and compute:
          - avg_min_10:  average of minimums over groups of 10  (100 groups)
          - avg_min_50:  average of minimums over groups of 50  (20 groups)
          - global_min:  best tour found across all 1000 draws
          - best_path:   the permutation that achieved global_min
        """
        n = len(self.cities)
        city_ids = [c[0] for c in self.cities]

        tours: list[tuple[float, list[int]]] = []
        for _ in range(n_total):
            perm = city_ids[:]
            random.shuffle(perm)
            dist = self._path_distance(perm)
            tours.append((dist, perm))

        distances = [d for d, _ in tours]

        # (a) groups of 10
        group_size_10 = 10
        mins_10 = [
            min(distances[i : i + group_size_10])
            for i in range(0, n_total, group_size_10)
        ]
        avg_min_10 = sum(mins_10) / len(mins_10)

        # (b) groups of 50
        group_size_50 = 50
        mins_50 = [
            min(distances[i : i + group_size_50])
            for i in range(0, n_total, group_size_50)
        ]
        avg_min_50 = sum(mins_50) / len(mins_50)

        # (c) global minimum
        best_dist, best_perm = min(tours, key=lambda x: x[0])

        self.tsp_path = best_perm

        return {
            "avg_min_10": avg_min_10,
            "avg_min_50": avg_min_50,
            "global_min": best_dist,
            "best_path": best_perm,
        }

    def _path_distance(self, path: list[int]) -> float:
        indices = [cid - 1 for cid in path]
        total = sum(
            self.distances[indices[i]][indices[i + 1]] for i in range(len(indices) - 1)
        )
        total += self.distances[indices[-1]][indices[0]]
        return total

    # ------------------------------------------------------------------
    # Task 3 – Minimum Spanning Tree (Prim's algorithm)
    # ------------------------------------------------------------------

    def compute_mst(self):
        self.mst = []
        V = len(self.cities)
        is_in_tree = [False] * V
        # (parent_index, min_weight_to_tree)
        min_distance = [(0, float("inf")) for _ in range(V)]
        current = 0
        for i in range(V):
            self.mst.append((current, min_distance[current][0]))
            is_in_tree[current] = True
            # update min_distances
            for j in range(V):
                if self.distances[current][j] < min_distance[j][1]:
                    min_distance[j] = (current, self.distances[current][j])
            # find new closest vertex not yet in tree
            closest_to_tree = float("inf")
            closest_candidate = 0
            for j in range(V):
                if not is_in_tree[j]:
                    if closest_to_tree > min_distance[j][1]:
                        closest_candidate = j
                        closest_to_tree = min_distance[j][1]
            current = closest_candidate

    # ------------------------------------------------------------------
    # Task 4 – TSP tour from MST via DFS pre-order
    # ------------------------------------------------------------------

    def compute_tsp_from_mst(self):
        if not self.mst:
            raise RuntimeError("Call compute_mst() before compute_tsp_from_mst().")

        V = len(self.cities)
        # Build adjacency list from MST edges (skip the dummy self-loop at index 0)
        adj: list[list[int]] = [[] for _ in range(V)]
        for u, v in self.mst[1:]:  # first entry is (0,0) – root self-loop
            adj[u].append(v)
            adj[v].append(u)

        # Iterative DFS pre-order from vertex 0
        visited = [False] * V
        order: list[int] = []
        stack = [0]
        while stack:
            node = stack.pop()
            if visited[node]:
                continue
            visited[node] = True
            order.append(node)
            # push neighbours in reverse order so smallest index is processed first
            for nb in sorted(adj[node], reverse=True):
                if not visited[nb]:
                    stack.append(nb)

        # Convert 0-based indices back to city IDs (1-based)
        self.tsp_path = [self.cities[i][0] for i in order]

    # ------------------------------------------------------------------
    # Metrics
    # ------------------------------------------------------------------

    def tsp_total_distance(self) -> Optional[float]:
        if not self.tsp_path:
            return None
        return self._path_distance(self.tsp_path)

    def mst_total_weight(self) -> Optional[float]:
        if not self.mst:
            return None
        return sum(self.distances[u][v] for u, v in self.mst[1:])

    def print_distances(self):
        for row in self.distances:
            print("  ".join(f"{d:7.1f}" for d in row))

    # ------------------------------------------------------------------
    # Visualisation
    # ------------------------------------------------------------------

    def visualize(self, filename: str = "plot_tsp"):
        plots = []
        if self.mst:
            plots.append("mst")
        if self.tsp_path:
            plots.append("tsp")

        if not plots:
            print(
                "Nothing to visualize — run compute_mst() and/or "
                "sample_random_permutations() / compute_tsp_from_mst() first."
            )
            return

        for plot_type in plots:
            fig, ax = plt.subplots(figsize=(10, 7))

            xs = [c[1][0] for c in self.cities]
            ys = [c[1][1] for c in self.cities]

            if plot_type == "mst":
                for u, v in self.mst[1:]:
                    ax.plot(
                        [self.cities[u][1][0], self.cities[v][1][0]],
                        [self.cities[u][1][1], self.cities[v][1][1]],
                        color="green",
                        linewidth=1.2,
                        zorder=1,
                    )
                ax.set_title(
                    f"MST – {self.name}\nWeight: {self.mst_total_weight():.0f}"
                )

            elif plot_type == "tsp":
                indices = [cid - 1 for cid in self.tsp_path]
                path_xs = [self.cities[i][1][0] for i in indices] + [
                    self.cities[indices[0]][1][0]
                ]
                path_ys = [self.cities[i][1][1] for i in indices] + [
                    self.cities[indices[0]][1][1]
                ]
                ax.plot(
                    path_xs,
                    path_ys,
                    color="blue",
                    linewidth=1,
                    zorder=2,
                    label="TSP path",
                )
                ax.set_title(
                    f"TSP – {self.name}\nDistance: {self.tsp_total_distance():.0f}"
                )

            ax.scatter(xs, ys, color="red", zorder=5, s=40, label="Cities")
            for cid, (x, y) in self.cities:
                ax.annotate(
                    str(cid),
                    (x, y),
                    textcoords="offset points",
                    xytext=(5, 5),
                    fontsize=7,
                    color="dimgray",
                )

            ax.set_xlabel("X Coordinate")
            ax.set_ylabel("Y Coordinate")
            ax.legend()
            plt.tight_layout()

            safe_name = self.name.replace(" ", "_").replace("/", "_")
            out = f"{filename}_{plot_type}_{safe_name}.png"
            fig.savefig(out, dpi=150, bbox_inches="tight")
            plt.close(fig)
            print(f"Saved: {out}")
