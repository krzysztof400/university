import os
import subprocess
import argparse
import glob

def run_tests(executable_path, test_directory, extra_args=None):
    """
    Runs a compiled C++ program against all .txt files in a directory.

    Args:
        executable_path (str): The path to the compiled C++ executable.
        test_directory (str): The directory containing the test files.
        extra_args (list, optional): A list of extra command-line arguments.
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
            # Add any extra arguments if provided
            if extra_args:
                command.extend(extra_args)
            
            # Run the executable with the file content as stdin
            result = subprocess.run(
                command,
                input=input_data,
                capture_output=True,
                text=True,
                timeout=30,
                check=True  # Automatically raise an error if the process fails
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
        except subprocess.CalledProcessError as e:
            print(f"Error: Process failed with return code {e.returncode}")
            print("--- STDOUT ---")
            print(e.stdout.strip())
            print("\n--- STDERR ---")
            print(e.stderr.strip())
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

    # Maps user-facing task number to internal task name
    task_map = {
        '1': 'bfs_dfs',
        '2': 'topological_sort',
        '3': 'tarjan',
        '4': 'bipartiteness',
    }
    
    # Consistent config structure:
    # 'executables' is always a list.
    # 'args' is an optional list of arguments for those executables.
    task_configs = {
        'bfs_dfs': {
            'executables': ['1bfs', '1dfs'], 
            'test_subdir': '1', 
            'args': ['print_tree=1']
        },
        'topological_sort': {
            'executables': ['2topological_sort'], 
            'test_subdir': '2', 
            'args': None
        },
        'tarjan': {
            'executables': ['3tarjan'], 
            'test_subdir': '3', 
            'args': None
        },
        'bipartiteness': {
            'executables': ['4bipartiteness'], 
            'test_subdir': '4', 
            'args': None
        },
    }
    
    tasks_to_run = []
    if args.task == 'all':
        # Get all task names (e.g., 'bfs_dfs', 'topological_sort', ...)
        tasks_to_run = task_configs.keys()
    elif args.task in task_map:
        # Get the single internal task name
        tasks_to_run = [task_map[args.task]]
    else:
        print(f"Error: Unknown task '{args.task}'. Choose from {list(task_map.keys())} or 'all'.")
        return

    # Single loop to run all selected tasks
    for task_name in tasks_to_run:
        config = task_configs[task_name]
        test_directory = os.path.join(test_dir, config['test_subdir'])
        extra_args = config.get('args') # Gets args or None if not present
        
        # Loop through all executables for this task (1 for most, 2 for task 1)
        for exec_name in config['executables']:
            executable_path = os.path.join(bin_dir, exec_name)
            run_tests(executable_path, test_directory, extra_args)

if __name__ == "__main__":
    main()
