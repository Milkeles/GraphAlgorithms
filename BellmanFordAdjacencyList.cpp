/* [Description]
 * This program implements the Bellman-Ford shortest path algorithm from a starting node
 * to all other nodes in the graph. It supports graphs with negative edge weights and can
 * detect negative weight cycles.
 * Additionally, the program measures the time and memory consumption of this implementation
 * for each test graph and outputs these metrics.
 * Important note: Memory measurement is OS-dependent and works on Linux via /proc/self/status.
 *
 * Libraries:
 * - iostream: For printing messages and errors to stdout.
 * - fstream: For reading input graph files and the status file for memory usage.
 * - chrono: For measuring elapsed execution time.
 * - vector: For storing the edge list and distance array.
 * - tuple: For representing edges as (from, to, weight) tuples.
 * - string: For file path handling and string operations.
 *
 * Author: H. Hristov
 * Ruse, 2025
 */
#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <tuple>
#include <string>

using namespace std;

const long long INF = 1e18;

/* This function measures the memory usage of the current program and prints it out.
 * The function will probably not work on Windows! The /proc/self/status file seems to be Linux-specific.
 */
void PrintMemoryUsage() {
    ifstream status("/proc/self/status");
    string line;
    while (getline(status, line)) {
        if (line.find("VmPeak") != string::npos || line.find("VmRSS") != string::npos) {
            cout << line << '\n';
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cout << "Memory usage at start:\n";
    PrintMemoryUsage();

    chrono::steady_clock::time_point begin = chrono::steady_clock::now();

    string filePath = "graph_N10000_D0.100000_negtrue_1.in";
    ifstream fileStream(filePath);

    int N;
    fileStream >> N;

    vector<tuple<int,int,long long>> edges;
    int u, v;
    long long w;
    while (fileStream >> u >> v >> w) {
        edges.emplace_back(u, v, w);
    }

    vector<long long> distances(N + 1, INF);
    distances[1] = 0;

    // Bellman-Ford algorithm
    for (int i = 1; i <= N - 1; ++i) {
        bool updated = false;
        for (auto &edge : edges) {
            int from, to;
            long long weight;
            tie(from, to, weight) = edge;
            if (distances[from] != INF && distances[to] > distances[from] + weight) {
                distances[to] = distances[from] + weight;
                updated = true;
            }
        }
        if (!updated) break;
    }

    bool negCycle = false;
    for (auto &edge : edges) {
        int from, to;
        long long weight;
        tie(from, to, weight) = edge;
        if (distances[from] != INF && distances[to] > distances[from] + weight) {
            negCycle = true;
            break;
        }
    }

    // for (int i = 1; i <= N; ++i) {
    //     if (distances[i] == INF) cout << "INF ";
    //     else cout << distances[i] << " ";
    // }
    // cout << '\n';

    if (negCycle) {
        cout << "Warning: negative weight cycle detected." << '\n';
    }

    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    cout << "\nMemory usage after algorithm:\n";
    PrintMemoryUsage();
    cout << "Elapsed time = " << chrono::duration_cast<chrono::nanoseconds>(end - begin).count() << " ns\n";

    return 0;
}
