/* [Description]
 * This program implements the A* shortest-path algorithm on a directed graph represented
 * by an adjacency list, with nodes labeled 1…N in the input. We use f(u) = g(u) + h(u),
 * where g(u) is the exact cost from the start (node 1) and h(u)=0, so this behaves
 * exactly like Dijkstra while preserving the A* structure for future heuristic swaps.
 * Additionally, the program measures the time and memory consumption of this implementation of the
 * algorithm for each test test graph and outputs them.
 * Important note: The method used to measure the memory consumption is OS-dependent and works specifically
 * on Linux, replicating the results on another operating system will require making changes in the program.
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

// Trivial zero heuristic, must be swapped with something that actually works.
inline long long Heuristic(int)
{
    return 0;
}

/* This function measures the memory usage of the current program and prints it out.
 * The function will probably not work on Windows! The /proc/self/status file seems to be Linux-specific.
 */
void PrintMemoryUsage()
{
    ifstream status("/proc/self/status");
    string line;
    while (getline(status, line))
    {
        if (line.find("VmPeak") != string::npos ||
            line.find("VmRSS") != string::npos)
        {
            cout << line << '\n';
        }
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cout << "Memory usage at start:\n";
    PrintMemoryUsage();

    auto begin = chrono::steady_clock::now();

    const string filePath = "graph_N10000_D0.100000_negfalse_1.in";
    ifstream fileStream(filePath);
    int N;
    fileStream >> N;

    vector<vector<pair<int, long long>>> adjacencyList(N + 1);
    int u, v;
    long long w;
    while (fileStream >> u >> v >> w)
    {
        adjacencyList[u].emplace_back(v, w);
    }

    vector<long long> dist(N + 1, INF);
    vector<bool> seen(N + 1, false);
    dist[1] = 0;

    // Min-heap ordered by f = g + h
    priority_queue <
        pair<long long, int>,
        vector<pair<long long, int>>,
        greater<pair<long long, int>>>
        pq;
    pq.push({dist[1] + Heuristic(1), 1});

    while (!pq.empty())
    {
        auto [f, x] = pq.top();
        pq.pop();
        if (seen[x])
            continue;
        seen[x] = true;

        for (auto [y, wt] : adjacencyList[x])
        {
            long long g = dist[x] + wt;
            if (g < dist[y])
            {
                dist[y] = g;
                pq.push({g + Heuristic(y), y});
            }
        }
    }

    //  for (int i = 1; i <= N; ++i) {
    //      cout << (dist[i] == INF ? -1 : dist[i]) << " ";
    //  }
    // cout << '\n';

    auto end = chrono::steady_clock::now();
    cout << "\nMemory usage after algorithm:\n";
    PrintMemoryUsage();
    cout << "Elapsed time = " << chrono::duration_cast<chrono::nanoseconds>(end - begin).count() << " ns\n";

    return 0;
}
