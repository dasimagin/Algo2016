#include<algorithm>
#include<functional>
#include<iostream>
#include<vector>

struct Collector {
    size_t id;
    size_t minStamp;
    size_t diffStamp;

    Collector(size_t id, size_t minStamp)
    : id(id)
    , minStamp(minStamp)
    , diffStamp(0)
    {}
};

std::vector<size_t> getDiegoStamps(std::istream& in) {
    size_t size;
    in >> size;
    std::vector<size_t> values(size);
    for (auto& value : values) {
        in >> value;
    }
    return values;
}

std::vector<Collector> getCollectors(std::istream& in) {
    size_t size;
    in >> size;
    std::vector<Collector> collectors;
    size_t minStamp;
    for (size_t i = 0; i < size; ++i) {
        in >> minStamp;
        collectors.emplace_back(i, minStamp);
    }
    return collectors;
}

int main() {
    std::vector<size_t> diegoStamps = getDiegoStamps(std::cin);
    std::sort(diegoStamps.begin(), diegoStamps.end(), std::greater<size_t>());
    diegoStamps.erase(std::unique(diegoStamps.begin(), diegoStamps.end()), diegoStamps.end());

    std::vector<Collector> collectors = getCollectors(std::cin);
    std::sort(
        collectors.begin(), collectors.end(),
        [](const Collector& lhs, const Collector& rhs) {
          return lhs.minStamp > rhs.minStamp;
        }
    );

    auto it = diegoStamps.begin();
    for (auto& collector : collectors) {
        it = std::find_if(
            it, diegoStamps.end(),
            [&collector](const size_t& stamp) {
             return stamp < collector.minStamp;
            }
        );
        collector.diffStamp = std::distance(it, diegoStamps.end());
    }

    std::sort(
        collectors.begin(), collectors.end(),
        [](const Collector& lhs, const Collector& rhs) {
          return lhs.id < rhs.id;
        }
    );
    for (const auto& collector : collectors) {
        std::cout << collector.diffStamp << "\n";
    }

    return 0;
}
