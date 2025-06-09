import numpy as np
import matplotlib.pyplot as plt
from sklearn.decomposition import PCA
import pandas as pd
import random

def load_data(emnist_file, dbscan_results_file):
    """
    Loads EMNIST feature data and DBSCAN cluster assignments.

    Args:
        emnist_file (str): Path to the EMNIST data file (e.g., 'emnist_data.txt').
        dbscan_results_file (str): Path to the DBSCAN cluster assignments file (e.g., 'dbscan_results.txt').

    Returns:
        tuple: A tuple containing:
            - features (np.ndarray): The loaded EMNIST features.
            - labels (np.ndarray): The original EMNIST labels.
            - cluster_assignments (np.ndarray): DBSCAN cluster assignments.
    """
    print(f"Loading EMNIST data from {emnist_file}...")
    features_list = []
    labels_list = []
    with open(emnist_file, 'r') as f:
        for line in f:
            try:
                parts = line.strip().split('|')
                feature_str = parts[0]
                label_str = parts[1]

                features_list.append([float(x) for x in feature_str.split(',')])
                labels_list.append(int(label_str))
            except (ValueError, IndexError) as e:
                print(f"Skipping malformed line in {emnist_file}: {line.strip()} - Error: {e}")
                continue
    features = np.array(features_list)
    labels = np.array(labels_list)
    print(f"Loaded {len(features)} samples from {emnist_file}.")

    print(f"Loading DBSCAN results from {dbscan_results_file}...")
    cluster_assignments = []
    with open(dbscan_results_file, 'r') as f:
        # Skip the header line
        f.readline()
        for line in f:
            try:
                cluster_assignments.append(int(line.strip()))
            except ValueError as e:
                print(f"Skipping malformed line in {dbscan_results_file}: {line.strip()} - Error: {e}")
                continue
    cluster_assignments = np.array(cluster_assignments)
    print(f"Loaded {len(cluster_assignments)} cluster assignments from {dbscan_results_file}.")

    # Ensure sizes match, adjust if necessary (due to potential malformed lines)
    min_len = min(len(features), len(cluster_assignments))
    features = features[:min_len]
    labels = labels[:min_len]
    cluster_assignments = cluster_assignments[:min_len]

    return features, labels, cluster_assignments

def visualize_dbscan(features, labels, cluster_assignments):
    """
    Visualizes DBSCAN clustering results using PCA for dimensionality reduction.

    Args:
        features (np.ndarray): Original high-dimensional features.
        labels (np.ndarray): Original true labels.
        cluster_assignments (np.ndarray): Cluster IDs assigned by DBSCAN.
    """
    print("Applying PCA to reduce dimensionality to 2D...")
    pca = PCA(n_components=2)
    features_2d = pca.fit_transform(features)
    print(f"Explained variance ratio by 2 components: {pca.explained_variance_ratio_.sum():.2f}")

    unique_clusters = np.unique(cluster_assignments)
    num_clusters = len(unique_clusters) - 1  # Exclude noise (cluster_id 0)

    if num_clusters > 0:
        cmap = plt.get_cmap('tab20', num_clusters)  # 'tab20' has 20 distinct colors, good for up to 20 clusters
        cluster_colors = {c_id: cmap(idx % cmap.N) for idx, c_id in enumerate(
            sorted(unique_clusters) if 0 not in unique_clusters else sorted([c for c in unique_clusters if c != 0]))}
        # Special color for noise
        noise_color = 'lightgray'  # Or 'black', 'darkgray'
        if 0 in unique_clusters:
            cluster_colors[0] = noise_color
    else:
        print("No clusters found (only noise). Skipping visualization.")
        return

    print("Plotting clusters...")
    for cluster_id in sorted(unique_clusters):
        mask = (cluster_assignments == cluster_id)
        if cluster_id == 0: # Noise
            plt.scatter(features_2d[mask, 0], features_2d[mask, 1],
                        s=5, color=cluster_colors[cluster_id], label=f'Noise (Cluster 0)', alpha=0.5)
        else:
            plt.scatter(features_2d[mask, 0], features_2d[mask, 1],
                        s=10, color=cluster_colors[cluster_id], label=f'Cluster {cluster_id}', alpha=0.7)

            # Optional: Calculate and display majority label for this cluster
            # This can get crowded if many clusters are close, so it's commented by default.
            # You can uncomment and adjust its position/size if needed.
            # cluster_labels = labels[mask]
            # if len(cluster_labels) > 0:
            #     majority_label = pd.Series(cluster_labels).mode()[0]
            #     # Find an approximate center for annotation (average of points)
            #     center_x = np.mean(features_2d[mask, 0])
            #     center_y = np.mean(features_2d[mask, 1])
            #     plt.text(center_x, center_y, str(majority_label),
            #              fontsize=10, color='black', ha='center', va='center',
            #              bbox=dict(facecolor='white', alpha=0.7, edgecolor='none', boxstyle='round,pad=0.2'))


    plt.title('DBSCAN Clustering of EMNIST Digits (PCA to 2D)')
    plt.xlabel(f'Principal Component 1 (explains {pca.explained_variance_ratio_[0]*100:.1f}%)')
    plt.ylabel(f'Principal Component 2 (explains {pca.explained_variance_ratio_[1]*100:.1f}%)')
    plt.legend(bbox_to_anchor=(1.05, 1), loc='upper left', borderaxespad=0.)
    plt.grid(True, linestyle='--', alpha=0.6)
    plt.tight_layout()
    plt.savefig('dbscan_emnist_visualization.png', dpi=300)
    print("Visualization complete.")

if __name__ == "__main__":
    emnist_data_file = "emnist_data.txt"
    dbscan_results_file = "dbscan_results.txt"

    # Make sure the C++ program has generated these files first!
    features, labels, cluster_assignments = load_data(emnist_data_file, dbscan_results_file)

    # Only visualize if data was loaded successfully and has enough samples
    if features.shape[0] > 0 and cluster_assignments.shape[0] > 0:
        visualize_dbscan(features, labels, cluster_assignments)
    else:
        print("Not enough data to visualize. Please ensure 'emnist_data.txt' and 'dbscan_results.txt' are correctly populated.")