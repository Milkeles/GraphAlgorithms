// Hristo Hristov
#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include <filesystem>
#include <set>

class GraphGenerator {
private:
    int N; // Number of nodes
    double D; // Density
    bool allow_negative_weights;
    std::vector<std::vector<std::pair<int, int>>> adj_list; // Adjacency list (neighbor, weight)

    // Random number generators
    std::mt19937 rng;
    std::uniform_real_distribution<double> dist_prob;
    std::uniform_int_distribution<int> dist_weight_pos;
    std::uniform_int_distribution<int> dist_weight_neg;

    // DFS to check if graph is connected
    void dfs(int v, std::vector<bool>& visited) {
        visited[v] = true;
        for (const auto& [u, w] : adj_list[v]) {
            if (!visited[u]) {
                dfs(u, visited);
            }
        }
    }

    // Check if the graph is connected
    bool is_connected() {
        std::vector<bool> visited(N, false);
        dfs(0, visited);
        for (bool v : visited) {
            if (!v) return false;
        }
        return true;
    }

public:
    GraphGenerator(int nodes, double density, bool neg_weights)
        : N(nodes), D(density), allow_negative_weights(neg_weights),
          rng(static_cast<unsigned>(time(0))),
          dist_prob(0.0, 1.0),
          dist_weight_pos(1, 10),
          dist_weight_neg(-10, 10) {
        adj_list.resize(N);
    }

    // Generate a connected random graph
    void generate_graph() {
        while (true) {
            // Clear the previous graph
            for (auto& neighbors : adj_list) {
                neighbors.clear();
            }

            // Generate edges using Erdős–Rényi model
            for (int u = 0; u < N; ++u) {
                for (int v = u + 1; v < N; ++v) {
                    if (dist_prob(rng) < D) {
                        int weight;
                        if (allow_negative_weights) {
                            do {
                                weight = dist_weight_neg(rng);
                            } while (weight == 0); // Avoid zero weights
                        } else {
                            weight = dist_weight_pos(rng);
                        }
                        adj_list[u].push_back({v, weight});
                        adj_list[v].push_back({u, weight}); // Undirected graph
                    }
                }
            }

            // Check if the graph is connected
            if (is_connected()) {
                break;
            }
        }
    }

    // Save the graph to a file in the current directory
    void save_to_file(const std::string& output_dir, int graph_id) {
        // Construct filename (output_dir is "." for current directory)
        std::string filename = "graph_N" + std::to_string(N) +
                              "_D" + std::to_string(D) +
                              "_neg" + (allow_negative_weights ? "true" : "false") +
                              "_" + std::to_string(graph_id) + ".txt";

        std::ofstream outfile(filename);
        if (!outfile) {
            std::cerr << "Error: Could not open file " << filename << std::endl;
            return;
        }

        // Write number of nodes
        outfile << N << "\n";

        // Write edges as edge list: source target weight
        std::set<std::pair<int, int>> written_edges; // To avoid duplicates
        for (int u = 0; u < N; ++u) {
            for (const auto& [v, weight] : adj_list[u]) {
                if (u < v) { // Only write each edge once (u < v)
                    outfile << u << " " << v << " " << weight << "\n";
                }
            }
        }

        outfile.close();
        std::cout << "Generated graph saved to " << filename << std::endl;
    }
};

int main() {
    // Parameters for size tests (fixed D = 0.5)
    std::vector<std::pair<int, double>> size_tests = {
        {100, 0.1},    // Small
        {1000, 0.1},   // Medium
        {10000, 0.1}   // Large
    };

    // Parameters for density tests (fixed N = 1000)
    std::vector<std::pair<int, double>> density_tests = {
        {1000, 0.1},   // Sparse
        {1000, 0.5},   // Medium
        {1000, 0.9}    // Dense
    };

    bool allow_negative_weights = false; // Toggle this to true for negative weights
    std::string output_dir = "."; // Save in the current directory

    // Generate graphs for size tests (1 graph per configuration)
    for (const auto& [N, D] : size_tests) {
        GraphGenerator generator(N, D, allow_negative_weights);
        generator.generate_graph();
        generator.save_to_file(output_dir, 1);
    }

    // Generate graphs for density tests (1 graph per configuration)
    for (const auto& [N, D] : density_tests) {
        GraphGenerator generator(N, D, allow_negative_weights);
        generator.generate_graph();
        generator.save_to_file(output_dir, 1);
    }

    return 0;
}