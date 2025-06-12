import random
import time
import matplotlib.pyplot as plt
import numpy as np
from statistics import mean

def binary_search_recursive(arr, v, left, right, comparisons=0):
    """Recursive binary search. Returns (found, comparisons) where found is 1 if v is in arr, 0 otherwise."""
    if left > right:
        return 0, comparisons
    
    mid = (left + right) // 2
    comparisons += 1  # Comparison with arr[mid]
    
    if arr[mid] == v:
        return 1, comparisons
    elif arr[mid] > v:
        return binary_search_recursive(arr, v, left, mid - 1, comparisons + 1)  # Comparison in condition
    else:
        return binary_search_recursive(arr, v, mid + 1, right, comparisons + 1)  # Comparison in condition

def generate_sorted_array(n):
    """Generate a sorted array of n unique integers."""
    return sorted(random.sample(range(-10 * n, 10 * n), n))

def run_experiment(n, v_selector, trials=1):
    """Run binary search experiment for given size n and value selector, averaging over trials."""
    total_comparisons = 0
    total_time = 0
    arr = generate_sorted_array(n)
    
    for _ in range(trials):
        v = v_selector(arr, n)
        start_time = time.time()
        found, comparisons = binary_search_recursive(arr, v, 0, n - 1)
        end_time = time.time()
        total_comparisons += comparisons
        total_time += (end_time - start_time) * 1e6  # Convert to microseconds
    
    return total_comparisons / trials, total_time / trials

def select_beginning(arr, n):
    """Select a value near the beginning (first 10% of array)."""
    window = max(1, n // 10)
    return arr[random.randint(0, window - 1)]

def select_middle(arr, n):
    """Select a value near the middle (middle 10% of array)."""
    mid = n // 2
    window = max(1, n // 10)
    start = max(0, mid - window // 2)
    end = min(n, mid + window // 2)
    return arr[random.randint(start, end - 1)]

def select_end(arr, n):
    """Select a value near the end (last 10% of array)."""
    window = max(1, n // 10)
    return arr[random.randint(n - window, n - 1)]

def select_random(arr, n):
    """Select a random value from the array."""
    return random.choice(arr)

def select_outside(arr, n):
    """Select a value not in the array."""
    return max(arr) + 1

def main():
    # Array sizes for testing
    sizes = list(range(1000, 100001, 1000))
    selectors = {
        'beginning': select_beginning,
        'middle': select_middle,
        'end': select_end,
        'random': select_random,
        'outside': select_outside
    }
    trials = 1000  # Number of trials for random selection to average results
    results = {name: {'sizes': [], 'comparisons': [], 'times': []} for name in selectors}
    
    # Run experiments
    for n in sizes:
        for name, selector in selectors.items():
            num_trials = trials if name == 'random' else 1
            avg_comparisons, avg_time = run_experiment(n, selector, num_trials)
            results[name]['sizes'].append(n)
            results[name]['comparisons'].append(avg_comparisons)
            results[name]['times'].append(avg_time)
    
    # Master Theorem estimate: T(n) = T(n/2) + O(1) => O(log n)
    # Expected comparisons: ~log2(n) (best case for middle, worst case for outside/end)
    theoretical_comparisons = [np.log2(n) for n in sizes]
    
    # Estimate O(1) constant factor
    constant_factors = {name: {'comparisons': [], 'times': []} for name in selectors}
    for name in selectors:
        for i, n in enumerate(sizes):
            # Constant factor for comparisons: actual_comparisons / log2(n)
            constant_factors[name]['comparisons'].append(results[name]['comparisons'][i] / np.log2(n))
            # Constant factor for time: actual_time / log2(n)
            constant_factors[name]['times'].append(results[name]['times'][i] / np.log2(n))
    
    # Plotting
    # 1. Comparisons vs Input Size
    plt.figure(figsize=(12, 6))
    for name in selectors:
        plt.plot(results[name]['sizes'], results[name]['comparisons'], label=name, marker='.')
    plt.plot(sizes, theoretical_comparisons, label='log2(n) (Theoretical)', linestyle='--')
    plt.title('Average Number of Comparisons vs Input Size')
    plt.xlabel('Input Size (n)')
    plt.ylabel('Comparisons')
    plt.legend()
    plt.grid(True)
    plt.savefig('comparisons_vs_size.png')
    plt.show()
    
    # 2. Execution Time vs Input Size
    plt.figure(figsize=(12, 6))
    for name in selectors:
        plt.plot(results[name]['sizes'], results[name]['times'], label=name, marker='.')
    plt.title('Average Execution Time vs Input Size')
    plt.xlabel('Input Size (n)')
    plt.ylabel('Time (microseconds)')
    plt.legend()
    plt.grid(True)
    plt.savefig('time_vs_size.png')
    plt.show()
    
    # 3. Constant Factor for Comparisons
    plt.figure(figsize=(12, 6))
    for name in selectors:
        plt.plot(sizes, constant_factors[name]['comparisons'], label=name, marker='.')
    plt.title('Constant Factor for Comparisons vs Input Size')
    plt.xlabel('Input Size (n)')
    plt.ylabel('Constant Factor (Comparisons / log2(n))')
    plt.legend()
    plt.grid(True)
    plt.savefig('constant_factor_comparisons.png')
    plt.show()
    
    # 4. Constant Factor for Time
    plt.figure(figsize=(12, 6))
    for name in selectors:
        plt.plot(sizes, constant_factors[name]['times'], label=name, marker='.')
    plt.title('Constant Factor for Execution Time vs Input Size')
    plt.xlabel('Input Size (n)')
    plt.ylabel('Constant Factor (Time / log2(n))')
    plt.legend()
    plt.grid(True)
    plt.savefig('constant_factor_time.png')
    plt.show()
    
    # Demonstrate correctness on small data
    print("\nDemonstrating correctness on small data:")
    arr = [1, 3, 5, 7, 9]
    test_values = [3, 8]
    for v in test_values:
        found, comparisons = binary_search_recursive(arr, v, 0, len(arr) - 1)
        print(f"Array: {arr}, Search for {v}: Found={found}, Comparisons={comparisons}")

if __name__ == "__main__":
    main()