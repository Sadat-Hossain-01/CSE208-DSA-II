#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

#include "BinaryHeap.h"
#include "FibHeap.h"
#include "header.h"
using namespace std;

struct Edge {
    int u, v;
    long long w;
};

const long long INF = 2e16;
int n_vertices, n_edges, k;
vector<vector<Edge>> adj;
vector<long long> dist_bn, dist_fb;
vector<int> len_bn, len_fb;
vector<bool> visited;

void dijkstra_bn(int s) {
    // initialize-single-source-distance
    fill(dist_bn.begin(), dist_bn.end(), INF);
    fill(len_bn.begin(), len_bn.end(), 2e8);
    dist_bn[s] = 0;
    len_bn[s] = 0;
    vector<Pair> v(n_vertices);
    // 0-based
    for (int i = 0; i < n_vertices; i++) {
        if (i == s)
            v[i] = {i, 0};
        else
            v[i] = {i, INF};
    }
    BinHeap<Pair> pq(v);
    while (!pq.isEmpty()) {
        // extract-min
        Pair cur = pq.getMin();
        // debug(cur);
        pq.deleteMin();
        int u = cur.u;
        for (const Edge& e : adj[u]) {
            int v = e.v;
            // relaxation
            if (dist_bn[v] > dist_bn[u] + e.w) {
                pq.decreaseKey({v, dist_bn[v]}, dist_bn[u] + e.w);
                dist_bn[v] = dist_bn[u] + e.w;
                len_bn[v] = len_bn[u] + 1;
            }
        }
    }
    // cerr << "Binary done\n";
}

void dijkstra_fb(int s) {
    // initialize-single-source-distance
    fill(dist_fb.begin(), dist_fb.end(), INF);
    fill(len_fb.begin(), len_fb.end(), 2e8);
    dist_fb[s] = 0;
    len_fb[s] = 0;
    FibHeap<Pair> fq(n_vertices);
    // 0-based
    for (int i = 0; i < n_vertices; i++) {
        Pair p;
        if (i == s)
            p = {i, 0};
        else
            p = {i, INF};
        fq.insert(p);
    }
    while (!fq.isEmpty()) {
        // extract-min
        Pair cur = fq.extractMin();
        // debug(cur);
        int u = cur.u;
        for (const Edge& e : adj[u]) {
            int v = e.v;
            // relaxation
            if (dist_fb[v] > dist_fb[u] + e.w) {
                fq.decreaseKey({v, dist_fb[v]}, dist_fb[u] + e.w);
                dist_fb[v] = dist_fb[u] + e.w;
                len_fb[v] = len_fb[u] + 1;
            }
        }
    }
    // cerr << "Fibonacci done\n";
}

int main() {
    // undirected
    ifstream in1;
    in1.open("graph.txt");

    in1 >> n_vertices >> n_edges;
    adj.resize(n_vertices + 2);
    dist_bn.resize(n_vertices + 2);
    dist_fb.resize(n_vertices + 2);
    len_bn.resize(n_vertices + 2);
    len_fb.resize(n_vertices + 2);
    visited.resize(n_vertices + 2);

    for (int i = 1; i <= n_edges; i++) {
        // assuming 0-based
        // undirected graph as well
        int u, v;
        long long w;
        in1 >> u >> v >> w;
        Edge e = {u, v, w};
        adj[u].push_back(e);
        e = {v, u, w};
        adj[v].push_back(e);
    }
    in1.close();

    ifstream in2;
    in2.open("queries.txt");
    ofstream out;
    out.open("output.txt");

    in2 >> k;
    for (int q = 1; q <= k; q++) {
        int s, t;
        in2 >> s >> t;
        auto startTime = chrono::high_resolution_clock::now();
        dijkstra_bn(s);
        auto endTime = chrono::high_resolution_clock::now();
        double binary_time =
            chrono::duration_cast<chrono::nanoseconds>(endTime - startTime)
                .count() /
            (1000000.0);
        // cout << "Binary: " << dist_bn[t] << " " << len_bn[t] << endl;

        startTime = chrono::high_resolution_clock::now();
        dijkstra_fb(s);
        endTime = chrono::high_resolution_clock::now();
        double fibonacci_time =
            chrono::duration_cast<chrono::nanoseconds>(endTime - startTime)
                .count() /
            (1000000.0);
        // debug(len_fb[t], len_bn[t], dist_fb[t], dist_bn[t]);
        // cout << "Fibonacci: " << dist_fb[t] << " " << len_fb[t] << endl;

        out << len_bn[t] << " " << dist_bn[t] << " " << binary_time << "ms "
            << fibonacci_time << "ms\n";
    }

    in2.close();
    out.close();
    return 0;
}