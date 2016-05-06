#include <iostream>
#include <climits>
#include <cstdint>
#include <vector>


typedef int16_t NodeID;
typedef int16_t EdgeID;

template<typename T>
struct Matrix {
    typedef std::vector<std::vector<T>> Type;
    static Type init(size_t size, const T& value){
        return Type(size, std::vector<T>(size, value));
    }
};

typedef std::vector<NodeID> Path;

const NodeID NONE = -1;
const int INF = INT_MIN;

void restorePath(const Matrix<NodeID>::Type& via, NodeID from, NodeID to, Path& path) {
    if (via[from][to] == NONE) {
        path.push_back(to);
        return;
    }
    restorePath(via, from, via[from][to], path);
    restorePath(via, via[from][to], to, path);
}

int main() {
    size_t townN, roadN, checkPointN;
    std::cin >> townN >> roadN >> checkPointN;

    auto weights = Matrix<int>::init(townN, INF);
    auto edges = Matrix<EdgeID>::init(townN, NONE);
    auto viaNodes = Matrix<NodeID>::init(townN, NONE);
    {   // read town graph
        NodeID from, to;
        int weight;
        for (EdgeID edgeId = 1; edgeId <= roadN; ++edgeId) {
            std::cin >> from >> to >> weight;
            --from, --to;
            if (weight > weights[from][to]) {
                weights[from][to] = weight;
                edges[from][to] = edgeId;
            }
        }
    }

    std::vector<NodeID> checkPoints;
    for (NodeID checkPoint; checkPointN; --checkPointN) {
        std::cin >> checkPoint;
        checkPoints.push_back(checkPoint - 1);
    }

    // Floyd–Warshall algorithm
    for (NodeID via = 0; via < townN; ++via) {
        for (NodeID from = 0; from < townN; ++from) {
            for (NodeID to = 0; to < townN; ++to) {
                if (weights[from][via] != INF && weights[via][to] != INF) {
                    const int weight = weights[from][via] + weights[via][to];
                    if (weights[from][to] < weight) {
                        weights[from][to] = weight;
                        viaNodes[from][to] = via;
                    }
                }
            }
        }
    }

    for (NodeID via = 0; via < townN; ++via) {
        if (weights[via][via] > 0) {
            for (size_t i = 0; i + 1 < checkPoints.size(); ++i) {
                const auto from = checkPoints[i];
                const auto to = checkPoints[i + 1];
                if (weights[from][via] != INF && weights[via][to] != INF) {
                    std::cout << "Inf" << std::endl;
                    return 0;
                }
            }
        }
    }

    Path path{checkPoints.front()};
    for (size_t i = 0; i + 1 < checkPoints.size(); ++i) {
        const auto from = checkPoints[i];
        const auto to = checkPoints[i + 1];
        if (from != to || weights[from][to] > 0) {
            restorePath(viaNodes, from, to, path);
        }
    }

    std::cout << path.size() - 1 << std::endl;
    for (size_t i = 0; i + 1 < path.size(); ++i) {
        const auto from = path[i];
        const auto to = path[i + 1];
        std::cout << edges[from][to] << " ";
    }
    std::cout << std::endl;

    return 0;
}
