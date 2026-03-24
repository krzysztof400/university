import math
from typing import Optional

import matplotlib.pyplot as plt


class Graph:
    def __init__(self, path: str):
        self.cities: list[tuple[int, tuple[float, float]]] = []
        self.distances: list[list[float]] = []
        self.mst: list[tuple[int, int]] = []
        self.tsp_path: list[int] = []
        self.name: str

        self._load(path)
        self._compute_distances()

    def _load(self, path: str):
        with open(path, "r") as f:
            for line in f:
                if line.startswith("NAME"):
                    self.name = line.split(":")[-1]
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
                self.distances[i][j] = d
                self.distances[j][i] = d

    def compute_mst(self):
        visited = []
        visited.append(cities[0])
        min_edge = float("inf")
        for city in cities:
            

    def tsp_total_distance(self) -> Optional[float]:
        if not self.tsp_path:
            return None
        indices = [cid - 1 for cid in self.tsp_path]
        total = sum(
            self.distances[indices[i]][indices[i + 1]] for i in range(len(indices) - 1)
        )
        total += self.distances[indices[-1]][indices[0]]
        return total

    def mst_total_weight(self) -> Optional[float]:
        if not self.mst:
            return None
        return sum(self.distances[u][v] for u, v in self.mst)

    def print_distances(self):
        for row in self.distances:
            print("  ".join(f"{d:7.1f}" for d in row))

    def visualize(self, filename: str = "plot_tsp"):
        plots = []
        if self.mst:
            plots.append("mst")
        if self.tsp_path:
            plots.append("tsp")

        if not plots:
            print(
                "Nothing to visualize — run compute_mst() and/or compute_tsp_greedy() first."
            )
            return

        for plot_type in plots:
            fig, ax = plt.subplots(figsize=(10, 7))

            xs = [c[1][0] for c in self.cities]
            ys = [c[1][1] for c in self.cities]

            if plot_type == "mst":
                for u, v in self.mst:
                    ax.plot(
                        [self.cities[u][1][0], self.cities[v][1][0]],
                        [self.cities[u][1][1], self.cities[v][1][1]],
                        color="green",
                        linewidth=1.2,
                        zorder=1,
                        label="MST" if (u, v) == self.mst[0] else "",
                    )
                ax.set_title(
                    f"MST Visualization\nMST weight: {self.mst_total_weight():.1f}"
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
                    f"TSP Visualization\nTSP distance: {self.tsp_total_distance():.1f}"
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

            out = f"{filename}_{self.name}.png"
            fig.savefig(out, dpi=150, bbox_inches="tight")
            plt.close(fig)
            print(f"Saved: {out}")
