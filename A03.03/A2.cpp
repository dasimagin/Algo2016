#include <algorithm>
#include <iostream>
#include <vector>


std::vector<uint64_t> getUInt64Vector(std::istream& in, size_t size) {
    std::vector<uint64_t> values(size);
    for (auto& value : values) {
        in >> value;
    }
    return  values;
}

uint64_t findMaxProfit(
       const std::vector<uint64_t>& distances,
       const std::vector<uint64_t>& profits,
       size_t minDistance) {
    if (distances.size() == 0) {
        return 0;
    }

    std::vector<uint64_t> result{profits[0]};
    size_t current = 1;
    for (; current < distances.size(); ++current) {
        if (distances[current] - distances[0] >= minDistance) {
            break;
        }
        result.push_back(std::max(profits[current], result[current - 1]));
    }

    for (size_t previous = 0; current < distances.size(); ++current) {
        for (; previous + 1 < current; ++previous) {
            if (distances[current] - distances[previous + 1] < minDistance) {
                break;
            }
        }
        result.push_back(std::max(profits[current] + result[previous], result[current - 1]));
    }

    return  result.back();
}

int main() {
    size_t size, minDistance;
    std::cin >> size >> minDistance;
    const std::vector<uint64_t> distances = getUInt64Vector(std::cin, size);
    const std::vector<uint64_t> profits = getUInt64Vector(std::cin, size);
    std::cout << findMaxProfit(distances, profits, minDistance) << std::endl;;

    return 0;
}
