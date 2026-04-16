import matplotlib.pyplot as plt
import re

def load_tsp_coordinates(tsp_file: str) -> dict:
    """Extracts node coordinates from a standard TSPLIB file."""
    nodes = {}
    with open(tsp_file, "r") as f:
        parsing = False
        for line in f:
            line = line.strip()
            if line == "NODE_COORD_SECTION":
                parsing = True
                continue
            if line == "EOF" or not line:
                break
            if parsing:
                parts = line.split()
                if len(parts) >= 3:
                    node_id = int(parts[0])
                    nodes[node_id] = (float(parts[1]), float(parts[2]))
    return nodes

def load_tour(tour_file: str) -> list[int]:
    """Reads a sequence of node IDs from a text file."""
    with open(tour_file, "r") as f:
        content = f.read()
    # Grabs all numbers in the file, ignoring text/brackets/commas
    return [int(n) for n in re.findall(r'\d+', content)]

def plot_tsp(nodes: dict, tour: list[int], output_name: str = "tsp_plot.png"):
    """Plots the cities and the calculated path."""
    # Ensure the tour closes back to the start
    if tour[0] != tour[-1]:
        tour.append(tour[0])
        
    try:
        xs = [nodes[node_id][0] for node_id in tour]
        ys = [nodes[node_id][1] for node_id in tour]
    except KeyError as e:
        print(f"Error: Node {e} found in tour but not in the .tsp file!")
        return

    plt.figure(figsize=(10, 7))
    
    # Draw the path
    plt.plot(xs, ys, color="blue", linewidth=1.5, zorder=1, label="Your Path")
    
    # Draw the cities
    plt.scatter(xs, ys, color="red", zorder=2, s=20, label="Cities")
    
    plt.title("TSP Path Visualization")
    plt.xlabel("X Coordinate")
    plt.ylabel("Y Coordinate")
    plt.grid(True, linestyle="--", alpha=0.5)
    plt.legend()
    plt.tight_layout()
    
    # Save and show
    plt.savefig(output_name, dpi=150)
    print(f"Saved plot to {output_name}")
    plt.show()

if __name__ == "__main__":
    # ---------------------------------------------------------
    # 1. PUT YOUR FILE PATHS HERE
    # ---------------------------------------------------------
    TSP_FILE_PATH = "./data/di38.tsp"
    
    # This should be a text file containing your path's node IDs 
    # (e.g., "1 4 2 5 3" or "[1, 4, 2, 5, 3]" or separated by newlines)
    PATH_FILE_PATH = "./batch_results/di3" 
    
    # ---------------------------------------------------------
    # 2. RUN IT
    # ---------------------------------------------------------
    print("Loading coordinates...")
    city_coords = load_tsp_coordinates(TSP_FILE_PATH)
    
    print("Loading path...")
    calculated_path = load_tour(PATH_FILE_PATH)
    
    print("Generating plot...")
    plot_tsp(city_coords, calculated_path)