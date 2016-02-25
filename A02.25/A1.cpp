#include<algorithm>
#include<iostream>
#include<vector>

std::vector<int> getIntVector(std::istream& in) {
    size_t size;
    in >> size;
    std::vector<int> values(size);
    for (auto& value : values) {
        in >> value;
    }
    return values;
}

std::ostream& operator<<(std::ostream& out, const std::vector<size_t>& values) {
    for (const auto& value : values) {
        out << value << std::endl;
    }
    return out;
}

template<class RandomIt>
std::vector<size_t> pancakeSort(RandomIt begin, RandomIt end) {
    std::vector<size_t> indexes;
    for (; begin != end; --end) {
        const auto it = std::max_element(begin, end) + 1;
        if (it != end) {
            indexes.push_back(std::distance(begin, it));
            std::reverse(begin, it);
            indexes.push_back(std::distance(begin, end));
            std::reverse(begin, end);
        }
    }
    return indexes;
}

int main() {
    auto values = getIntVector(std::cin);
    std::cout << pancakeSort(values.begin(), values.end());
    return 0;
}
