#include <iostream>
#include <iterator>
#include <vector>

template<class RandomIt, class T>
RandomIt search(RandomIt begin, RandomIt end, const T& value) {
    const size_t size = std::distance(begin, end);
    if (size == 0) {
        return end;
    } else if (size == 1) {
        return (*begin == value) ? begin : end;
    }

    const RandomIt middle = begin + size / 2;
    if (*middle > value) {
        RandomIt it =  search(begin, middle, value);
        return it == middle ? end : it;
    } else {
        RandomIt it =  search(middle, end, value);
        return it == end ? end : it;
    }
}

template<class RandomIt, class T>
void test(const std::string name, RandomIt begin, RandomIt end, const T& value, RandomIt answer) {
    const RandomIt result = search(begin, end, value);
    if (result != answer) {
        std::cout << name << ": error!" << std::endl;
    }
};


int main() {
    std::vector<int> vector {1, 2, 3, 4, 5};

    test("Test-1", vector.begin(), vector.end(), 2, vector.begin() + 1);
    test("Test-2", vector.begin(), vector.end(), 13, vector.end());
}
