#include <fstream>
#include <iostream>
#include <queue>
#include <stack>
#include <vector>
using namespace std;

typedef long long ll;
struct Edge {
    int to;
    ll weight;
    bool operator>(const Edge& other) const {
        if (weight != other.weight) return weight > other.weight;
        return to > other.to;
    }
};

const ll INF = 2e15;
int n, m, s, d;
vector<vector<Edge>> adj;
vector<ll> dist;
vector<int> parent;

bool bellman_ford(int source) {
    dist[source] = 0;
    parent[source] = -1;
    for (int i = 1; i <= n; i++) {
        bool changed = false;
        for (int from = 0; from < n; from++) {
            for (auto& edge : adj[from]) {
                int to = edge.to;
                ll weight = edge.weight;
                if (dist[from] < INF) {
                    if (dist[to] > dist[from] + weight) {
                        if (i == n) return false;
                        dist[to] = dist[from] + weight;
                        parent[to] = from;
                        changed = true;
                    }
                }
            }
        }
        if (!changed) break;
    }
    return true;
}

int main() {
    ifstream in;
    in.open("bellman_in.txt");
    in >> n >> m;
    adj.resize(n + 1);
    dist.assign(n + 1, INF);
    parent.resize(n + 1);
    for (int i = 0; i < m; i++) {
        int a, b, c;
        in >> a >> b >> c;
        adj[a].push_back({b, c});
    }
    in >> s >> d;
    bool ret = bellman_ford(s);
    if (!ret) {
        cout << "The graph contains a negative cycle\n";
        return 0;
    }
    if (d >= n || dist[d] == INF) {
        cout << "Destination unreachable from source\n";
    } else {
        cout << "The graph does not contain a negative cycle\n";
        cout << "Shortest path cost: " << dist[d] << "\n";
        stack<int> st;
        while (d != -1) {
            st.push(d);
            d = parent[d];
        }
        while (!st.empty()) {
            cout << st.top() << " ";
            st.pop();
            if (!st.empty()) cout << "-> ";
        }
        cout << "\n";
    }
    in.close();
}