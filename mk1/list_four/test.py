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
import tempfile
import sys
from typing import List, Tuple, Dict
import statistics

class TreeTester:
    def __init__(self, executable_path: str, tree_type: str):
        """
        Initialize tree tester.
        
        Args:
            executable_path: Path to the compiled tree executable
            tree_type: Type of tree ('rb', 'bst', 'splay')
        """
        self.executable_path = executable_path
        self.tree_type = tree_type
        
    def run_commands(self, commands: List[str], print_output: bool = True) -> Tuple[str, Dict]:
        """
        Run a sequence of commands on the tree implementation.
        
        Args:
            commands: List of commands to execute
            print_output: Whether to print tree states
            
        Returns:
            Tuple of (output, stats) where stats contains performance metrics
        """
        try:
            # Create input string
            input_str = '\n'.join(commands) + '\nq\n'
            
            # Run the executable
            process = subprocess.Popen(
                [self.executable_path],
                stdin=subprocess.PIPE,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                text=True
            )
            
            output, error = process.communicate(input=input_str, timeout=30)
            
            if process.returncode != 0:
                raise RuntimeError(f"Process failed with return code {process.returncode}: {error}")
            
            # Parse statistics from output
            stats = self._parse_stats(output)
            
            if print_output:
                print(f"\n=== {self.tree_type.upper()} Tree Output ===")
                print(output)
                
            return output, stats
            
        except subprocess.TimeoutExpired:
            process.kill()
            raise RuntimeError("Process timed out")
        except Exception as e:
            raise RuntimeError(f"Error running commands: {e}")
    
    def _parse_stats(self, output: str) -> Dict:
        """Parse performance statistics from program output."""
        stats = {
            'assignments': 0,
            'comparisons': 0,
            'height': 0
        }
        
        lines = output.split('\n')
        for line in lines:
            if 'Total assignments:' in line:
                try:
                    stats['assignments'] = int(line.split(':')[1].strip())
                except:
                    pass
            elif 'Total comparisons:' in line:
                try:
                    stats['comparisons'] = int(line.split(':')[1].strip())
                except:
                    pass
            elif 'Height:' in line:
                try:
                    stats['height'] = int(line.split(':')[1].strip())
                except:
                    pass
                    
        return stats

class TestRunner:
    def __init__(self, rb_path: str, bst_path: str = None, splay_path: str = None):
        """
        Initialize test runner with paths to tree implementations.
        
        Args:
            rb_path: Path to Red-Black tree executable
            bst_path: Path to BST executable (optional)
            splay_path: Path to Splay tree executable (optional)
        """
        self.testers = {}
        
        if rb_path and os.path.exists(rb_path):
            self.testers['rb'] = TreeTester(rb_path, 'rb')
        if bst_path and os.path.exists(bst_path):
            self.testers['bst'] = TreeTester(bst_path, 'bst')
        if splay_path and os.path.exists(splay_path):
            self.testers['splay'] = TreeTester(splay_path, 'splay')
            
        if not self.testers:
            raise ValueError("No valid tree executables found")
    

    
    def test_performance(self):
        """
        Test performance for large datasets (n = 10k, 20k, ..., 100k).
        Run 20 tests for each n value.
        """
        print("="*60)
        print("PERFORMANCE TESTING")
        print("="*60)
        
        n_values = [10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000, 100000]
        num_tests = 20
        
        results = {}
        
        for tree_type in self.testers.keys():
            results[tree_type] = {
                'scenario1': {'n_values': [], 'avg_comparisons': [], 'max_comparisons': [], 
                             'avg_assignments': [], 'max_assignments': [], 'avg_height': [], 'max_height': []},
                'scenario2': {'n_values': [], 'avg_comparisons': [], 'max_comparisons': [],
                             'avg_assignments': [], 'max_assignments': [], 'avg_height': [], 'max_height': []}
            }
        
        for n in n_values:
            print(f"\nTesting n = {n:,}")
            
            # Scenario 1: Ascending insert, random delete
            print(f"  Scenario 1 (ascending insert, random delete)")
            scenario1_results = self._run_performance_tests(n, num_tests, scenario=1)
            
            # Scenario 2: Random insert, random delete  
            print(f"  Scenario 2 (random insert, random delete)")
            scenario2_results = self._run_performance_tests(n, num_tests, scenario=2)
            
            # Store results
            for tree_type in self.testers.keys():
                if tree_type in scenario1_results:
                    results[tree_type]['scenario1']['n_values'].append(n)
                    for metric in ['comparisons', 'assignments', 'height']:
                        values = scenario1_results[tree_type][metric]
                        results[tree_type]['scenario1'][f'avg_{metric}'].append(statistics.mean(values))
                        results[tree_type]['scenario1'][f'max_{metric}'].append(max(values))
                
                if tree_type in scenario2_results:
                    results[tree_type]['scenario2']['n_values'].append(n)
                    for metric in ['comparisons', 'assignments', 'height']:
                        values = scenario2_results[tree_type][metric]
                        results[tree_type]['scenario2'][f'avg_{metric}'].append(statistics.mean(values))
                        results[tree_type]['scenario2'][f'max_{metric}'].append(max(values))
        
        # Generate plots
        self._generate_performance_plots(results)
        
        return results
    
    def _run_performance_tests(self, n: int, num_tests: int, scenario: int) -> Dict:
        """Run performance tests for given n and scenario."""
        results = {}
        
        for tree_type, tester in self.testers.items():
            print(f"    Testing {tree_type.upper()}", end="", flush=True)
            
            test_results = {
                'comparisons': [],
                'assignments': [],
                'height': []
            }
            
            for test_num in range(num_tests):
                if test_num % 5 == 0:
                    print(".", end="", flush=True)
                
                try:
                    # Generate test data
                    if scenario == 1:
                        insert_order = list(range(1, n + 1))  # Ascending
                    else:
                        insert_order = list(range(1, n + 1))
                        random.shuffle(insert_order)  # Random
                    
                    delete_order = list(range(1, n + 1))
                    random.shuffle(delete_order)  # Always random delete
                    
                    # Create commands (without printing)
                    commands = ['r']  # Reset stats
                    
                    # Insert operations
                    for key in insert_order:
                        commands.append(f'i{key}')
                    
                    # Get stats after insertions
                    commands.append('s')
                    commands.append('h')
                    
                    # Delete operations
                    for key in delete_order:
                        commands.append(f'd{key}')
                    
                    # Get final stats
                    commands.append('s')
                    commands.append('h')
                    
                    # Run test
                    output, stats = tester.run_commands(commands, print_output=False)
                    
                    # Store results
                    test_results['comparisons'].append(stats.get('comparisons', 0))
                    test_results['assignments'].append(stats.get('assignments', 0))
                    test_results['height'].append(stats.get('height', 0))
                    
                except Exception as e:
                    print(f"Error in test {test_num}: {e}")
                    continue
            
            print(f" completed ({len(test_results['comparisons'])} successful tests)")
            results[tree_type] = test_results
        
        return results
    
    def _generate_performance_plots(self, results: Dict):
        """Generate performance plots from test results."""
        print("\nGenerating performance plots...")
        
        metrics = ['comparisons', 'assignments', 'height']
        metric_labels = ['Number of Comparisons', 'Number of Assignments', 'Tree Height']
        
        scenarios = ['scenario1', 'scenario2']
        scenario_labels = ['Scenario 1 (Ascending Insert)', 'Scenario 2 (Random Insert)']
        
        # Create subplots for each metric and scenario
        fig, axes = plt.subplots(3, 2, figsize=(15, 12))
        fig.suptitle('Tree Performance Comparison', fontsize=16)
        
        colors = {'rb': 'red', 'bst': 'blue', 'splay': 'green'}
        
        for metric_idx, (metric, metric_label) in enumerate(zip(metrics, metric_labels)):
            for scenario_idx, (scenario, scenario_label) in enumerate(zip(scenarios, scenario_labels)):
                ax = axes[metric_idx, scenario_idx]
                
                # Plot average values
                for tree_type in results.keys():
                    data = results[tree_type][scenario]
                    if data['n_values']:
                        ax.plot(data['n_values'], data[f'avg_{metric}'], 
                               'o-', color=colors.get(tree_type, 'black'), 
                               label=f'{tree_type.upper()} (avg)', linewidth=2)
                        ax.plot(data['n_values'], data[f'max_{metric}'], 
                               's--', color=colors.get(tree_type, 'black'), 
                               label=f'{tree_type.upper()} (max)', alpha=0.7)
                
                ax.set_xlabel('n (number of elements)')
                ax.set_ylabel(metric_label)
                ax.set_title(f'{metric_label} - {scenario_label}')
                ax.legend()
                ax.grid(True, alpha=0.3)
                
                # Use log scale for large values
                if metric in ['comparisons', 'assignments']:
                    ax.set_yscale('log')
        
        plt.tight_layout()
        plt.savefig('tree_performance_comparison.png', dpi=300, bbox_inches='tight')
        plt.show()
        
        # Generate separate detailed plots for each metric
        for metric_idx, (metric, metric_label) in enumerate(zip(metrics, metric_labels)):
            plt.figure(figsize=(12, 8))
            
            for scenario_idx, (scenario, scenario_label) in enumerate(zip(scenarios, scenario_labels)):
                plt.subplot(1, 2, scenario_idx + 1)
                
                for tree_type in results.keys():
                    data = results[tree_type][scenario]
                    if data['n_values']:
                        plt.plot(data['n_values'], data[f'avg_{metric}'], 
                               'o-', color=colors.get(tree_type, 'black'), 
                               label=f'{tree_type.upper()} (avg)', linewidth=2)
                        plt.plot(data['n_values'], data[f'max_{metric}'], 
                               's--', color=colors.get(tree_type, 'black'), 
                               label=f'{tree_type.upper()} (max)', alpha=0.7)
                
                plt.xlabel('n (number of elements)')
                plt.ylabel(metric_label)
                plt.title(scenario_label)
                plt.legend()
                plt.grid(True, alpha=0.3)
                
                if metric in ['comparisons', 'assignments']:
                    plt.yscale('log')
            
            plt.suptitle(f'{metric_label} Comparison', fontsize=14)
            plt.tight_layout()
            plt.savefig(f'tree_{metric}_comparison.png', dpi=300, bbox_inches='tight')
            plt.show()

def main():
    """Main function to run all tests."""
    print("Tree Implementation Testing Framework")
    print("="*50)

    rb_executable = "./rb_tree" if os.path.exists("./rb_tree") else None
    bst_executable = "./bst_tree" if os.path.exists("./bst_tree") else None
    splay_executable = "./splay_tree" if os.path.exists("./splay_tree") else None
    
    try:
        runner = TestRunner(rb_executable, bst_executable, splay_executable)
        runner.test_performance()
    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    main()