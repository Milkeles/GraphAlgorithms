/* [Description]
 * This program computes shortest paths from node 1 to all other nodes using Johnson's reweighting:
 * it first runs Bellman–Ford to obtain vertex potentials and detect negative cycles, then runs
 * Dijkstra from the first node on the reweighted graph. It measures time and memory usage.
 * Important note: Memory measurement is OS-dependent and works on Linux via /proc/self/status.
 *
 * Libraries:
 * - iostream: For printing messages and errors to stdout.
 * - fstream: For reading input graph files and the status file for memory usage.
 * - chrono: For measuring elapsed execution time.
 * - vector: For storing edges and distance arrays.
 * - tuple: For representing edges as (from, to, weight) tuples.
 * - queue: For priority_queue in Dijkstra.
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
typedef pair<ll,int> pli;
const ll INF = numeric_limits<ll>::max() / 4;

/* Prints peak and current memory usage of the process (Linux-specific). */
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

    string filePath = "graph_N100_D0.100000_negtrue_1.in";
    ifstream fileStream(filePath);

    int N;
    fileStream >> N;

    vector<tuple<int,int,ll>> edges;
    int u, v;
    ll w;
    while (fileStream >> u >> v >> w) {
        edges.emplace_back(u, v, w);
    }

    // Bellman-Ford to compute potentials h
    vector<ll> h(N+1, 0);
    for (int i = 1; i < N; ++i) {
        bool updated = false;
        for (auto &e : edges) {
            tie(u, v, w) = e;
            if (h[u] + w < h[v]) {
                h[v] = h[u] + w;
                updated = true;
            }
        }
        if (!updated) break;
    }
    // Check for negative cycles
    for (auto &e : edges) {
        tie(u, v, w) = e;
        if (h[u] + w < h[v]) {
            cout << "Warning: negative weight cycle detected.\n";
            return 1;
        }
    }

    // Build reweighted adjacency list
    vector<vector<pair<int,ll>>> adj(N+1);
    for (auto &e : edges) {
        tie(u, v, w) = e;
        ll w2 = w + h[u] - h[v];
        adj[u].emplace_back(v, w2);
    }

    // Dijkstra from source 1 only
    vector<ll> d(N+1, INF);
    d[1] = 0;
    priority_queue<pli, vector<pli>, greater<pli>> pq;
    pq.emplace(0, 1);
    while (!pq.empty()) {
        auto [du, x] = pq.top(); pq.pop();
        if (du != d[x]) continue;
        for (auto &pr : adj[x]) {
            int y = pr.first;
            ll w2 = pr.second;
            ll nd = du + w2;
            if (nd < d[y]) {
                d[y] = nd;
                pq.emplace(nd, y);
            }
        }
    }

    // Output distances from node 1
    for (int i = 1; i <= N; ++i) {
        if (d[i] >= INF / 2) cout << "INF";
        else cout << (d[i] - h[1] + h[i]);
        if (i < N) cout << ' ';
    }
    cout << '\n';

    auto end = chrono::steady_clock::now();
    cout << "\nMemory usage after algorithm:\n";
    PrintMemoryUsage();
    cout << "Elapsed time = " << chrono::duration_cast<chrono::nanoseconds>(end - begin).count() << " ns\n";

    return 0;
}
