#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <stdexcept>
#include <string>

struct Node {
    double x, y;
};

struct Edge {
    int u, v;
    double weight;

    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

class UnionFindBase {
protected:
    std::vector<int> parent;
    int findCount = 0;

public:
    UnionFindBase(int n) : parent(n) {
        for (int i = 0; i < n; ++i) {
            parent[i] = i;
        }
    }

    virtual int find(int x) = 0;
    virtual void unionSets(int x, int y) = 0;

    int getFindCount() const { return findCount; }
    virtual std::string getVariantName() const = 0;
};

// Variant 1: Naive union without path compression
class UnionFindNaive : public UnionFindBase {
public:
    UnionFindNaive(int n) : UnionFindBase(n) {}

    int find(int x) override {
        ++findCount;
        if (parent[x] != x) {
            return find(parent[x]);
        }
        return x;
    }

    void unionSets(int x, int y) override {
        int rootX = find(x);
        int rootY = find(y);
        if (rootX != rootY) {
            parent[rootX] = rootY;
        }
    }

    std::string getVariantName() const override {
        return "bez kompresji sciezki";
    }
};

// Variant 2: Naive union with path compression
class UnionFindNaivePC : public UnionFindBase {
public:
    UnionFindNaivePC(int n) : UnionFindBase(n) {}

    int find(int x) override {
        ++findCount;
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    void unionSets(int x, int y) override {
        int rootX = find(x);
        int rootY = find(y);
        if (rootX != rootY) {
            parent[rootX] = rootY;
        }
    }

    std::string getVariantName() const override {
        return "z kompresja sciezki";
    }
};

// Variant 3: Union by rank without path compression
class UnionFindRank : public UnionFindBase {
protected:
    std::vector<int> rank;

public:
    UnionFindRank(int n) : UnionFindBase(n), rank(n, 0) {}

    int find(int x) override {
        ++findCount;
        if (parent[x] != x) {
            return find(parent[x]);
        }
        return x;
    }

    void unionSets(int x, int y) override {
        int rootX = find(x);
        int rootY = find(y);
        if (rootX != rootY) {
            if (rank[rootX] < rank[rootY]) {
                parent[rootX] = rootY;
            }
            else if (rank[rootX] > rank[rootY]) {
                parent[rootY] = rootX;
            }
            else {
                parent[rootY] = rootX;
                rank[rootX]++;
            }
        }
    }

    std::string getVariantName() const override {
        return "Union by Rank bez kompresji sciezki";
    }
};

// Variant 4: Union by rank with path compression
class UnionFindRankPC : public UnionFindRank {
public:
    UnionFindRankPC(int n) : UnionFindRank(n) {}

    int find(int x) override {
        ++findCount;
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    void unionSets(int x, int y) override {
        int rootX = find(x);
        int rootY = find(y);
        if (rootX != rootY) {
            if (rank[rootX] < rank[rootY]) {
                parent[rootX] = rootY;
            }
            else if (rank[rootX] > rank[rootY]) {
                parent[rootY] = rootX;
            }
            else {
                parent[rootY] = rootX;
                rank[rootX]++;
            }
        }
    }

    std::string getVariantName() const override {
        return "Union by Rank z kompresja sciezki";
    }
};

class Graph {
    int n;
    std::vector<Node> nodes;
    std::vector<Edge> edges;

public:
    Graph(int n) : n(n) {}

    void addNode(double x, double y) {
        nodes.push_back({ x, y });
    }

    void addEdge(int u, int v, double weight) {
        edges.push_back({ u, v, weight });
    }

    template<typename UnionFindType>
    void runKruskalMST() {
        UnionFindType uf(n);
        std::vector<Edge> mst;

        auto sortStart = std::chrono::high_resolution_clock::now();
        std::sort(edges.begin(), edges.end());
        auto sortEnd = std::chrono::high_resolution_clock::now();
        auto sortTime = std::chrono::duration_cast<std::chrono::milliseconds>(sortEnd - sortStart).count();

        auto kruskalStart = std::chrono::high_resolution_clock::now();
        for (const Edge& edge : edges) {
            if (uf.find(edge.u) != uf.find(edge.v)) {
                mst.push_back(edge);
                uf.unionSets(edge.u, edge.v);
            }
        }
        auto kruskalEnd = std::chrono::high_resolution_clock::now();
        auto kruskalTime = std::chrono::duration_cast<std::chrono::milliseconds>(kruskalEnd - kruskalStart).count();

        double totalWeight = 0;
        for (const Edge& edge : mst) {
            totalWeight += edge.weight;
        }

        std::cout << "\nWariant: " << uf.getVariantName() << "\n";
        std::cout << "Liczba krawedzi w MST: " << mst.size() << "\n";
        std::cout << "Suma wag MST: " << totalWeight << "\n";
        std::cout << "Czas sortowania krawedzi: " << sortTime << " ms\n";
        std::cout << "Czas glownej petli Kruskala: " << kruskalTime << " ms\n";
        std::cout << "Liczba operacji find: " << uf.getFindCount() << "\n";
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) throw std::runtime_error("Cannot open file");

        int m;
        file >> n;
        nodes.resize(n);

        for (int i = 0; i < n; ++i) {
            file >> nodes[i].x >> nodes[i].y;
        }

        file >> m;
        for (int i = 0; i < m; ++i) {
            int u, v;
            double weight;
            file >> u >> v >> weight;
            edges.push_back({ u, v, weight });
        }
    }
};

int main() {
    std::vector<std::string> filenames = { "g1.txt", "g2.txt", "g3.txt" };

    for (const auto& filename : filenames) {
        try {
            std::cout << "\nPrzetwarzanie pliku: " << filename << "\n";
            std::cout << "----------------------------------------\n";

            Graph graph(0);
            graph.loadFromFile(filename);

            // Test all variants
            graph.runKruskalMST<UnionFindNaive>();
            graph.runKruskalMST<UnionFindNaivePC>();
            graph.runKruskalMST<UnionFindRank>();
            graph.runKruskalMST<UnionFindRankPC>();
        }
        catch (const std::exception& e) {
            std::cerr << "Blad: " << e.what() << "\n";
        }
    }

    return 0;
}