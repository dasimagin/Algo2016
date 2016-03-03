#include <iostream>
#include <vector>


inline int64_t findPenalty(int64_t length) {
    const int64_t diff = length - 200;
    return diff * diff;
}

std::vector<int64_t> getDistances(std::istream& in) {
    size_t size;
    in >> size;
    std::vector<int64_t> values(size);
    for (auto& value: values) {
        in >> value;
    }
    return values;
}

size_t findMinPenalty(const std::vector<int64_t>& distances) {
    std::vector<int64_t> penalties;
    penalties.reserve(distances.size());

    for (size_t current = 0; current < distances.size(); ++current) {
        int64_t minPenalty = findPenalty(distances[current]);
        for (size_t previous = 0; previous < current; ++previous) {
            const int64_t diff = distances[current] - distances[previous];
            const int64_t penalty = penalties[previous] + findPenalty(diff);
            if (penalty < minPenalty) {
                minPenalty = penalty;
            }
        }
        penalties.push_back(minPenalty);
    }

    return penalties.back();
}

int main() {
    const std::vector<int64_t> distances = getDistances(std::cin);
    std::cout << findMinPenalty(distances) << std::endl;
    return 0;
}
