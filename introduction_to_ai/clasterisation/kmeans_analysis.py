import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
import pandas as pd

# Load centroids and cluster assignments
with open("kmeans_results.txt", "r") as file:
    lines = file.readlines()

# Parse centroids
centroids_start = lines.index("Centroids:\n") + 1
assignments_start = lines.index("Cluster Assignments:\n") + 1

centroids = np.array([list(map(float, line.strip().split(','))) for line in lines[centroids_start:assignments_start - 1]])
assignments = np.array([int(line.strip()) for line in lines[assignments_start:]])

# Load label-cluster mapping
label_cluster_mapping = []
with open("cluster_labels.txt", "r") as file:
    for line in file:
        parts = line.strip().split(", ")
        label = int(parts[0].split(": ")[1])
        cluster = int(parts[1].split(": ")[1])
        label_cluster_mapping.append((label, cluster))

# Create a DataFrame for label-cluster mapping
df = pd.DataFrame(label_cluster_mapping, columns=["Label", "Cluster"])

# Create a heatmap for label distribution across clusters
heatmap_data = df.groupby(["Label", "Cluster"]).size().unstack(fill_value=0)
plt.figure(figsize=(10, 8))
sns.heatmap(heatmap_data, annot=True, fmt="d", cmap="YlGnBu")
plt.title("Label Distribution Across Clusters")
plt.xlabel("Cluster")
plt.ylabel("Label")
plt.savefig("label_distribution_heatmap.png")

# Visualize centroids
plt.figure(figsize=(15, 5))
for i, centroid in enumerate(centroids):
    plt.subplot(1, len(centroids), i + 1)
    plt.imshow(centroid.reshape(28, 28), cmap='gray')
    plt.title(f"Cluster {i}")
plt.axis('off')
plt.savefig("centroids_visualization.png")