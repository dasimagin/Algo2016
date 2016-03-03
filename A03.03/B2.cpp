#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include <vector>

std::vector<int> getIntVector(std::istream& in) {
    size_t size;
    in >> size;
    std::vector<int> values(size);
    for (auto& value : values) {
        in >> value;
    }
    return  values;
}

struct Score {
    int previous;
    double value;
};

typedef std::vector<Score> Scores;

bool operator<(const Score& lhs, const Score& rhs) {
    return lhs.value < rhs.value;
}

struct Result {
    double score;
    std::list<int> portions;
};

std::ostream& operator<<(std::ostream& out, const Result& result) {
    out << result.score << std::endl;
    for (const auto& portion : result.portions) {
        out << portion << " ";
    }
    return out << std::endl;
}

Result findMaxScore(const std::vector<int>& dayLimits) {
    std::vector<Scores> result {
        Scores(dayLimits.front(), {0, 0.0})
    };
    for (size_t day = 1; day < dayLimits.size(); ++day) {
        result.push_back(Scores(dayLimits[day], {0, 0.0}));
        for (int current = 1; current <= dayLimits[day]; ++current) {
            for (int previous = 1; previous <= dayLimits[day - 1]; ++previous) {
                const double score = result[day - 1][previous - 1].value +
                    static_cast<double>(current)/previous;
                if (result[day][current - 1].value< score) {
                    result[day][current - 1].value = score;
                    result[day][current - 1].previous = previous;
                }
            }
        }
    }

    auto it = std::max_element(result.back().begin(), result.back().end());
    int previous = static_cast<int>(std::distance(result.back().begin(), it)) + 1;
    std::list<int> portions;
    for (size_t i = dayLimits.size(); i > 0; --i) {
        portions.push_front(previous);
        previous = result[i - 1][previous - 1].previous;
    }

    return {it->value, portions};
}

int main() {
    const std::vector<int> dayLimits = getIntVector(std::cin);
    std::cout.precision(10);
    std::cout << findMaxScore(dayLimits) << std::endl;
    return 0;
}
