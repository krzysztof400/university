import os
import subprocess
import argparse
import glob
from graphviz import Digraph, Graph

def visualize_graph(file_path, output_format='png'):
    """
    Generates a visualization of a graph from a text file.

    The graph file is expected to have the following format:
    - First line: 'D' for directed, 'U' for undirected.
    - Second line: Number of vertices (n).
    - Third line: Number of edges (m).
    - Next m lines: Edge definitions in the format 'u v'.

    Args:
        file_path (str): The path to the graph file.
        output_format (str): The output format for the visualization (e.g., 'png', 'svg').
    """
    try:
        with open(file_path, 'r') as f:
            lines = f.readlines()

        graph_type = lines[0].strip().upper()
        edges = [line.strip().split() for line in lines[3:]]

        if graph_type == 'D':
            dot = Digraph(comment=f'Graph from {os.path.basename(file_path)}')
        elif graph_type == 'U':
            dot = Graph(comment=f'Graph from {os.path.basename(file_path)}')
        else:
            print(f"Warning: Unknown graph type '{graph_type}' in {file_path}. Assuming undirected.")
            dot = Graph(comment=f'Graph from {os.path.basename(file_path)}')

        nodes = set()
        for edge in edges:
            if len(edge) == 2:
                u, v = edge
                nodes.add(u)
                nodes.add(v)
                dot.edge(u, v)

        for node in nodes:
            dot.node(node)

        output_filename = os.path.splitext(file_path)[0]
        dot.render(output_filename, format=output_format, view=False, cleanup=True)
        print(f"Generated visualization for {file_path} as {output_filename}.{output_format}")

    except FileNotFoundError:
        print(f"Error: The file {file_path} was not found.")
    except Exception as e:
        print(f"An error occurred while visualizing {file_path}: {e}")

def visualize_tree(edges, graph_type, output_filename_base, output_format='png'):
    """
    Generates a visualization of a tree from a list of edges.
    """
    try:
        if graph_type == 'D':
            dot = Digraph(comment=f'Tree from {os.path.basename(output_filename_base)}')
        else: # 'U' or other, default to undirected for trees
            dot = Graph(comment=f'Tree from {os.path.basename(output_filename_base)}')

        nodes = set()
        for edge in edges:
            if len(edge) == 2:
                u, v = edge
                nodes.add(u)
                nodes.add(v)
                dot.edge(u, v)

        for node in nodes:
            dot.node(node)

        dot.render(output_filename_base, format=output_format, view=False, cleanup=True)
        print(f"Generated tree visualization as {output_filename_base}.{output_format}")

    except Exception as e:
        print(f"An error occurred while visualizing the tree: {e}")


def run_tests(executable_path, test_directory, extra_args=None, visualize_tree_output=False):
    """
    Runs a compiled C++ program against all .txt files in a directory.

    Args:
        executable_path (str): The path to the compiled C++ executable.
        test_directory (str): The directory containing the test files.
        extra_args (list, optional): A list of extra command-line arguments for the executable.
        visualize_tree_output (bool): If True, parses TREE_EDGES from stdout and visualizes the tree.
    """
    if not os.path.exists(executable_path):
        print(f"Error: Executable not found at {executable_path}")
        return

    test_files = glob.glob(os.path.join(test_directory, '*.txt'))
    if not test_files:
        print(f"No test files found in {test_directory}")
        return

    print(f"\n--- Testing {os.path.basename(executable_path)} ---")
    for test_file in sorted(test_files):
        print(f"\n--- Running on {os.path.basename(test_file)} ---")
        try:
            with open(test_file, 'r') as f:
                input_data = f.read()

            command = [executable_path]
            if extra_args:
                command.extend(extra_args)

            result = subprocess.run(
                command,
                input=input_data,
                capture_output=True,
                text=True,
                timeout=30 # 30-second timeout for each test case
            )

            print("--- STDOUT ---")
            print(result.stdout)
            if result.stderr:
                print("--- STDERR ---")
                print(result.stderr)

            if visualize_tree_output and 'TREE_EDGES:' in result.stdout:
                lines = result.stdout.splitlines()
                try:
                    tree_edges_index = lines.index('TREE_EDGES:')
                    edge_lines = lines[tree_edges_index + 1:]
                    tree_edges = [line.strip().split() for line in edge_lines if line.strip()]
                    
                    if tree_edges:
                        with open(test_file, 'r') as f:
                            graph_type = f.readline().strip().upper()
                        
                        output_filename_base = os.path.splitext(test_file)[0] + '_tree'
                        visualize_tree(tree_edges, graph_type, output_filename_base)

                except (ValueError, IndexError) as e:
                    print(f"Could not parse tree edges from output for {test_file}: {e}")


        except FileNotFoundError:
            print(f"Error: The file {test_file} was not found.")
        except subprocess.TimeoutExpired:
            print("Error: Process timed out.")
        except Exception as e:
            print(f"An error occurred while running the test: {e}")

def create_graph(filename, graph_type, num_vertices, edges_list):
    """
    Creates a graph file from provided data.
    """
    num_edges = len(edges_list)
    try:
        with open(filename, 'w') as f:
            f.write(f"{graph_type.upper()}\n")
            f.write(f"{num_vertices}\n")
            f.write(f"{num_edges}\n")
            for u, v in edges_list:
                f.write(f"{u} {v}\n")
        print(f"Successfully created graph file: {filename}")
        print(f"Type: {graph_type.upper()}, Vertices: {num_vertices}, Edges: {num_edges}")
    except Exception as e:
        print(f"An error occurred while creating the graph file: {e}")


def main():
    """
    Main function to parse arguments and run the specified tests or visualizations.
    """
    parser = argparse.ArgumentParser(description="Run tests, visualize, or create graphs for AoD Lab 1.")
    parser.add_argument('task',
                        help="The task to perform: 1/bfs_dfs, 2/topological_sort, 3/tarjan, 4/bipartiteness, all, visualize, create.")
    parser.add_argument('--test_dir', default='./test', help="The base directory for test files.")
    parser.add_argument('--bin_dir', default='./bin', help="The directory containing compiled executables.")
    parser.add_argument('--file', help="The file to visualize or the output file for creation.")
    parser.add_argument('--format', default='png', help="Output format for visualization (e.g., png, svg).")
    
    parser.add_argument('--graphtype', choices=['D', 'U'], help="Graph type for creation ('D' for directed, 'U' for undirected).")
    parser.add_argument('--nodes', type=int, help="Total number of nodes for graph creation.")
    parser.add_argument('--edges', nargs='+', help="List of edges for graph creation, e.g., --edges 1 2 2 3.")

    args = parser.parse_args()

    task_map = {
        '1': 'bfs_dfs', 'bfs_dfs': 'bfs_dfs',
        '2': 'topological_sort', 'topological_sort': 'topological_sort',
        '3': 'tarjan', 'tarjan': 'tarjan',
        '4': 'bipartiteness', 'bipartiteness': 'bipartiteness',
    }
    
    task_configs = {
        'bfs_dfs': {'executable': '1bfs_dfs', 'test_subdir': '1'},
        'topological_sort': {'executable': '2topological_sort', 'test_subdir': '2'},
        'tarjan': {'executable': '3tarjan', 'test_subdir': '3'},
        'bipartiteness': {'executable': '4bipartiteness', 'test_subdir': '4'},
    }
    
    actual_task = task_map.get(args.task)

    if args.task == 'create':
        if not (args.graphtype and args.nodes is not None and args.edges and args.file):
            print("Usage for 'create': python run_test.py create --file <filename> --graphtype <D/U> --nodes <n> --edges <u1 v1 u2 v2 ...>")
            return
        
        if len(args.edges) % 2 != 0:
            print("Error: Edges must be provided in pairs (e.g., u v).")
            return
        
        edge_pairs = [[args.edges[i], args.edges[i+1]] for i in range(0, len(args.edges), 2)]
        create_graph(args.file, args.graphtype, args.nodes, edge_pairs)

    elif args.task == 'visualize':
        if args.file:
            visualize_graph(args.file, args.format)
        else:
            print("Please specify a file to visualize using the --file argument.")
            print("Example: python run_test.py visualize --file ./test/1/6vertices.txt")
            
    elif args.task == 'all':
        for task_name in task_configs:
            config = task_configs[task_name]
            executable_path = os.path.join(args.bin_dir, config['executable'])
            test_directory = os.path.join(args.test_dir, config['test_subdir'])
            
            extra_args = None
            visualize_tree = False
            if task_name == 'bfs_dfs':
                extra_args = ['bfs', 'start=1', 'print_tree=1']
                visualize_tree = True

            run_tests(executable_path, test_directory, extra_args=extra_args, visualize_tree_output=visualize_tree)
            
    elif actual_task:
        config = task_configs[actual_task]
        executable_path = os.path.join(args.bin_dir, config['executable'])
        test_directory = os.path.join(args.test_dir, config['test_subdir'])
        
        extra_args = None
        visualize_tree = False
        if actual_task == 'bfs_dfs':
            extra_args = ['bfs', 'start=1', 'print_tree=1']
            visualize_tree = True
            
        run_tests(executable_path, test_directory, extra_args=extra_args, visualize_tree_output=visualize_tree)
        
    else:
        print(f"Unknown task: {args.task}")


if __name__ == "__main__":
    try:
        import graphviz
    except ImportError:
        print("Graphviz library not found. Please install it to use the visualization feature:")
        print("pip install graphviz")
    main()

