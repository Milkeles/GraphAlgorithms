/* [Description]
 * Dijkstra’s single‐source shortest‐path on a directed graph (1…N), using
 * radix_heap::pair_radix_heap (one-level radix heap) for O(m + n log C)
 * amortized time.  We avoid double‐pull on extract, use emplace, and reserve
 * adjacency lists for a small constant-factor speedup.
 *
 * Memory usage (VmPeak/VmRSS) before and after via /proc/self/status (Linux),
 * and elapsed time in nanoseconds via chrono.
 *
 * Author: H. Hristov
 * Ruse, 2025
 */

 #include <iostream>
 #include <fstream>
 #include <chrono>
 #include <vector>
 #include <limits>
 #include "radix_heap.h"
 
 using namespace std;
 static constexpr long long INF = numeric_limits<long long>::max();
 
/* This function measures the memory usage of the current program and prints it out.
 * The function will probably not work on Windows! The /proc/self/status file seems to be Linux-specific.
 */
 void PrintMemoryUsage(){
     ifstream status("/proc/self/status");
     string line;
     while(getline(status,line)){
         if(line.find("VmPeak") != string::npos ||
            line.find("VmRSS")  != string::npos){
             cout<< line << "\n";
         }
     }
 }
 
 int main(){
     ios::sync_with_stdio(0);
     cin.tie(0);
     cout.tie(0);
 
     cout<<"Memory usage at start:\n";
     PrintMemoryUsage();
     auto begin = chrono::steady_clock::now();
 
     const string filePath = "graph_N10000_D0.100000_negfalse_1.in";
     ifstream in(filePath);
     int N; in>>N;
     vector<vector<pair<int,long long>>> adj(N+1);
     adj.reserve(N+1);

     int u,v; long long w;
     while(in>>u>>v>>w){
         adj[u].emplace_back(v,w);
     }
 
     vector<long long> dist(N+1, INF);
     vector<char>     seen(N+1, 0);
     dist[1] = 0;
 
     radix_heap::pair_radix_heap<long long,int> pq;
     pq.emplace(0LL, 1);
 
     while(!pq.empty()){
         // only one refill by calling top_value() first
         int      x = pq.top_value();
         long long d = pq.top_key();
         pq.pop();
         if(seen[x]) continue;
         seen[x] = 1;
 
         for(auto &e: adj[x]){
             int to = e.first;
             if(seen[to]) continue;
             long long nd = d + e.second;
             if(nd < dist[to]){
                 dist[to] = nd;
                 pq.emplace(nd, to);
             }
         }
     }
 
    //  for(int i=1;i<=N;++i){
    //      cout << (dist[i]==INF? -1 : dist[i]) << " ";
    //  }
     cout<<"\n\n";
 
     auto end = chrono::steady_clock::now();
     cout<<"Memory usage after algorithm:\n";
     PrintMemoryUsage();
     cout<<"Elapsed time = " << chrono::duration_cast<chrono::nanoseconds>(end-begin).count() <<" ns\n";
 
     return 0;
 }
 