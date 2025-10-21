import os
import subprocess
import argparse
import glob

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

    print(f"--- Testing {os.path.basename(executable_path)} ---")
    for test_file in sorted(test_files):
        print(f"\n--- Running on {os.path.basename(test_file)} ---")
        try:
            with open(test_file, 'r') as f:
                input_data = f.read()

            command = [executable_path]

            result = subprocess.run(
                command,
                input=input_data,
                capture_output=True,
                text=True,
                timeout=30  # 30-second timeout for each test case
            )

            print("--- STDOUT ---")
            # Print stdout line by line, removing trailing empty lines
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
    parser = argparse.ArgumentParser(description="Run tests for AoD Lab 1.")
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
        'bfs_dfs': {'executable': '1bfs_dfs', 'test_subdir': '1'},
        'topological_sort': {'executable': '2topological_sort', 'test_subdir': '2'},
        'tarjan': {'executable': '3tarjan', 'test_subdir': '3'},
        'bipartiteness': {'executable': '4bipartiteness', 'test_subdir': '4'},
    }
    
    actual_task = task_map.get(args.task)

    if args.task == 'all':
        for task_name in task_configs:
            config = task_configs[task_name]
            executable_path = os.path.join(bin_dir, config['executable'])
            test_directory = os.path.join(test_dir, config['test_subdir'])
            run_tests(executable_path, test_directory)
            
    elif actual_task:
        config = task_configs[actual_task]
        executable_path = os.path.join(bin_dir, config['executable'])
        test_directory = os.path.join(test_dir, config['test_subdir'])
        run_tests(executable_path, test_directory)
        
    else:
        print(f"Unknown task: {args.task}. Please use 1, 2, 3, 4, or 'all'.")

if __name__ == "__main__":
    main()

