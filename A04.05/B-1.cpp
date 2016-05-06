#include <algorithm>
#include <climits>
#include <cstdint>
#include <iostream>
#include <vector>


const int16_t NONE = -1;
const int16_t INF = SHRT_MIN;

struct Edge {
    int16_t from, to;
    int16_t weight;

    Edge(int16_t from, int16_t to, int16_t weight): from(from), to(to), weight(weight) {}
};

std::vector<Edge> readEdges(std::istream& in, size_t edgeN) {
    std::vector<Edge> edges;
    for (int16_t from, to, weight; edgeN; --edgeN) {
        std::cin >> from >> to >> weight;
        edges.emplace_back(from - 1, to - 1, weight);
    }
    return edges;
}

int main() {
    size_t nodeN, edgeN;
    std::cin >> nodeN >> edgeN;

    const std::vector<Edge> edges = readEdges(std::cin, edgeN);

    // Ford–Fulkerson algorithm
    std::vector<int> result(nodeN, INF);
    std::vector<int16_t> previous(nodeN, NONE);
    for (const auto& edge : edges) {
        if (edge.from == 0 && result[edge.to] < edge.weight) {
            result[edge.to] = edge.weight;
            previous[edge.to] = edge.from;
        }
    }
    for (int16_t i = 1; i <= nodeN; ++i) {
        std::vector<int> current = result;
        for (const auto& edge : edges) {
            if (result[edge.from] == INF) {
                continue;
            }
            const int weight = result[edge.from] + edge.weight;
            if (current[edge.to] < weight) {
                current[edge.to] = weight;
                previous[edge.to] = edge.from;
            }
        }
        result = std::move(current);
    }

    std::vector<int16_t> path;
    for (int16_t node = nodeN - 1; node != NONE; node = previous[node]) {
        path.push_back(node);
        if (path.size() > nodeN) {
            std::cout << "Inf" << std::endl;
            return 0;
        }
    }

    if (result.back() == INF) {
        std::cout << "Fail" << std::endl;
    } else {
       std::cout << result.back() << std::endl;
    }

    return 0;
}
