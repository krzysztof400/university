import os
import subprocess
import argparse
import glob

def run_bfs_dfs(config, bin_dir, test_dir):
    """Runs both BFS and DFS tests for task 1."""
    test_directory = os.path.join(test_dir, config['test_subdir'])

    # Run BFS
    executable_path_bfs = os.path.join(bin_dir, config['executable_bfs'])
    run_tests(executable_path_bfs, test_directory)

    # FIX: Run DFS (was running BFS again)
    executable_path_dfs = os.path.join(bin_dir, config['executable_dfs'])
    run_tests(executable_path_dfs, test_directory)

def run_tests(executable_path, test_directory):
    """
    Runs a compiled C++ program against all .txt files in a directory.

    Args:
        executable_path (str): The path to the compiled C++ executable.
        test_directory (str): The directory containing the test files.
    """
    if not os.path.exists(executable_path):
        print(f"Error: Executable not found at {executable_path}")
        return

    test_files = glob.glob(os.path.join(test_directory, '*.txt'))
    if not test_files:
        print(f"No test files found in {test_directory}")
        return

    exec_name = os.path.basename(executable_path)
    print(f"--- Testing {exec_name} ---")
    
    for test_file in sorted(test_files):
        print(f"\n--- Running on {os.path.basename(test_file)} ---")
        try:
            with open(test_file, 'r') as f:
                input_data = f.read()

            command = [executable_path]
            
            # FIX: Check basename and append argument correctly
            if exec_name == "1bfs" or exec_name == "1dfs":
                command.append("print_tree=1")

            # FIX: Consolidated subprocess run
            result = subprocess.run(
                command,
                input=input_data,
                capture_output=True,
                text=True,
                timeout=30
            )

            print("--- STDOUT ---")
            print(result.stdout.strip())
            if result.stderr:
                print("\n--- STDERR ---")
                print(result.stderr.strip())

        except FileNotFoundError:
            print(f"Error: The file {test_file} was not found.")
        except subprocess.TimeoutExpired:
            print("Error: Process timed out.")
        except Exception as e:
            print(f"An error occurred while running the test: {e}")

def main():
    """
    Main function to parse arguments and run the specified tests.
    """
    parser = argparse.ArgumentParser(description="Run tests for AoDO Lab 1.")
    parser.add_argument('task', help="The task to perform: 1, 2, 3, 4, or 'all' to run all tests.")

    args = parser.parse_args()

    bin_dir = './bin'
    test_dir = './test'

    task_map = {
        '1': 'bfs_dfs',
        '2': 'topological_sort',
        '3': 'tarjan',
        '4': 'bipartiteness',
    }
    
    task_configs = {
        'bfs_dfs': {'executable_bfs': '1bfs', 'executable_dfs': '1dfs', 'test_subdir': '1'},
        'topological_sort': {'executable': '2topological_sort', 'test_subdir': '2'},
        'tarjan': {'executable': '3tarjan', 'test_subdir': '3'},
        'bipartiteness': {'executable': '4bipartiteness', 'test_subdir': '4'},
    }
    
    # FIX: Restructured logic to separate 'all' from specific tasks
    if args.task == 'all':
        # Run all tasks
        for task_name, config in task_configs.items():
            if task_name == 'bfs_dfs':
                # FIX: Use the 'config' from the loop, not 'task_configs[actual_task]'
                run_bfs_dfs(config=config, bin_dir=bin_dir, test_dir=test_dir)
            else:
                executable_path = os.path.join(bin_dir, config['executable'])
                test_directory = os.path.join(test_dir, config['test_subdir'])
                run_tests(executable_path, test_directory)
    
    else:
        # Run a specific task
        actual_task = task_map.get(args.task)

        if actual_task == 'bfs_dfs':
            config = task_configs[actual_task]
            run_bfs_dfs(config=config, bin_dir=bin_dir, test_dir=test_dir)
        
        elif actual_task:
            config = task_configs[actual_task]
            executable_path = os.path.join(bin_dir, config['executable'])
            test_directory = os.path.join(test_dir, config['test_subdir'])
            run_tests(executable_path, test_directory)
            
        else:
            # Handle unknown specific task
            print(f"Unknown task: {args.task}. Please use 1, 2, 3, 4, or 'all'.")

if __name__ == "__main__":
    main()