/* [Description]
 * This program computes all-pairs shortest paths using Johnson's reweighting method:
 * it first runs Bellman–Ford to obtain vertex potentials and detect negative cycles,
 * then runs Dijkstra from each node on the reweighted graph.
 * Additionally, the program measures the time and memory consumption of this implementation
 * for each test graph and outputs these metrics.
 * Important note: Memory measurement is OS-dependent and works on Linux via /proc/self/status.
 *
 * Libraries:
 * - iostream: For printing messages and errors to stdout.
 * - fstream: For reading input graph files and the status file for memory usage.
 * - chrono: For measuring elapsed execution time.
 * - vector: For storing edges, adjacency lists, and distance matrices.
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
    // 
    string filePath = "graph_N10000_D0.001000_negfalse_1.in";
    ifstream fileStream(filePath);

    int N;
    fileStream >> N;

    // Read edges
    vector<tuple<int,int,ll>> edges;
    int u, v;
    ll w;
    while (fileStream >> u >> v >> w) {
        edges.emplace_back(u, v, w);
    }

    // Bellman–Ford to compute vertex potentials h
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

    // Dijkstra on the reweighted graph.
    vector<vector<ll>> all_dist(N+1, vector<ll>(N+1, INF));

    vector<ll> d(N+1);
    for (int s = 1; s <= N; ++s) {
        fill(d.begin(), d.end(), INF);
        d[s] = 0;
        priority_queue<pli, vector<pli>, greater<pli>> pq;
        pq.emplace(0, s);
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
        for (int t = 1; t <= N; ++t) {
            if (d[t] < INF)
                all_dist[s][t] = d[t] - h[s] + h[t];
        }
    }

    // for (int j = 1; j <= N; ++j) {
    //     if (all_dist[1][j] == INF)
    //         cout << "INF";
    //     else
    //         cout << all_dist[1][j];
    //     if (j < N) cout << ' ';
    // }
    // cout << '\n';

    auto end = chrono::steady_clock::now();
    cout << "\nMemory usage after algorithm:\n";
    PrintMemoryUsage();
    cout << "Elapsed time = " << chrono::duration_cast<chrono::nanoseconds>(end - begin).count() << " ns\n";

    return 0;
}
