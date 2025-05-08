/*
Installation Instructions (only once):

1. Install g++ with OpenMP support:
   sudo apt update
   sudo apt install g++

2. Compile the code using:
   g++ -fopenmp P2.cpp -o P2

3. Run the program:
   ./P2
*/

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <omp.h>

using namespace std;

class Graph {
    int V;                      // Number of vertices
    vector<vector<int>> adj;     // Adjacency list

public:
    Graph(int V) {
        this->V = V;
        adj.resize(V);
    }

    // Function to add an edge between nodes u and v
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);  // Undirected graph
    }

    // Parallel BFS function
    void parallelBFS(int start) {
        vector<bool> visited(V, false);
        queue<int> q;
        visited[start] = true;
        q.push(start);

        cout << "Parallel BFS starting from node " << start << ": ";

        while (!q.empty()) {
            int size = q.size();
            
            #pragma omp parallel for
            for (int i = 0; i < size; i++) {
                int u;
                
                #pragma omp critical
                {
                    if (!q.empty()) {
                        u = q.front();
                        q.pop();
                        cout << u << " ";
                    }
                }

                // Visit neighbors of node u
                for (int v : adj[u]) {
                    if (!visited[v]) {
                        visited[v] = true;
                        q.push(v);
                    }
                }
            }
        }
        cout << endl;
    }

    // Parallel DFS function using stack
    void parallelDFS(int start) {
        vector<bool> visited(V, false);
        stack<int> s;

        s.push(start);
        cout << "Parallel DFS starting from node " << start << ": ";

        while (!s.empty()) {
            int u = s.top();
            s.pop();

            if (!visited[u]) {
                visited[u] = true;
                cout << u << " ";

                // Parallelize the exploration of neighbors
                #pragma omp parallel for
                for (int v : adj[u]) {
                    if (!visited[v]) {
                        s.push(v);
                    }
                }
            }
        }
        cout << endl;
    }
};

int main() {
    Graph g(6); // Create graph with 6 vertices
    
    // Add edges to the graph
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);

    // Parallel BFS starting from node 0
    g.parallelBFS(0);
    
    // Parallel DFS starting from node 0
    g.parallelDFS(0);

    return 0;
}
