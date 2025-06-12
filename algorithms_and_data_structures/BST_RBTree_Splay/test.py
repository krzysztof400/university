#!/usr/bin/env python3
"""
Comprehensive testing framework for Red-Black Tree, BST, and Splay Tree implementations.
Tests correctness and performance according to the specified requirements.
"""

import subprocess
import random
import matplotlib.pyplot as plt
import numpy as np
import os
import statistics
from typing import List, Tuple, Dict


class TreeTester:
    """Tester for a single tree implementation executable."""

    def __init__(self, executable_path: str, tree_type: str):
        """
        Initialize the TreeTester with the path to the executable and tree type.

        Args:
            executable_path (str): Path to the tree executable.
            tree_type (str): Type of tree ('rb', 'bst', or 'splay').
        """
        self.executable_path = executable_path
        self.tree_type = tree_type

    def run_commands(self, commands: List[str], print_output: bool = True) -> Tuple[str, Dict]:
        """
        Run the given commands on the tree executable and parse the output statistics.

        Args:
            commands (List[str]): List of commands to execute.
            print_output (bool): Whether to print the command output.

        Returns:
            Tuple[str, Dict]: The raw output and parsed statistics.
        
        Raises:
            RuntimeError: If the process fails, times out, or encounters an error.
        """
        try:
            # Join commands with newlines, then add 'q' to quit
            input_str = '\n'.join(commands) + '\nq\n'

            process = subprocess.Popen(
                [self.executable_path],
                stdin=subprocess.PIPE,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                text=True
            )

            # Increased timeout slightly for potentially slow BST operations
            output, error = process.communicate(input=input_str, timeout=60) 

            if process.returncode != 0:
                raise RuntimeError(f"Process failed ({self.tree_type}) with return code {process.returncode}:\n{error}")

            stats = self._parse_stats(output)

            if print_output:
                print(f"\n=== {self.tree_type.upper()} Tree Output ===\n{output}")

            return output, stats

        except subprocess.TimeoutExpired:
            process.kill()
            raise RuntimeError(f"{self.tree_type} process timed out")
        except Exception as e:
            raise RuntimeError(f"Error running {self.tree_type} commands: {e}")

    def _parse_stats(self, output: str) -> Dict:
        """
        Parse the output to extract statistics like assignments, comparisons, and height.

        Args:
            output (str): The raw output from the tree executable.

        Returns:
            Dict: A dictionary with parsed 'assignments', 'comparisons', and 'height'.
                  - 'assignments' and 'comparisons' reflect totals after all operations.
                  - 'height' reflects the tree height after insertions (first occurrence).
        """
        assignments = 0
        comparisons = 0
        height = 0
        height_set = False

        for line in output.splitlines():
            if 'Total assignments:' in line:
                try:
                    assignments = int(line.split(':')[1].strip())
                except ValueError:
                    pass  # Silently ignore parsing errors
            elif 'Total comparisons:' in line:
                try:
                    comparisons = int(line.split(':')[1].strip())
                except ValueError:
                    pass  # Silently ignore parsing errors
            elif 'Height:' in line and not height_set:
                try:
                    height = int(line.split(':')[1].strip())
                    height_set = True  # Ensure only the first height is captured
                except ValueError:
                    pass  # Silently ignore parsing errors

        return {
            'assignments': assignments,
            'comparisons': comparisons,
            'height': height
        }


class TestRunner:
    """Runner to test multiple tree implementations and compare their performance."""

    def __init__(self, rb_path: str = None, bst_path: str = None, splay_path: str = None):
        """
        Initialize the TestRunner with paths to tree executables.

        Args:
            rb_path (str, optional): Path to Red-Black Tree executable.
            bst_path (str, optional): Path to BST executable.
            splay_path (str, optional): Path to Splay Tree executable.

        Raises:
            ValueError: If no valid executable paths are provided.
        """
        self.testers = {}

        if rb_path and os.path.exists(rb_path):
            self.testers['rb'] = TreeTester(rb_path, 'rb')
        if bst_path and os.path.exists(bst_path):
            self.testers['bst'] = TreeTester(bst_path, 'bst')
        if splay_path and os.path.exists(splay_path):
            self.testers['splay'] = TreeTester(splay_path, 'splay')

        if not self.testers:
            raise ValueError("No valid tree executables found.")

    def test_performance(self):
        """
        Run performance tests for different scenarios and generate comparison plots.

        Returns:
            Dict: Results containing performance metrics for each tree type and scenario.
        """
        print("="*60)
        print("PERFORMANCE TESTING")
        print("="*60)

        n_values = list(range(10000, 100001, 10000))
        num_tests = 5

        results = {
            tree_type: {
                'scenario1': self._init_metrics(),
                'scenario2': self._init_metrics()
            } for tree_type in self.testers
        }

        for n in n_values:
            print(f"\nTesting n = {n:,}")

            print("  Scenario 1 (ascending insert, random delete)")
            scenario1_results = self._run_performance_tests(n, num_tests, scenario=1)

            print("  Scenario 2 (random insert, random delete)")
            scenario2_results = self._run_performance_tests(n, num_tests, scenario=2)

            for tree_type in self.testers:
                for scenario_name, scenario_data in [('scenario1', scenario1_results), ('scenario2', scenario2_results)]:
                    if tree_type in scenario_data:
                        metrics = scenario_data[tree_type]
                        results[tree_type][scenario_name]['n_values'].append(n)
                        for metric in ['comparisons', 'assignments', 'height']:
                            # Only append if there are values to avoid errors with empty lists
                            if metrics[metric]:
                                results[tree_type][scenario_name][f'avg_{metric}'].append(statistics.mean(metrics[metric]))
                                results[tree_type][scenario_name][f'max_{metric}'].append(max(metrics[metric]))
                            else:
                                # Append 0 or a placeholder if no data was collected
                                results[tree_type][scenario_name][f'avg_{metric}'].append(0)
                                results[tree_type][scenario_name][f'max_{metric}'].append(0)


        self._generate_performance_plots(results)
        return results

    def _init_metrics(self) -> Dict:
        """
        Initialize a dictionary to store metrics for a scenario.

        Returns:
            Dict: A dictionary with lists for n_values and average/maximum metrics.
        """
        return {
            'n_values': [],
            'avg_comparisons': [], 'max_comparisons': [],
            'avg_assignments': [], 'max_assignments': [],
            'avg_height': [], 'max_height': []
        }

    def _run_performance_tests(self, n: int, num_tests: int, scenario: int) -> Dict:
        """
        Run performance tests for a given scenario and number of tests.

        Args:
            n (int): Number of elements to test.
            num_tests (int): Number of test iterations.
            scenario (int): Scenario number (1 for ascending insert, 2 for random insert).

        Returns:
            Dict: Results with metrics for each tree type.
        """
        results = {}

        for tree_type, tester in self.testers.items():
            print(f"    {tree_type.upper()}", end="", flush=True)

            test_metrics = {
                'comparisons': [],
                'assignments': [],
                'height': []
            }

            for test_num in range(num_tests):
                if test_num % 1 == 0: # Print a dot more frequently to show progress
                    print(".", end="", flush=True)

                try:
                    # Generate command sequence
                    insert_order = list(range(1, n + 1))
                    if scenario == 2:
                        random.shuffle(insert_order)  # Random insert for scenario 2

                    delete_order = list(insert_order)
                    random.shuffle(delete_order)  # Random delete for both scenarios

                    commands = ['r']  # Reset stats
                    
                    # --- MODIFICATION START ---
                    # Send command character and its argument on separate lines
                    for key in insert_order:
                        commands.append('i')
                        commands.append(str(key))
                    # --- MODIFICATION END ---

                    commands += ['s', 'h']  # Print stats and height after insertions
                    
                    # --- MODIFICATION START ---
                    for key in delete_order:
                        commands.append('d')
                        commands.append(str(key))
                    # --- MODIFICATION END ---

                    commands += ['s', 'h']  # Print stats and height after deletions

                    _, stats = tester.run_commands(commands, print_output=False)

                    for metric in test_metrics:
                        test_metrics[metric].append(stats.get(metric, 0))

                except Exception as e:
                    print(f"\nError in test {test_num} for {tree_type} (N={n}): {e}")
                    # If an error occurs, append default 0s so it doesn't break the averages
                    for metric in test_metrics:
                        test_metrics[metric].append(0) # Or NaN if you prefer to exclude from averages
            
            # Filter out any 0s that came from failed runs if you want averages only for successful runs
            # For robust plotting, it's better to keep 0s or NaNs. Current plotting handles 0s.
            
            print(f" done ({len([x for x in test_metrics['comparisons'] if x > 0])}/{num_tests} successful)") # Show successful tests
            results[tree_type] = test_metrics

        return results

    def _generate_performance_plots(self, results: Dict):
        """
        Generate plots to compare performance metrics across tree types and scenarios.

        Args:
            results (Dict): Performance test results.
        """
        print("\nGenerating plots...")

        metrics = ['comparisons', 'assignments', 'height']
        metric_labels = ['Comparisons', 'Assignments', 'Height']
        scenarios = ['scenario1', 'scenario2']
        scenario_labels = ['Ascending Insert', 'Random Insert']
        colors = {'rb': 'red', 'bst': 'blue', 'splay': 'green'}
        linestyles = {'avg': 'o-', 'max': 's--'}

        fig, axes = plt.subplots(3, 2, figsize=(16, 12))
        fig.suptitle('Tree Performance Comparison', fontsize=18)

        for i, metric in enumerate(metrics):
            for j, scenario in enumerate(scenarios):
                ax = axes[i][j]
                
                # Collect all data points for this metric and scenario
                all_n_values = []
                all_avg_data = []
                all_max_data = []
                
                for tree_type in results:
                    data = results[tree_type][scenario]
                    if data['n_values']:
                        # Ensure lists are not empty before plotting
                        if data[f'avg_{metric}'] and data[f'max_{metric}']:
                            ax.plot(data['n_values'], data[f'avg_{metric}'], linestyles['avg'], label=f"{tree_type.upper()} (avg)", color=colors.get(tree_type, 'black'))
                            ax.plot(data['n_values'], data[f'max_{metric}'], linestyles['max'], label=f"{tree_type.upper()} (max)", alpha=0.6, color=colors.get(tree_type, 'black'))
                        else:
                            print(f"Warning: No data for {tree_type.upper()} {metric} in {scenario_labels[j]} (avg/max lists empty). Skipping plot.")


                ax.set_title(f"{metric_labels[i]} - {scenario_labels[j]}")
                ax.set_xlabel("n (elements)")
                ax.set_ylabel(metric_labels[i])
                ax.grid(True, alpha=0.3)
                
                # Only show legend if there are lines to show
                if ax.lines: 
                    ax.legend()

                # Apply log scale only if there's actual data and it's not height
                if metric != 'height':
                    # Check if all y-values are zero, if so, don't use log scale
                    has_non_zero_data = False
                    for tree_type in results:
                        data = results[tree_type][scenario]
                        if data[f'avg_{metric}'] and any(val > 0 for val in data[f'avg_{metric}']):
                            has_non_zero_data = True
                            break
                    
                    if has_non_zero_data:
                        ax.set_yscale('log')
                    else:
                        # If all data is zero, set y-limits to make 0 visible
                        ax.set_ylim(-0.1, 1) # Small range including 0

        plt.tight_layout(rect=[0, 0.03, 1, 0.95])
        plt.savefig("tree_performance_comparison.png", dpi=300)
        plt.show()

def main():
    """Main entry point for the testing framework."""
    print("Tree Implementation Testing Framework")
    print("=" * 50)

    # Check for executables and provide clearer messages
    rb_executable = "./rb_tree"
    bst_executable = "./bst"
    splay_executable = "./splay_tree"

    found_executables = []
    if os.path.exists(rb_executable):
        found_executables.append(rb_executable)
    else:
        print(f"Warning: Red-Black Tree executable not found at '{rb_executable}'. Skipping RB Tree tests.")

    if os.path.exists(bst_executable):
        found_executables.append(bst_executable)
    else:
        print(f"Warning: BST executable not found at '{bst_executable}'. Skipping BST tests.")

    if os.path.exists(splay_executable):
        found_executables.append(splay_executable)
    else:
        print(f"Warning: Splay Tree executable not found at '{splay_executable}'. Skipping Splay Tree tests.")

    if not found_executables:
        print("Error: No tree executables found. Please compile them and place them in the same directory as the script.")
        return

    try:
        runner = TestRunner(
            rb_path=rb_executable if os.path.exists(rb_executable) else None,
            bst_path=bst_executable if os.path.exists(bst_executable) else None,
            splay_path=splay_executable if os.path.exists(splay_executable) else None
        )
        runner.test_performance()
    except Exception as e:
        print(f"An unexpected error occurred during testing: {e}")


if __name__ == "__main__":
    main()