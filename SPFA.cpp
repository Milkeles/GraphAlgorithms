/* [Description]
 * This program computes shortest paths from node 1 to all other nodes using the SPFA (Shortest Path Faster) algorithm,
 * which handles negative edge weights and typically runs faster than Bellman–Ford on sparse graphs.
 * Additionally, the program measures the time and memory consumption of this implementation
 * for each test graph and outputs these metrics.
 * Important note: Memory measurement is OS-dependent and works on Linux via /proc/self/status.
 *
 * Libraries:
 * - iostream: For printing messages and errors to stdout.
 * - fstream: For reading input graph files and the status file for memory usage.
 * - chrono: For measuring elapsed execution time.
 * - vector: For storing adjacency lists and distance arrays.
 * - tuple: For representing edges while reading input (from, to, weight).
 * - queue: For the SPFA processing queue.
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
#include <tuple>
#include <queue>
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

    string filePath = "graph_N10000_D0.100000_negtrue_1.in";
    ifstream fileStream(filePath);

    int N;
    fileStream >> N;

    vector<vector<pair<int,ll>>> adj(N+1);
    int u, v;
    ll w;
    while (fileStream >> u >> v >> w) {
        adj[u].emplace_back(v, w);
    }

    // SPFA algorithm from source 1
    vector<ll> dist(N+1, INF);
    vector<bool> inQueue(N+1, false);
    queue<int> q;
    dist[1] = 0;
    q.push(1);
    inQueue[1] = true;
    bool negCycle = false;
    vector<int> cnt(N+1, 0);

    while (!q.empty()) {
        int x = q.front(); q.pop();
        inQueue[x] = false;
        for (auto &pr : adj[x]) {
            int y = pr.first;
            ll w2 = pr.second;
            if (dist[x] + w2 < dist[y]) {
                dist[y] = dist[x] + w2;
                if (!inQueue[y]) {
                    q.push(y);
                    inQueue[y] = true;
                    if (++cnt[y] > N) {
                        negCycle = true;
                        break;
                    }
                }
            }
        }
        if (negCycle) break;
    }

    if (negCycle) {
        cout << "Warning: negative weight cycle detected.\n";
        return 1;
    }

    // for (int i = 1; i <= N; ++i) {
    //     if (dist[i] >= INF/2) cout << "INF";
    //     else cout << dist[i];
    //     if (i < N) cout << ' ';
    // }
    // cout << '\n';

    auto end = chrono::steady_clock::now();

    cout << "\nMemory usage after algorithm:\n";
    PrintMemoryUsage();
    cout << "Elapsed time = " << chrono::duration_cast<chrono::nanoseconds>(end - begin).count() << " ns\n";
    return 0;
}
