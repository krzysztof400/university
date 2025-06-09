#include <vector>
#include <cmath>
#include <limits>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm> 
#include <numeric>  

const int NUM_SAMPLES = 60000; // Maximum number of samples
const int NUM_FEATURES = 784;  // Number of features per sample

// Enum for point states in DBSCAN
enum PointState {
    NOISE = 0,
    NOT_CLASSIFIED = -1,
    CLASSIFIED = -2 // A temporary state to indicate a point is processed into a cluster
};

// Function to calculate the Euclidean distance between two points
float euclidean_distance(const float* point1, const float* point2, int dimensions) {
    float distance_sq = 0.0f;
    for (int i = 0; i < dimensions; i++) {
        distance_sq += (point1[i] - point2[i]) * (point1[i] - point2[i]);
    }
    return std::sqrt(distance_sq);
}

// Function to find neighbors within epsilon distance
std::vector<int> find_neighbors(const float features[][NUM_FEATURES], int num_samples, int point_idx, float eps) {
    std::vector<int> neighbors;
    for (int i = 0; i < num_samples; ++i) {
        if (i == point_idx) continue;
        if (euclidean_distance(features[point_idx], features[i], NUM_FEATURES) <= eps) {
            neighbors.push_back(i);
        }
    }
    return neighbors;
}

// DBSCAN algorithm implementation
std::vector<int> dbscan(const float features[][NUM_FEATURES], int num_samples, float eps, int minPts, int& num_clusters_found) {
    std::vector<int> cluster_assignments(num_samples, NOT_CLASSIFIED);
    num_clusters_found = 0;

    for (int i = 0; i < num_samples; ++i) {
        // Print progress
        std::cout << "\rDBSCAN progress: " << static_cast<int>(static_cast<float>(i) / num_samples * 100.0f) 
                  << "% (" << i << "/" << num_samples << " points processed)" << std::flush;

        if (cluster_assignments[i] != NOT_CLASSIFIED) {
            continue; // Already visited or classified
        }

        std::vector<int> neighbors = find_neighbors(features, num_samples, i, eps);

        if (neighbors.size() < minPts) {
            cluster_assignments[i] = NOISE; // Mark as noise for now
        } else {
            // Core point, create a new cluster
            num_clusters_found++;
            cluster_assignments[i] = num_clusters_found; // Assign to new cluster ID

            std::vector<int> q;
            q.push_back(i); // Add core point to queue

            int head = 0;
            while (head < q.size()) {
                int current_point_idx = q[head++];
                std::vector<int> current_neighbors = find_neighbors(features, num_samples, current_point_idx, eps);

                for (int neighbor_idx : current_neighbors) {
                    if (cluster_assignments[neighbor_idx] == NOT_CLASSIFIED) {
                        cluster_assignments[neighbor_idx] = num_clusters_found; // Assign to current cluster
                        std::vector<int> next_neighbors = find_neighbors(features, num_samples, neighbor_idx, eps);
                        if (next_neighbors.size() >= minPts) {
                            q.push_back(neighbor_idx); // Add to queue if it's a core point too
                        }
                    } else if (cluster_assignments[neighbor_idx] == NOISE) {
                        // Noise point is reachable from a core point, so it's a border point
                        cluster_assignments[neighbor_idx] = num_clusters_found;
                    }
                }
            }
        }
    }
    std::cout << "\rDBSCAN progress: 100% (" << num_samples << "/" << num_samples << " points processed) Complete.\n"; // Final print
    return cluster_assignments;
}

// Function to evaluate DBSCAN results
void evaluate_dbscan(const std::vector<int>& cluster_assignments, const int* labels, int num_samples, int num_clusters_found) {
    int noise_count = 0;
    for (int assignment : cluster_assignments) {
        if (assignment == NOISE) {
            noise_count++;
        }
    }

    float noise_percentage = static_cast<float>(noise_count) / num_samples * 100.0f;
    std::cout << "--- DBSCAN Evaluation ---" << std::endl;
    std::cout << "Total samples: " << num_samples << std::endl;
    std::cout << "Number of clusters found: " << num_clusters_found << std::endl;
    std::cout << "Noise samples: " << noise_count << std::endl;
    std::cout << "Percentage of noise: " << noise_percentage << "%" << std::endl;

    // Evaluate clusters for homogeneity and misclassifications
    int total_correctly_classified_in_clusters = 0;
    int total_misclassified_in_clusters = 0;
    int total_points_in_clusters = 0;

    for (int c = 1; c <= num_clusters_found; ++c) { // Iterate through each cluster ID (starting from 1)
        std::map<int, int> label_counts; // Counts of actual labels within this cluster
        int cluster_size = 0;

        for (int i = 0; i < num_samples; ++i) {
            if (cluster_assignments[i] == c) {
                label_counts[labels[i]]++;
                cluster_size++;
            }
        }

        if (cluster_size == 0) continue; // Skip empty clusters

        total_points_in_clusters += cluster_size;

        int majority_label = -1;
        int max_count = 0;
        for (const auto& pair : label_counts) {
            if (pair.second > max_count) {
                max_count = pair.second;
                majority_label = pair.first;
            }
        }

        int misclassified_in_current_cluster = cluster_size - max_count;
        total_misclassified_in_clusters += misclassified_in_current_cluster;
        total_correctly_classified_in_clusters += max_count;

        std::cout << "\nCluster " << c << " (Size: " << cluster_size << "):" << std::endl;
        std::cout << "  Majority Label: " << majority_label << " (Count: " << max_count << ")" << std::endl;
        std::cout << "  Misclassified in cluster: " << misclassified_in_current_cluster << std::endl;
        std::cout << "  Percentage misclassified in cluster: " << (static_cast<float>(misclassified_in_current_cluster) / cluster_size * 100.0f) << "%" << std::endl;
    }

    float overall_accuracy_in_clusters = 0.0f;
    if (total_points_in_clusters > 0) {
        overall_accuracy_in_clusters = static_cast<float>(total_correctly_classified_in_clusters) / total_points_in_clusters * 100.0f;
    }
    
    float overall_misclassification_in_clusters_percentage = 0.0f;
    if (total_points_in_clusters > 0) {
        overall_misclassification_in_clusters_percentage = static_cast<float>(total_misclassified_in_clusters) / total_points_in_clusters * 100.0f;
    }

    std::cout << "\nOverall Accuracy within Clusters (excluding noise): " << overall_accuracy_in_clusters << "%" << std::endl;
    std::cout << "Overall Misclassification Percentage within Clusters: " << overall_misclassification_in_clusters_percentage << "%" << std::endl;
}

int main() {
    // Allocate memory on the heap
    auto features = new float[NUM_SAMPLES][NUM_FEATURES]; // 2D array for features
    auto labels = new int[NUM_SAMPLES];                  // 1D array for labels

    std::cout << "Starting to parse EMNIST data..." << std::endl;

    std::ifstream file("emnist_data.txt"); // Open the file
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file emnist_data.txt" << std::endl;
        delete[] features; // Free allocated memory
        delete[] labels;
        return 1;
    }

    int sample_index = 0; // To track the current sample index
    std::string line;

    while (std::getline(file, line)) { // Read file line by line
        if (sample_index >= NUM_SAMPLES) {
            std::cerr << "Error: Exceeded maximum number of samples (" << NUM_SAMPLES << ")" << std::endl;
            break;
        }

        std::stringstream ss(line);
        std::string segment;

        // Split the line into features and label
        std::getline(ss, segment, '|'); // Get the features part
        std::stringstream feature_stream(segment);
        std::string feature;
        int feature_index = 0;

        while (std::getline(feature_stream, feature, ',')) {
            if (feature_index >= NUM_FEATURES) {
                std::cerr << "Error: Exceeded maximum number of features (" << NUM_FEATURES << ") on sample " << sample_index << std::endl;
                break;
            }
            features[sample_index][feature_index] = std::stof(feature); // Parse and store feature
            feature_index++;
        }

        if (!std::getline(ss, segment)) { // Get the label part
            std::cerr << "Error: Missing label for sample " << sample_index << std::endl;
            break;
        }
        labels[sample_index] = std::stoi(segment); // Parse and store label

        sample_index++;
    }

    file.close(); // Close the file

    std::cout << "Parsed " << sample_index << " samples." << std::endl;

    // Reduce the dataset size for testing purposes
    const int REDUCED_NUM_SAMPLES = 10000; // Smaller dataset size
    auto reduced_features = new float[REDUCED_NUM_SAMPLES][NUM_FEATURES];
    auto reduced_labels = new int[REDUCED_NUM_SAMPLES];

    // Randomly sample data points from the original dataset
    std::srand(42); // Seed for reproducibility
    std::vector<int> indices(sample_index);
    std::iota(indices.begin(), indices.end(), 0);
    std::random_shuffle(indices.begin(), indices.end());

    for (int i = 0; i < REDUCED_NUM_SAMPLES; ++i) {
        int idx = indices[i];
        std::copy(features[idx], features[idx] + NUM_FEATURES, reduced_features[i]);
        reduced_labels[i] = labels[idx];
    }

    // Update pointers to use the reduced dataset
    delete[] features;
    delete[] labels;
    features = reduced_features;
    labels = reduced_labels;
    sample_index = REDUCED_NUM_SAMPLES;

    // --- DBSCAN Parameters ---
    float eps = 100.0f; // Epsilon: Maximum distance between two samples for one to be considered as in the neighborhood of the other
    int minPts = 5;    // Minimum number of samples in a neighborhood for a point to be considered as a core point

    std::cout << "\nRunning DBSCAN with eps = " << eps << " and minPts = " << minPts << "..." << std::endl;

    int num_clusters_found = 0;
    std::vector<int> cluster_assignments = dbscan(features, sample_index, eps, minPts, num_clusters_found);

    // Save DBSCAN results to a file
    const std::string output_file = "dbscan_results.txt";
    std::ofstream output(output_file);
    if (!output.is_open()) {
        std::cerr << "Error: Could not open output file " << output_file << std::endl;
        delete[] features;
        delete[] labels;
        return 1;
    }

    output << "DBSCAN Cluster Assignments (Noise = 0):\n";
    for (int i = 0; i < sample_index; ++i) {
        output << cluster_assignments[i] << "\n";
    }
    output.close();

    // Save label-cluster mapping to a separate file
    std::ofstream label_cluster_file("dbscan_cluster_labels.txt");
    if (!label_cluster_file.is_open()) {
        std::cerr << "Error: Could not open file dbscan_cluster_labels.txt" << std::endl;
        delete[] features;
        delete[] labels;
        return 1;
    }

    for (int i = 0; i < sample_index; i++) {
        label_cluster_file << "Label: " << labels[i] << ", Cluster: " << cluster_assignments[i] << "\n";
    }
    label_cluster_file.close();

    std::cout << "DBSCAN clustering completed. Results saved to " << output_file << " and dbscan_cluster_labels.txt" << std::endl;

    // Evaluate the results
    evaluate_dbscan(cluster_assignments, labels, sample_index, num_clusters_found);

    // Free allocated memory
    delete[] features;
    delete[] labels;

    return 0;
}
