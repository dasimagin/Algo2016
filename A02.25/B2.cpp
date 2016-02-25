#include<algorithm>
#include<functional>
#include<iostream>
#include<vector>


struct Singer {
    size_t weight;
    size_t town;

    Singer(size_t weight, size_t town): weight(weight), town(town) {}

    bool operator<(const Singer& other) const {
        return weight < other.weight;
    }
};

std::vector<Singer> getSingers(std::istream& in) {
    std::vector<Singer> singers;
    for (size_t town = 0; town < 4; ++town) {
        size_t size;
        std::cin >> size;
        for (size_t weight; size; --size) {
            std::cin >> weight;
            singers.emplace_back(weight, town);
        }
    }
    return singers;
}


int main() {
    std::vector<Singer> singers = getSingers(std::cin);
    std::sort(singers.begin(), singers.end());

    std::vector<size_t> current{0, 0, 0, 0};
    auto it = singers.begin();
    for (bool initialized = false; !initialized; ++it) {
        current[it->town] = it->weight;
        initialized = std::all_of(
            current.begin(), current.end(),
            [](const size_t n){ return n > 0;});
    }

    std::vector<size_t> result = current;
    const auto minMax = std::minmax_element(current.begin(), current.end());
    size_t resultDiff = *minMax.second - *minMax.first;
    for (; it != singers.end(); ++it) {
        current[it->town] = it->weight;
        const size_t minWeight = *std::min_element(current.begin(), current.end());
        const size_t currentDiff = it->weight - minWeight;
        if (currentDiff < resultDiff) {
            result = current;
            resultDiff = currentDiff;
        }
    }

    for (const auto& town : result) {
        std::cout << town << " ";
    }
    std::cout << std::endl;

    return 0;
}
