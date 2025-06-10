#!/usr/bin/env python3
"""
Test script for Binomial Heap experiments
Performs the required experiments as specified in the assignment.
"""

import subprocess
import os
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from pathlib import Path
import seaborn as sns

class BinomialHeapTester:
    def __init__(self, cpp_file="datastruct_binominal_heap.cpp", executable="binomial_heap"):
        self.cpp_file = cpp_file
        self.executable = executable
        self.compile_program()
    
    def compile_program(self):
        """Compile the C++ program"""
        print(f"Compiling {self.cpp_file}...")
        result = subprocess.run([
            "g++", "-std=c++17", "-O2", "-o", self.executable, self.cpp_file
        ], capture_output=True, text=True)
        
        if result.returncode != 0:
            print(f"Compilation failed: {result.stderr}")
            raise RuntimeError("Failed to compile C++ program")
        print("Compilation successful!")
    
    def run_experiment(self, n, experiment_num):
        """Run a single experiment with given n and experiment number"""
        output_file = f"experiment_n{n}_exp{experiment_num}.csv"
        
        print(f"Running experiment {experiment_num} with n={n}...")
        result = subprocess.run([
            f"./{self.executable}", str(n), str(experiment_num), output_file
        ], capture_output=True, text=True)
        
        if result.returncode != 0:
            print(f"Experiment failed: {result.stderr}")
            return None
        
        print(result.stdout)
        return output_file
    
    def experiment_1_multiple_runs(self, n=500, num_experiments=5):
        """
        Experiment 1: Run 5 experiments with n=500 and create historical comparison plots
        """
        print(f"\n=== Experiment 1: Multiple runs with n={n} ===")
        
        plt.figure(figsize=(15, 10))
        
        all_data = []
        
        for exp_num in range(1, num_experiments + 1):
            output_file = self.run_experiment(n, exp_num)
            if output_file:
                # Read the data
                df = pd.read_csv(output_file)
                df['experiment'] = exp_num
                all_data.append(df)
                
                # Plot individual experiment
                plt.subplot(2, 3, exp_num)
                plt.plot(df['operation'], df['comparisons'], 'b-', alpha=0.7, linewidth=1)
                plt.title(f'Experiment {exp_num}')
                plt.xlabel('Operation Number')
                plt.ylabel('Comparisons per Operation')
                plt.grid(True, alpha=0.3)
        
        # Combined plot
        plt.subplot(2, 3, 6)
        colors = ['red', 'blue', 'green', 'orange', 'purple']
        for i, df in enumerate(all_data):
            plt.plot(df['operation'], df['comparisons'], 
                    color=colors[i], alpha=0.7, linewidth=1, 
                    label=f'Exp {i+1}')
        plt.title('All Experiments Combined')
        plt.xlabel('Operation Number')
        plt.ylabel('Comparisons per Operation')
        plt.legend()
        plt.grid(True, alpha=0.3)
        
        plt.tight_layout()
        plt.savefig(f'experiment1_n{n}_historical_plots.png', dpi=300, bbox_inches='tight')
        plt.show()
        
        # Calculate and display statistics
        combined_df = pd.concat(all_data, ignore_index=True)
        stats = combined_df.groupby('experiment')['comparisons'].agg(['sum', 'mean', 'std'])
        print("\nStatistics per experiment:")
        print(stats)
        
        return all_data
    
    def experiment_2_scaling(self, n_values=None, num_runs_per_n=3):
        """
        Experiment 2: Test scaling behavior for different values of n
        """
        if n_values is None:
            n_values = list(range(100, 1100, 100))  # 100, 200, ..., 1000
        
        print(f"\n=== Experiment 2: Scaling analysis ===")
        print(f"Testing n values: {n_values}")
        
        results = []
        
        for n in n_values:
            print(f"\nTesting n = {n}")
            n_results = []
            
            for run in range(num_runs_per_n):
                output_file = self.run_experiment(n, run + 1)
                if output_file:
                    df = pd.read_csv(output_file)
                    total_comparisons = df['comparisons'].sum()
                    total_operations = len(df)
                    avg_cost = total_comparisons / total_operations
                    avg_cost_per_n = total_comparisons / n
                    
                    n_results.append({
                        'n': n,
                        'run': run + 1,
                        'total_comparisons': total_comparisons,
                        'total_operations': total_operations,
                        'avg_cost_per_operation': avg_cost,
                        'avg_cost_per_n': avg_cost_per_n
                    })
            
            if n_results:
                # Calculate average across runs for this n
                avg_total_comparisons = np.mean([r['total_comparisons'] for r in n_results])
                avg_cost_per_operation = np.mean([r['avg_cost_per_operation'] for r in n_results])
                avg_cost_per_n = np.mean([r['avg_cost_per_n'] for r in n_results])
                std_cost_per_n = np.std([r['avg_cost_per_n'] for r in n_results])
                
                results.append({
                    'n': n,
                    'avg_total_comparisons': avg_total_comparisons,
                    'avg_cost_per_operation': avg_cost_per_operation,
                    'avg_cost_per_n': avg_cost_per_n,
                    'std_cost_per_n': std_cost_per_n
                })
        
        # Create DataFrame and plot results
        results_df = pd.DataFrame(results)
        
        # Plot 1: Total comparisons vs n
        plt.figure(figsize=(15, 5))
        
        plt.subplot(1, 3, 1)
        plt.plot(results_df['n'], results_df['avg_total_comparisons'], 'bo-', linewidth=2)
        plt.xlabel('n')
        plt.ylabel('Average Total Comparisons')
        plt.title('Total Comparisons vs n')
        plt.grid(True, alpha=0.3)
        
        # Plot 2: Average cost per operation vs n
        plt.subplot(1, 3, 2)
        plt.plot(results_df['n'], results_df['avg_cost_per_operation'], 'ro-', linewidth=2)
        plt.xlabel('n')
        plt.ylabel('Average Comparisons per Operation')
        plt.title('Average Cost per Operation vs n')
        plt.grid(True, alpha=0.3)
        
        # Plot 3: Average cost per n (normalized)
        plt.subplot(1, 3, 3)
        plt.errorbar(results_df['n'], results_df['avg_cost_per_n'], 
                    yerr=results_df['std_cost_per_n'], 
                    fmt='go-', linewidth=2, capsize=5)
        plt.xlabel('n')
        plt.ylabel('Total Comparisons / n')
        plt.title('Normalized Cost vs n')
        plt.grid(True, alpha=0.3)
        
        plt.tight_layout()
        plt.savefig('experiment2_scaling_analysis.png', dpi=300, bbox_inches='tight')
        plt.show()
        
        # Display results table
        print("\nScaling Results:")
        print(results_df.to_string(index=False, float_format='%.2f'))
        
        # Save results
        results_df.to_csv('scaling_results.csv', index=False)
        
        return results_df
    
    def analyze_complexity(self, results_df):
        """
        Analyze the theoretical complexity
        """
        print("\n=== Complexity Analysis ===")
        
        # Expected theoretical complexity for binomial heap operations:
        # Insert: O(log n) amortized
        # Extract-Min: O(log n)
        # Union: O(log n)
        
        n_vals = results_df['n'].values
        avg_costs = results_df['avg_cost_per_operation'].values
        
        # Fit to log(n) curve
        log_n = np.log2(n_vals)
        
        # Linear regression: cost = a * log(n) + b
        coeffs = np.polyfit(log_n, avg_costs, 1)
        a, b = coeffs
        
        print(f"Fitted curve: avg_cost ≈ {a:.2f} * log₂(n) + {b:.2f}")
        
        # Plot the fit
        plt.figure(figsize=(10, 6))
        plt.scatter(n_vals, avg_costs, color='blue', s=50, alpha=0.7, label='Actual Data')
        
        # Plot theoretical curve
        n_theory = np.linspace(min(n_vals), max(n_vals), 100)
        log_n_theory = np.log2(n_theory)
        cost_theory = a * log_n_theory + b
        plt.plot(n_theory, cost_theory, 'r-', linewidth=2, label=f'Fitted: {a:.2f}log₂(n) + {b:.2f}')
        
        plt.xlabel('n')
        plt.ylabel('Average Comparisons per Operation')
        plt.title('Complexity Analysis: Average Cost vs n')
        plt.legend()
        plt.grid(True, alpha=0.3)
        plt.savefig('complexity_analysis.png', dpi=300, bbox_inches='tight')
        plt.show()
        
        # Calculate R²
        y_pred = a * log_n + b
        ss_res = np.sum((avg_costs - y_pred) ** 2)
        ss_tot = np.sum((avg_costs - np.mean(avg_costs)) ** 2)
        r_squared = 1 - (ss_res / ss_tot)
        print(f"R² value: {r_squared:.4f}")
    
    def cleanup(self):
        """Clean up generated files"""
        # Remove CSV files
        for file in Path('.').glob('experiment_*.csv'):
            file.unlink()
        
        # Remove executable
        if os.path.exists(self.executable):
            os.remove(self.executable)

def main():
    """Main function to run all experiments"""
    print("Binomial Heap Experiment Suite")
    print("=" * 50)
    
    # Initialize tester
    tester = BinomialHeapTester()
    
    try:
        # Experiment 1: Multiple runs with n=500
        exp1_data = tester.experiment_1_multiple_runs(n=500, num_experiments=5)
        
        # Experiment 2: Scaling analysis
        n_values = list(range(100, 1100, 100))  # Can be modified for different ranges
        scaling_results = tester.experiment_2_scaling(n_values=n_values, num_runs_per_n=3)
        
        # Complexity analysis
        tester.analyze_complexity(scaling_results)
        
        print("\n" + "=" * 50)
        print("All experiments completed successfully!")
        print("Generated files:")
        print("- experiment1_n500_historical_plots.png")
        print("- experiment2_scaling_analysis.png")
        print("- complexity_analysis.png")
        print("- scaling_results.csv")
        
    except Exception as e:
        print(f"Error during experiments: {e}")
    finally:
        # Cleanup
        cleanup_choice = input("\nClean up temporary files? (y/n): ").lower()
        if cleanup_choice == 'y':
            tester.cleanup()
            print("Cleanup completed.")

if __name__ == "__main__":
    main()