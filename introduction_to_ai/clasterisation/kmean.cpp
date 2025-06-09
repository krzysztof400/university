#include <vector>
#include <cmath>
#include <limits>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm> // For std::min_element

const int NUM_SAMPLES = 60000; // Maximum number of samples
const int NUM_FEATURES = 784;  // Number of features per sample

// Structure to hold K-Means results
struct KMeansResult {
    std::vector<std::vector<float>> centroids;
    std::vector<int> cluster_assignments;
    float inertia;
    int iterations;
};

// Function to calculate the Euclidean distance between two points
float euclidean_distance(const float* point1, const float* point2, int dimensions) {
    float distance_sq = 0.0f;
    for (int i = 0; i < dimensions; i++) {
        distance_sq += (point1[i] - point2[i]) * (point1[i] - point2[i]);
    }
    return std::sqrt(distance_sq);
}

// K-Means function
KMeansResult kmeans(float features[][NUM_FEATURES], int num_samples, int k, int max_iterations) {
    // Randomly initialize centroids
    std::vector<std::vector<float>> centroids(k, std::vector<float>(NUM_FEATURES, 0.0f));
    for (int i = 0; i < k; i++) {
        int random_index = std::rand() % num_samples;
        for (int j = 0; j < NUM_FEATURES; j++) {
            centroids[i][j] = features[random_index][j];
        }
    }

    std::vector<int> cluster_assignments(num_samples, -1); // Cluster assignments for each sample
    bool converged = false;
    int iterations = 0;

    while (!converged && iterations < max_iterations) {
        converged = true;

        // Step 1: Assign each sample to the nearest centroid
        for (int i = 0; i < num_samples; i++) {
            float min_distance = std::numeric_limits<float>::max();
            int closest_centroid = -1;

            for (int j = 0; j < k; j++) {
                float distance = euclidean_distance(features[i], centroids[j].data(), NUM_FEATURES);
                if (distance < min_distance) {
                    min_distance = distance;
                    closest_centroid = j;
                }
            }

            if (cluster_assignments[i] != closest_centroid) {
                converged = false;
                cluster_assignments[i] = closest_centroid;
            }
        }

        // Step 2: Update centroids based on the mean of assigned samples
        std::vector<std::vector<float>> new_centroids(k, std::vector<float>(NUM_FEATURES, 0.0f));
        std::vector<int> cluster_sizes(k, 0);

        for (int i = 0; i < num_samples; i++) {
            int cluster = cluster_assignments[i];
            cluster_sizes[cluster]++;
            for (int j = 0; j < NUM_FEATURES; j++) {
                new_centroids[cluster][j] += features[i][j];
            }
        }

        for (int j = 0; j < k; j++) {
            if (cluster_sizes[j] > 0) {
                for (int d = 0; d < NUM_FEATURES; d++) {
                    new_centroids[j][d] /= cluster_sizes[j];
                }
            }
            // Handle empty clusters (optional: re-initialize them)
            else {
                // For simplicity, we'll leave empty centroids as they are for now.
                // In a robust implementation, you might re-initialize them to a random point.
            }
        }
        centroids = new_centroids;
        iterations++;
    }

    // Calculate inertia
    float inertia = 0.0f;
    for (int i = 0; i < num_samples; i++) {
        int cluster = cluster_assignments[i];
        inertia += std::pow(euclidean_distance(features[i], centroids[cluster].data(), NUM_FEATURES), 2);
    }

    return {centroids, cluster_assignments, inertia, iterations};
}

int main(int argc, char* argv[]) {
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

    // Output the parsed data for verification
    std::cout << "Parsed " << sample_index << " samples." << std::endl;

    const int k = (argc > 1) ? std::stoi(argv[1]) : 10; // Number of clusters
    const int max_iterations = 100; // Maximum number of iterations
    const int num_runs = 5; // Number of times to repeat K-Means

    KMeansResult best_result;
    best_result.inertia = std::numeric_limits<float>::max(); // Initialize with a very large value

    std::cout << "\nRunning K-Means " << num_runs << " times to find the best inertia..." << std::endl;

    for (int run = 0; run < num_runs; ++run) {
        std::cout << "Run " << (run + 1) << "..." << std::endl;
        KMeansResult current_result = kmeans(features, sample_index, k, max_iterations);
        std::cout << "Run " << (run + 1) << " completed in " << current_result.iterations << " iterations with inertia: " << current_result.inertia << std::endl;

        if (current_result.inertia < best_result.inertia) {
            best_result = current_result;
        }
    }

    // Save results of the best run to files
    const std::string output_file = "kmeans_results.txt";
    std::ofstream output(output_file);
    if (!output.is_open()) {
        std::cerr << "Error: Could not open output file " << output_file << std::endl;
        delete[] features;
        delete[] labels;
        return 1;
    }

    // Save centroids
    output << "Centroids:\n";
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < NUM_FEATURES; j++) {
            output << best_result.centroids[i][j];
            if (j < NUM_FEATURES - 1) output << ",";
        }
        output << "\n";
    }

    // Save cluster assignments
    output << "Cluster Assignments:\n";
    for (int i = 0; i < sample_index; i++) {
        output << best_result.cluster_assignments[i] << "\n";
    }

    output.close();

    // Save label-cluster mapping to a separate file
    std::ofstream label_cluster_file("cluster_labels.txt");
    if (!label_cluster_file.is_open()) {
        std::cerr << "Error: Could not open file cluster_labels.txt" << std::endl;
        delete[] features;
        delete[] labels;
        return 1;
    }

    for (int i = 0; i < sample_index; i++) {
        label_cluster_file << "Label: " << labels[i] << ", Cluster: " << best_result.cluster_assignments[i] << "\n";
    }

    label_cluster_file.close();

    std::cout << "\nK-Means clustering (best run) completed in " << best_result.iterations << " iterations with inertia: " << best_result.inertia << std::endl;
    std::cout << "Results saved to " << output_file << " and cluster_labels.txt" << std::endl;

    // Free allocated memory
    delete[] features;
    delete[] labels;

    return 0;
}