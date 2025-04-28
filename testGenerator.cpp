/* [Description]
 * This program uses the Erdos-Renyi model to automatically generate random graphs
 *  with predefined density and size then save it into input files for testing purposes.
 *  
 *  Libraries:
 *  - iostream: To print out messages and errors.
 *  - vector: For the vector STL class and its methods
 *  - random: To generate random numbers
 *  - fstream: Writing into files.
 *  - set: For the STL set class and its methods, used to store unique elements.
 *
 *  Author: H. Hristov
 *  Ruse, 2025
 */
#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include <set>

using namespace std;

class GraphGenerator {
private:
    int N; // Number of nodes
    double D; // Density
    bool allowNegativeWeights;
    vector<vector<pair<int, int>>> adjList; // Adjacency list (neighbor, weight)

    // Random number generators
    mt19937 rng;
    uniform_real_distribution<double> distProb;
    uniform_int_distribution<int> distWeightPos;
    uniform_int_distribution<int> distWeightNeg;

    /* DFS to traverse the graph.
     * Used to check if the graoh is connected.
     */
    void dfs(int v, vector<bool>& visited) {
        visited[v] = true;
        for (const auto& [u, w] : adjList[v]) {
            if (!visited[u]) {
                dfs(u, visited);
            }
        }
    }

    // Check if the graph is connected
    bool IsConnected() {
        vector<bool> visited(N, false);
        dfs(0, visited);
        for (bool v : visited) {
            if (!v) return false;
        }
        return true;
    }

public:
    GraphGenerator(int nodes, double density, bool negWeights)
        : N(nodes), D(density), allowNegativeWeights(negWeights),
          rng(static_cast<unsigned>(time(0))),
          distProb(0.0, 1.0),
          distWeightPos(1, 10),
          distWeightNeg(-10, 10) {
        adjList.resize(N);
    }

    // Generate a random connected graph
    void generate_graph() {
        while (true) {
            for (auto& neighbors : adjList) {
                neighbors.clear();
            }

            // Generate edges using Erdos-Renyi model
            for (int u = 0; u < N; ++u) {
                for (int v = u + 1; v < N; ++v) {
                    if (distProb(rng) < D) {
                        int weight;
                        if (allowNegativeWeights) {
                            do {
                                weight = distWeightNeg(rng);
                            } while (weight == 0); // Avoid zero weights
                        } else {
                            weight = distWeightPos(rng);
                        }
                        adjList[u].push_back({v, weight});
                        adjList[v].push_back({u, weight}); // Undirected graph
                    }
                }
            }

            // Check if the graph is connected
            if (IsConnected()) {
                break;
            }
        }
    }

    // Save the graph to a file in the current directory
    void SaveToFile(const string& output_dir, int graph_id) {
        string filename = "graph_N" + to_string(N) +
                              "_D" + to_string(D) +
                              "_neg" + (allowNegativeWeights ? "true" : "false") +
                              "_" + to_string(graph_id) + ".in";

        ofstream outfile(filename);
        if (!outfile) {
            cerr << "Error: Could not open file " << filename << endl;
            return;
        }

        outfile << N << "\n";

        // Write edges as edge list: source target weight
        set<pair<int, int>> written_edges;
        for (int u = 0; u < N; ++u) {
            for (const auto& [v, weight] : adjList[u]) {
                if (u < v) { // Only write each edge once
                    outfile << u << " " << v << " " << weight << "\n";
                }
            }
        }

        outfile.close();
        cout << "Generated graph saved to " << filename << endl;
    }
};

int main() {
    vector<pair<int, double>> sizeTests = {
        {100, 0.1},
        {1000, 0.1},
        {10000, 0.1}
    };

    vector<pair<int, double>> densityTests = {
        {1000, 0.1}, 
        {1000, 0.5},
        {1000, 0.9}
    };

    bool allowNegativeWeights = true; // Toggle this to true for negative weights
    string output_dir = ".";

    for (const auto& [N, D] : sizeTests) {
        GraphGenerator generator(N, D, allowNegativeWeights);
        generator.generate_graph();
        generator.SaveToFile(output_dir, 1);
    }

    for (const auto& [N, D] : densityTests) {
        GraphGenerator generator(N, D, allowNegativeWeights);
        generator.generate_graph();
        generator.SaveToFile(output_dir, 1);
    }

    return 0;
}