import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import os
import glob

# Set global styling for cleaner plots
sns.set_theme(style="whitegrid")

def plot_simulated_annealing(file_path):
    print(f"Processing {file_path}...")
    df_sa = pd.read_csv(file_path)
    df_sa.columns = df_sa.columns.str.strip()
    df_sa = df_sa.replace('\r', '', regex=True)
    # Extract a base name for the output file and title (e.g., "my_network_sa_grid_results")
    base_name = os.path.basename(file_path).replace('.csv', '')
    
    # Create a figure with 2 subplots (1 row, 2 columns)
    fig, axes = plt.subplots(1, 2, figsize=(16, 6))
    fig.suptitle(f'Simulated Annealing: {base_name}', fontsize=16)

    # Plot 1: Impact of Alpha and Epochs on Best Distance
    sns.lineplot(
        data=df_sa, 
        x='alpha', 
        y='best_dist', 
        hue='epochs_per_temp', 
        marker='o',
        palette='viridis',
        ax=axes[0]
    )
    axes[0].set_title('Impact of Cooling Rate (\u03B1) on Solution Quality')
    axes[0].set_xlabel('Cooling Rate (\u03B1)')
    axes[0].set_ylabel('Best Distance (Lower is Better)')
    axes[0].legend(title='Epochs per Temp')

    # Plot 2: Impact of Alpha and Epochs on Execution Time
    sns.lineplot(
        data=df_sa, 
        x='alpha', 
        y='time_ms', 
        hue='epochs_per_temp', 
        marker='s',
        palette='magma',
        ax=axes[1]
    )
    axes[1].set_title('Execution Time vs. Cooling Rate (\u03B1)')
    axes[1].set_xlabel('Cooling Rate (\u03B1)')
    axes[1].set_ylabel('Time (ms)')
    axes[1].set_yscale('log') # Log scale is often better for time differences in SA
    axes[1].legend(title='Epochs per Temp')

    plt.tight_layout()
    out_file = f'{base_name}_analysis.png'
    plt.savefig(out_file, dpi=300)
    print(f"  -> Saved to '{out_file}'")
    
    # Close the figure to free memory and prevent plots from overlapping in the loop
    plt.close(fig)

def plot_tabu_search(file_path):
    print(f"Processing {file_path}...")
    df_ts = pd.read_csv(file_path)
    df_ts.columns = df_ts.columns.str.strip()
    df_ts = df_ts.replace('\r', '', regex=True)
    # Extract a base name for the output file and title
    base_name = os.path.basename(file_path).replace('.csv', '')

    # Create a figure with 2 subplots (1 row, 2 columns)
    fig, axes = plt.subplots(1, 2, figsize=(16, 6))
    fig.suptitle(f'Tabu Search: {base_name}', fontsize=16)

    # Plot 1: Impact of Tabu List Size on Best Distance
    sns.boxplot(
        data=df_ts, 
        x='tabu_list_size', 
        y='best_dist', 
        hue='max_iterations',
        palette='Set2',
        ax=axes[0]
    )
    axes[0].set_title('Impact of Tabu List Size on Solution Quality')
    axes[0].set_xlabel('Tabu List Size')
    axes[0].set_ylabel('Best Distance (Lower is Better)')
    axes[0].legend(title='Max Iterations')

    # Plot 2: Time vs. Max Iterations grouped by No Improve Limit
    sns.barplot(
        data=df_ts, 
        x='max_iterations', 
        y='time_ms', 
        hue='max_no_improve',
        palette='Blues',
        ax=axes[1],
        errorbar=None 
    )
    axes[1].set_title('Execution Time by Iteration Limits')
    axes[1].set_xlabel('Max Total Iterations')
    axes[1].set_ylabel('Time (ms)')
    axes[1].legend(title='Max Iter. w/o Improv.')

    plt.tight_layout()
    out_file = f'{base_name}_analysis.png'
    plt.savefig(out_file, dpi=300)
    print(f"  -> Saved to '{out_file}'")
    
    # Close the figure to free memory
    plt.close(fig)

if __name__ == "__main__":
    print("Scanning directory for files...")
    
    # Find all csv files containing "sa_grid" or "ts_grid"
    sa_files = glob.glob('*sa_grid*.csv')
    ts_files = glob.glob('*ts_grid*.csv')
    
    if not sa_files and not ts_files:
        print("No files matching '*sa_grid*.csv' or '*ts_grid*.csv' found in the current directory.")
    else:
        for f in sa_files:
            plot_simulated_annealing(f)
            
        for f in ts_files:
            plot_tabu_search(f)
            
        print("\nDone! All visualizations generated.")