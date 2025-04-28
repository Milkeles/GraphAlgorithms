/* [Description]
 * This program implements Dijkstra's shortest-path algorithm from a starting node to all other
 * nodes in the graph, using a custom indexed D-ary heap (MinIndexedDHeap) that supports true
 * decrease-key operations for efficiency. The heap's branching factor D can be tuned at
 * instantiation.
 * Additionally, the program measures the time and memory consumption of this implementation of the
 * algorithm for each test test graph and outputs them.
 * Important note: The method used to measure the memory consumption is OS-dependent and works specifically
 * on Linux, replicating the results on another operating system will require making changes in the program.
 *
 * Libraries:
 * - iostream, fstream: I/O for graph and /proc/self/status
 * - chrono: high-resolution timing
 * - vector, algorithm: data structures and utilities
 * - limits, stdexcept: constants and exceptions
 *
 * Author: H. Hristov
 * Ruse, 2025
 */

 #include <iostream>
 #include <fstream>
 #include <chrono>
 #include <vector>
 #include <string>
 #include <limits>
 #include <stdexcept>
 #include <algorithm>
 
 using namespace std;
 static constexpr long long INF = numeric_limits<long long>::max();
 
/* This function measures the memory usage of the current program and prints it out.
 * The function will probably not work on Windows! The /proc/self/status file seems to be Linux-specific.
 */
 void PrintMemoryUsage() {
     ifstream status("/proc/self/status");
     string line;
     while (getline(status, line)) {
         if (line.rfind("VmPeak", 0) == 0 || line.rfind("VmRSS", 0) == 0) {
             cout << line << '\n';
         }
     }
 }
 
 // Indexed D-ary min-heap supporting insert, decrease-key, and poll-min
 template<typename T>
 class MinIndexedDHeap {
 private:
     int D, size_, N;
     vector<int> pm, im;
     vector<T> values;
 
     inline int parent(int i) const { return (i - 1) / D; }
     inline int child(int i, int k) const { return i * D + k + 1; }
 
     void swim(int i) {
         while (i > 0) {
             int p = parent(i);
             if (values[im[i]] >= values[im[p]]) break;
             swap(im[i], im[p]);
             pm[im[i]] = i;
             pm[im[p]] = p;
             i = p;
         }
     }
 
     void sink(int i) {
         while (true) {
             int best = i;
             for (int k = 0; k < D; ++k) {
                 int c = child(i, k);
                 if (c >= size_) break;
                 if (values[im[c]] < values[im[best]]) best = c;
             }
             if (best == i) break;
             swap(im[i], im[best]);
             pm[im[i]] = i;
             pm[im[best]] = best;
             i = best;
         }
     }
 
     void checkKey(int ki) const {
         if (ki < 0 || ki >= N) throw invalid_argument("Key index out of bounds");
     }
 
 public:
     MinIndexedDHeap(int degree, int maxSize)
         : D(max(2, degree)), size_(0), N(max(D+1, maxSize)),
           pm(N, -1), im(N, -1), values(N)
     {}
 
     bool contains(int ki) const {
         checkKey(ki);
         return pm[ki] != -1;
     }
 
     void insert(int ki, const T &val) {
         if (contains(ki)) throw invalid_argument("Key already present");
         pm[ki] = size_;
         im[size_] = ki;
         values[ki] = val;
         swim(size_++);
     }
 
     void decrease(int ki, const T &newVal) {
         if (!contains(ki)) throw invalid_argument("Key not in heap");
         if (newVal < values[ki]) {
             values[ki] = newVal;
             swim(pm[ki]);
         }
     }
 
     int pollMinKey() {
         if (size_ == 0) throw out_of_range("Heap underflow");
         int minki = im[0];
         pm[minki] = -1;
         im[0] = im[--size_];
         pm[im[0]] = 0;
         sink(0);
         return minki;
     }
 
     T pollMinValue() {
         int ki = pollMinKey();
         return values[ki];
     }
 
     bool empty() const { return size_ == 0; }
 };
 
 int main() {
     ios::sync_with_stdio(false);
     cin.tie(nullptr);
 
     cout << "Memory usage at start:\n";
     PrintMemoryUsage();
     auto begin = chrono::steady_clock::now();
 
     string filePath = "graph_N10000_D0.100000_negfalse_1.in";
     ifstream fs(filePath);
     int N;
     fs >> N;
 
     vector<vector<pair<int,long long>>> adj(N+1);
     int u,v; long long w;
     while (fs >> u >> v >> w) {
         adj[u].emplace_back(v,w);
     }
 
     vector<long long> dist(N+1, INF);
     vector<int> prev(N+1, -1);
     dist[1] = 0;
 
     // degree estimate: avg edges per node
     int degree = max(2, (int)(adj[1].size()));
     MinIndexedDHeap<long long> heap(degree, N+1);
     heap.insert(1, 0LL);
 
     vector<char> visited(N+1, 0);
     while (!heap.empty()) {
         int x = heap.pollMinKey();
         if (visited[x]) continue;
         visited[x] = 1;
         for (auto &e : adj[x]) {
             int to = e.first;
             long long wt = e.second;
             if (visited[to]) continue;
             long long nd = dist[x] + wt;
             if (nd < dist[to]) {
                 dist[to] = nd;
                 prev[to] = x;
                 if (heap.contains(to)) heap.decrease(to, nd);
                 else heap.insert(to, nd);
             }
         }
     }
 
    //  for (int i = 1; i <= N; ++i) {
    //      if (dist[i] == INF) cout << "-1 ";
    //      else cout << dist[i] << ' ';
    //  }
    //  cout << '\n';
 
     auto end = chrono::steady_clock::now();
     cout << "\nMemory usage after algorithm:\n";
     PrintMemoryUsage();
     cout << "Elapsed time = " << chrono::duration_cast<chrono::nanoseconds>(end - begin).count() << " ns\n";
     return 0;
 }
 