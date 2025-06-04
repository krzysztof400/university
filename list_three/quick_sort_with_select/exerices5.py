import matplotlib.pyplot as plt
import re
import subprocess
import sys

def parse_output(output):
    comparisons_match = re.search(r'Number of comparisons: (\d+)', output)
    swaps_match = re.search(r'Number of swaps: (\d+)', output)
    
    if comparisons_match and swaps_match:
        comparisons = int(comparisons_match.group(1))
        swaps = int(swaps_match.group(1))
        return comparisons, swaps
    else:
        return None

def sort_n_elements(n, generator_path, sort_path):
    try:
        # Run generator and capture its output
        generator_result = subprocess.run(
            [generator_path, str(n)], 
            stdout=subprocess.PIPE, 
            text=True,
            check=True
        )
        generated_data = generator_result.stdout
        
        # Pass the generated data to the sorting algorithm
        sort_result = subprocess.run(
            [sort_path], 
            input=generated_data, 
            stdout=subprocess.PIPE, 
            text=True,
            check=True
        )
        
        # Combine outputs for parsing
        combined_output = generated_data + sort_result.stdout
        
        result = parse_output(combined_output)
        return result
    except subprocess.CalledProcessError as e:
        print(f"Error running subprocess: {e}")
        return None
    except Exception as e:
        print(f"Unexpected error: {e}")
        return None

# Define available generators and sorts
generator_options = {
    'r': './generator_random',
    'd': './generator_descending',
    'a': './ascending_generator'
}

sorts = ["./DP_quick_sort_with_select", "./DP_quick_sort", "./quick_sort_with_select", "./quick_sort"]

# Get user input for parameters
try:
    k = int(input("Enter the number of iterations for averaging: "))
    gap = int(input("Enter the gap for the number of elements: "))
    
    # Generator selection
    print("\nSelect a random number generator:")
    print("r - Random generator")
    print("d - Descending generator")
    print("a - Ascending generator")
    print("all - All generators")
    
    generator_choice = input("Enter your choice (r/d/a/all): ").strip().lower()
    
    # Determine which generators to use
    if generator_choice == 'all':
        generators = list(generator_options.values())
    elif generator_choice in generator_options:
        generators = [generator_options[generator_choice]]
    else:
        print(f"Invalid generator choice: {generator_choice}")
        print("Using random generator as default")
        generators = [generator_options['r']]
        
    # Confirm selected generators to the user
    gen_names = [g.split('/')[-1] for g in generators]
    print(f"Using generator(s): {', '.join(gen_names)}")
    
except ValueError:
    print("Please enter valid integers")
    sys.exit(1)

comparisons = []
swaps = []

# Collect data for different input sizes
for i in range(1, 101):  # Loop through 9 different input sizes
    n = i * gap
    for generator in generators:
        for sort in sorts:       
            total_comparisons = 0
            total_swaps = 0
            successful_runs = 0
            
            # Run k iterations for averaging
            for j in range(k):
                comparisons_swaps = sort_n_elements(n, generator, sort)
                if comparisons_swaps:
                    total_comparisons += comparisons_swaps[0]
                    total_swaps += comparisons_swaps[1]
                    successful_runs += 1
            
            # Calculate averages if we had successful runs
            if successful_runs > 0:
                avg_comparisons = total_comparisons / successful_runs
                avg_swaps = total_swaps / successful_runs
                comparisons.append((n, generator, sort, avg_comparisons))
                swaps.append((n, generator, sort, avg_swaps))
            else:
                print(f"Warning: No successful runs for n={n}, {generator}, {sort}")

# Calculate average comparisons and swaps across all sorting algorithms
n_values = sorted(set(c[0] for c in comparisons))
avg_comparisons = []
avg_swaps = []

for n in n_values:
    n_comparisons = [c[3] for c in comparisons if c[0] == n]
    n_swaps = [s[3] for s in swaps if s[0] == n]
    
    if n_comparisons:
        avg_comparisons.append((n, sum(n_comparisons) / len(n_comparisons)))
    if n_swaps:
        avg_swaps.append((n, sum(n_swaps) / len(n_swaps)))

# Calculate ratios (operation count divided by number of elements)
ratios_comparisons = [(n, avg_comp / n) for n, avg_comp in avg_comparisons]
ratios_swaps = [(n, avg_swap / n) for n, avg_swap in avg_swaps]

# Create 2x2 plot layout
fig, axs = plt.subplots(2, 2, figsize=(15, 10))

# Define colors and markers for different algorithms and generators
colors = {'./DP_quick_sort_with_select': 'blue', './DP_quick_sort': 'red', 
          './quick_sort_with_select': 'green', './quick_sort': 'orange'}
markers = {'./generator_random': 'o', './generator_descending': 's'}
linestyles = {'./generator_random': '-', './generator_descending': '--'}

# Subplot 1: Comparisons
axs[0, 0].set_title("Number of Comparisons", fontsize=14)
axs[0, 0].set_xlabel("Number of Elements", fontsize=12)
axs[0, 0].set_ylabel("Comparisons", fontsize=12)

# Subplot 2: Swaps
axs[0, 1].set_title("Number of Swaps", fontsize=14)
axs[0, 1].set_xlabel("Number of Elements", fontsize=12)
axs[0, 1].set_ylabel("Swaps", fontsize=12)

# Subplot 3: Comparison Ratios
axs[1, 0].set_title("Comparison Ratio (Comparisons/n)", fontsize=14)
axs[1, 0].set_xlabel("Number of Elements", fontsize=12)
axs[1, 0].set_ylabel("Ratio", fontsize=12)

# Subplot 4: Swap Ratios
axs[1, 1].set_title("Swap Ratio (Swaps/n)", fontsize=14)
axs[1, 1].set_xlabel("Number of Elements", fontsize=12)
axs[1, 1].set_ylabel("Ratio", fontsize=12)

# Plot data for each sorting algorithm and generator combination
for sort in sorts:
    for generator in generators:
        # Get the data for this combination
        sort_gen_comparisons = [(c[0], c[3]) for c in comparisons if c[1] == generator and c[2] == sort]
        sort_gen_swaps = [(s[0], s[3]) for s in swaps if s[1] == generator and s[2] == sort]
        
        # Skip if no data for this combination
        if not sort_gen_comparisons or not sort_gen_swaps:
            continue
            
        # Sort by number of elements
        sort_gen_comparisons.sort()
        sort_gen_swaps.sort()
        
        # Extract x and y values
        x_comp = [c[0] for c in sort_gen_comparisons]
        y_comp = [c[1] for c in sort_gen_comparisons]
        
        x_swap = [s[0] for s in sort_gen_swaps]
        y_swap = [s[1] for s in sort_gen_swaps]
        
        # Calculate ratios
        comp_ratios = [(x, y/x) for x, y in sort_gen_comparisons]
        swap_ratios = [(x, y/x) for x, y in sort_gen_swaps]
        
        # Extract ratio x and y values
        x_comp_ratio = [r[0] for r in comp_ratios]
        y_comp_ratio = [r[1] for r in comp_ratios]
        
        x_swap_ratio = [r[0] for r in swap_ratios]
        y_swap_ratio = [r[1] for r in swap_ratios]
        
        # Generate label (shorten paths for better readability)
        label = f"{sort}"
        
        # Plot with consistent styling
        color = colors[sort]
        marker = markers[generator]
        linestyle = linestyles[generator]
        
        # Plot comparisons
        axs[0, 0].plot(x_comp, y_comp, label=label, color=color, marker=marker, 
                      linestyle=linestyle, markersize=6)
        
        # Plot swaps
        axs[0, 1].plot(x_swap, y_swap, label=label, color=color, marker=marker,
                      linestyle=linestyle, markersize=6)
        
        # Plot comparison ratios
        axs[1, 0].plot(x_comp_ratio, y_comp_ratio, label=label, color=color, marker=marker,
                      linestyle=linestyle, markersize=6)
        
        # Plot swap ratios
        axs[1, 1].plot(x_swap_ratio, y_swap_ratio, label=label, color=color, marker=marker,
                      linestyle=linestyle, markersize=6)

# Add grid to all subplots
for i in range(2):
    for j in range(2):
        axs[i, j].grid(True, linestyle='--', alpha=0.7)

# Create a single legend for the entire figure
handles, labels = axs[0, 0].get_legend_handles_labels()
fig.legend(handles, labels, loc='upper center', bbox_to_anchor=(0.5, 0.05), 
           ncol=5, fontsize=10, frameon=True, fancybox=True, shadow=True)

# Adjust layout to make room for the legend
plt.tight_layout(rect=[0, 0.1, 1, 0.95])

# Create title based on generator choice
if len(generators) == 1:
    gen_name = generators[0].split('/')[-1].replace('_generator', '')
    title = f'Sorting Algorithm Performance ({gen_name.capitalize()} Input)'
else:
    title = 'Sorting Algorithm Performance Comparison'

# Add a title for the entire figure
fig.suptitle(title, fontsize=16)

plot_name = f"plot_max{gap*10}_k{k}_{'_'.join([sort.split('/')[-1].replace('_sort', '') for sort in sorts])}.png"
plt.savefig(plot_name)
print(f"Plot saved as {plot_name}")

plt.show()