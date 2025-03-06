#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <chrono>
#include <random>

using namespace std;
using namespace chrono;

class Graph {
private:
    int V;
    vector<vector<int>> adj;
    set<vector<int>> foundCycles; // Para evitar ciclos duplicados

public:
    Graph(int V) {
        this->V = V;
        adj.resize(V);
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    bool isEdge(int u, int v) {
        return find(adj[u].begin(), adj[u].end(), v) != adj[u].end();
    }

    // Método 1: Enumerar ciclos por permutação de vértices
    void findCyclesByPermutation() {
        vector<int> vertices(V);
        for (int i = 0; i < V; i++) {
            vertices[i] = i;
        }

        foundCycles.clear(); // Resetar para cada execução

        do {
            for (int i = 0; i < V; i++) {
                vector<int> cycle;
                cycle.push_back(vertices[i]);

                for (int j = i + 1; j < V; j++) {
                    if (isEdge(cycle.back(), vertices[j])) {
                        cycle.push_back(vertices[j]);
                    } else {
                        break;
                    }

                    if (cycle.size() > 2 && isEdge(cycle.back(), cycle[0])) {
                        vector<int> sortedCycle = cycle;
                        sort(sortedCycle.begin(), sortedCycle.end());
                        if (foundCycles.find(sortedCycle) == foundCycles.end()) {
                            foundCycles.insert(sortedCycle);
                        }
                    }
                }
            }
        } while (next_permutation(vertices.begin(), vertices.end()));
    }

    void dfs(int start, int current, vector<int>& path, vector<bool>& visited) {
        visited[current] = true;
        path.push_back(current);

        for (int neighbor : adj[current]) {
            if (neighbor == start && path.size() > 2) {
                vector<int> cycle = path;
                sort(cycle.begin(), cycle.end());

                if (foundCycles.find(cycle) == foundCycles.end()) {
                    foundCycles.insert(cycle);
                }
            } else if (!visited[neighbor]) {
                dfs(start, neighbor, path, visited);
            }
        }

        visited[current] = false;
        path.pop_back();
    }

    // Método 2: Busca por ciclos usando DFS
    void findCyclesByDFS() {
        foundCycles.clear();
        for (int i = 0; i < V; i++) {
            vector<bool> visited(V, false);
            vector<int> path;
            dfs(i, i, path, visited);
        }
    }

    // Função para medir o tempo de execução
    double measureTime(void (Graph::*method)()) {
        auto start = high_resolution_clock::now();
        (this->*method)();
        auto stop = high_resolution_clock::now();
        return duration<double>(stop - start).count();
    }
};

// Função para gerar um grafo aleatório com E arestas
Graph generateRandomGraph(int V, int E) {
    Graph g(V);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0, V - 1);

    set<pair<int, int>> edges;

    while (edges.size() < E) {
        int u = dist(gen);
        int v = dist(gen);
        if (u != v && edges.find({u, v}) == edges.end() && edges.find({v, u}) == edges.end()) {
            g.addEdge(u, v);
            edges.insert({u, v});
        }
    }

    return g;
}

int main() {
    vector<int> sizes = {5, 6, 7, 8, 10, 12 }; // Diferentes tamanhos de grafo
    int E = 10; // Número fixo de arestas (para manter testes justos)

    cout << "Comparação de desempenho:\n";
    cout << "Vértices | Permutação (s) | DFS (s)\n";
    cout << "-----------------------------------\n";

    for (int V : sizes) {
        Graph g = generateRandomGraph(V, E);

        double timePermutation = g.measureTime(&Graph::findCyclesByPermutation);
        double timeDFS = g.measureTime(&Graph::findCyclesByDFS);

        cout << V << "        | " << timePermutation << "       | " << timeDFS << "\n";
    }

    return 0;
}
