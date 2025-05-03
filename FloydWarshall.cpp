/* [Description]
 * This program computes shortest paths for all pairs in a graph using the FloydWarshall algorithm.
 * Additionally, the program measures the time and memory consumption of this implementation
 * for each test graph and outputs these metrics.
 * Important note: Memory measurement is OS-dependent and works on Linux via /proc/self/status.
 *
 * Libraries:
 * - iostream: For printing messages and errors to stdout.
 * - fstream: For reading the input graph file and the status file for memory usage.
 * - chrono: For measuring elapsed execution time.
 * - vector: For storing the distance matrix.
 * - limits: For INF definition.
 * - string: For file path handling.
 *
 * Author: H. Hristov
 * Ruse, 2025
 */

 #include <iostream>
 #include <fstream>
 #include <chrono>
 #include <vector>
 #include <limits>
 #include <string>
 
 using namespace std;
 using ll = long long;
 const ll INF = numeric_limits<ll>::max() / 4;
 
/* This function measures the memory usage of the current program and prints it out.
 * The function will probably not work on Windows! The /proc/self/status file seems to be Linux-specific.
 */
 void PrintMemoryUsage() {
     ifstream status("/proc/self/status");
     string line;
     while (getline(status, line)) {
         if (line.find("VmPeak") != string::npos || line.find("VmRSS") != string::npos) {
             cout << line << "\n";
         }
     }
 }
 
 int main() {
     ios::sync_with_stdio(false);
     cin.tie(nullptr);
 
     cout << "Memory usage at start:\n";
     PrintMemoryUsage();
 
     auto begin = chrono::steady_clock::now();

     // Warning: The code works but N10 000 is VERY slow. Try the other tests unless you're prepared to wait a while.
     string filePath = "graph_N10000_D0.001000_negfalse_1.in";
     ifstream fileStream(filePath);
 
     int N;
     fileStream >> N;
 
     // Initialize distance matrix
     vector<vector<ll>> dist(N+1, vector<ll>(N+1, INF));
     for (int i = 1; i <= N; ++i) dist[i][i] = 0;
 
     // Read edges
     int u, v;
     ll w;
     while (fileStream >> u >> v >> w) {
         dist[u][v] = w;
     }
 
     // Floyd–Warshall algorithm
     for (int k = 1; k <= N; ++k) {
         for (int i = 1; i <= N; ++i) {
             if (dist[i][k] == INF) continue;
             for (int j = 1; j <= N; ++j) {
                 if (dist[k][j] != INF && dist[i][k] + dist[k][j] < dist[i][j]) {
                     dist[i][j] = dist[i][k] + dist[k][j];
                 }
             }
         }
     }
 
    //  for (int j = 1; j <= N; ++j) {
    //      if (dist[1][j] == INF) cout << "INF";
    //      else cout << dist[1][j];
    //      if (j < N) cout << ' ';
    //  }
    //  cout << '\n';
 
     auto end = chrono::steady_clock::now();
     cout << "\nMemory usage after algorithm:\n";
     PrintMemoryUsage();
     cout << "Elapsed time = " << chrono::duration_cast<chrono::nanoseconds>(end - begin).count() << " ns\n";
 
     return 0;
 }