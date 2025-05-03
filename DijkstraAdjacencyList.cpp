/* [Description]
 * This program contains an implementation of Dijkstra's shortest path algorithm from a starting node to
 * all other nodes in the graph. However, the algorithm can easily be modified to stop early once it finds
 * the shortest path to a specific ending node for more efficiency if that's all we need.
 * Additionally, the program measures the time and memory consumption of this implementation of the
 * algorithm for each test test graph and outputs them.
 * Important note: The method used to measure the memory consumption is OS-dependent and works specifically
 * on Linux, replicating the results on another operating system will require making changes in the program.
 * 
 * Libraries:
 * - iostream: To print out messages and errors in the stdout.
 * - fstream: Reading from the status file and test graph files.
 * - chrono: Measure elapsed time.
 * - vector, queue: Necessary data structures to implement the algorithm.
 * 
 * Author: H. Hristov
 * Ruse, 2025
 */
#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <queue>


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
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    cout << "Memory usage at start:\n";
    PrintMemoryUsage();
    
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    
    string filePath = "graph_N10000_D0.100000_negfalse_1.in";
    ifstream fileStream(filePath);
    
    int N;
    fileStream >> N;
    
    vector<vector<pair<int, long long>>> adjacencyList(N + 1);
    int fromNode, toNode;
    long long edgeWeight;
    while (fileStream >> fromNode >> toNode >> edgeWeight) {
        adjacencyList[fromNode].push_back({toNode, edgeWeight});
    }
    
    vector<long long> distances(N + 1, INF);
    distances[1] = 0;
    
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
    pq.push({0, 1});
    
    vector<bool> visited(N + 1, false);
    
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        
        if (visited[u]) continue;
        visited[u] = true;
        
        for (auto [v, w] : adjacencyList[u]) {
            if (distances[v] > distances[u] + w) {
                distances[v] = distances[u] + w;
                pq.push({distances[v], v});
            }
        }
    }
    
    // for (int i = 1; i <= N; i++) {
    //     if (distances[i] == INF) cout << "-1 ";
    //     else cout << distances[i] << " ";
    // }
    // cout << endl;

    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    cout << "\nMemory usage after algorithm:\n";
    PrintMemoryUsage();
    cout << "Elapsed time = " << chrono::duration_cast<chrono::nanoseconds> (end - begin).count() << " ns" << '\n';

    return 0;
}