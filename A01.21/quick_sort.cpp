#include <algorithm>
#include <chrono>
#include <iostream>
#include <numeric>
#include <random>
#include <utility>
#include <vector>


template<class RandomIt>
void insertSort(RandomIt begin, RandomIt end) {
    if (std::distance(begin, end) < 2) {
        return;
    }
    for (auto i = begin + 1; i != end; ++i) {
        for (auto j = i; *(j - 1) > *j && j != begin; --j) {
            std::iter_swap(j, j - 1);
        }
    }
}

template<class RandomIt>
class QuickSort {

public:
    explicit QuickSort(size_t insertSortSize): insertSortSize_(insertSortSize) {}

    void operator()(RandomIt begin, RandomIt end) {
        const size_t size = end - begin;
        if (size > 1) {
            if (size <= insertSortSize_) {
                insertSort(begin, end);
                return;
            }
            std::uniform_int_distribution<size_t> uniform(0, size - 1);
            const RandomIt it = partition(begin, end, begin + uniform(engine_));
            (*this)(begin, it);
            (*this)(it, end);
        }
    }

private:
    RandomIt partition(RandomIt begin, RandomIt end, RandomIt pivot) {
        std::iter_swap(pivot, begin);
        pivot = begin++;
        for (--end; begin != end; ) {
            if (*begin > *pivot) {
                std::iter_swap(begin, end);
                --end;
            } else {
                ++begin;
            }
            if (begin == end) {
                break;
            } else if (*end < *pivot) {
                std::iter_swap(begin, end);
                ++begin;
            } else {
                --end;
            }
        }
        if (*pivot > *begin) {
            std::iter_swap(pivot, end);
        }
        return begin;
    }

    size_t insertSortSize_;
    std::default_random_engine engine_;
};

std::vector<int> read(std::istream& in) {
    size_t size;
    in >> size;
    std::vector<int> values(size);
    for (auto& value : values) {
        in >> value;
    }
    return values;
}

std::ostream& operator<<(std::ostream& out, const std::vector<int>& values) {
    for (const auto& value : values) {
        out << value << " ";
    }
    return out;
}

std::vector<int> generate(size_t size) {
    std::default_random_engine engine;
    std::uniform_int_distribution<int> uniform;
    std::vector<int> values(size);
    for (auto& value: values) {
        value = uniform(engine);
    }
    return values;
}

template<class RandomIt>
void quickSort(RandomIt begin, RandomIt end) {
    constexpr size_t INSERT_SORT_SIZE = 1000;
    QuickSort<RandomIt>{INSERT_SORT_SIZE}(begin, end);
}

constexpr size_t TEST_N = 100;
constexpr size_t ARRAY_SIZE = 1000000;

int main() {
    std::vector<double> time(TEST_N);

    for (size_t i = 0; i < TEST_N; ++i) {
        std::vector<int> values = generate(ARRAY_SIZE);
        const auto start = std::chrono::high_resolution_clock::now();
        quickSort(values.begin(), values.end());
        const auto end = std::chrono::high_resolution_clock::now();
        time[i] =  std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    }

    std::cout << "Average time = "
              << std::accumulate(time.begin(), time.end(), 0.0) / time.size()
              << " ms." << std::endl;

    return 0;
}
