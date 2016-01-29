#include <iostream>
#include <iterator>
#include <vector>


template<class InputItOne, class InputItTwo, class OutputIt>
void merge(InputItOne beginOne, InputItOne endOne,
            InputItTwo beginTwo, InputItTwo endTwo,
            OutputIt out) {
    for (; beginOne != endOne && beginTwo != endTwo; ++out) {
        *out = (*beginOne < *beginTwo) ? std::move(*beginOne++) : std::move(*beginTwo++);
    }
    std::move(beginOne, endOne, out);
    std::move(beginTwo, endTwo, out);
}

template<class RandomIt>
void merge_sort(RandomIt begin, RandomIt end) {
    const size_t size = std::distance(begin, end);
    if (size < 2) {
        return;
    }
    RandomIt middle = begin + size / 2;
    merge_sort(begin, middle);
    merge_sort(middle, end);

    std::vector<typename RandomIt::value_type> buffer;
    merge(begin, middle, middle, end, std::back_inserter(buffer));
    std::move(buffer.begin(), buffer.end(), begin);
}

std::ostream& operator<<(std::ostream& out, const std::vector<int>& values) {
    for (const auto& value : values) {
        out << value << " ";
    }
    return out;
}

std::vector<int> read(std::istream& in) {
    size_t size;
    in >> size;
    std::vector<int> values(size);
    for (auto& value : values) {
        in >> value;
    }
    return values;
}

int main() {
    std::vector<int> values = read(std::cin);
    merge_sort(values.begin(), values.end());
    std::cout << values << std::endl;
    return 0;
}
