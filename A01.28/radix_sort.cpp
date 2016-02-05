#include <cstdint>
#include <iostream>
#include <vector>


uint8_t getByte(const uint64_t& value, size_t n) {
    return *((uint8_t*)&value + n);
}

void radixSort(std::vector<uint64_t>& values) {
    std::vector<size_t> index;
    for (size_t i = 0; i < values.size(); ++i) {
        index.push_back(i);
    }

    for (size_t n = 0; n < sizeof(uint64_t); ++n) {
        std::vector<std::vector<size_t>> buckets(256);
        for (const auto& i: index) {
            buckets[getByte(values[i], n)].push_back(i);
        }
        index.clear();
        for (const auto& bucket: buckets) {
            for (const auto& i: bucket) {
                index.push_back(i);
            }
        }
    }

    std::vector<uint64_t> result;
    for (const auto& i: index) {
        result.push_back(values[i]);
    }

    values = std::move(result);
}

std::vector<uint64_t> read(std::istream& in) {
    size_t size;
    in >> size;
    std::vector<uint64_t> values(size);
    for (auto& value: values) {
        in >> value;
    }
    return values;
}

std::ostream& operator<<(std::ostream& out, const std::vector<uint64_t>& values) {
    for (const auto& value: values) {
        out << value << " ";
    }
    return out;
}

int main() {
    std::vector<uint64_t> values = read(std::cin);
    radixSort(values);
    std::cout << values << std::endl;
    return 0;
}

