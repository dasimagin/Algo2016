#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class UInt {
    static constexpr uint64_t BASE = 1000000000000000000;
    static constexpr uint64_t POWER = 18;

public:
    UInt(uint64_t value) {
        if (value < BASE) {
            digits_ = {value};
        } else {
            digits_ = {value % BASE, value / BASE};
        }
    }

    UInt& operator+=(const UInt& other) {
        if (other.digits_.size() > digits_.size()) {
            digits_.resize(other.digits_.size());
        }

        uint64_t buffer = 0;
        for (size_t i = 0; i < digits_.size(); ++i) {
            uint64_t& digit = digits_[i];
            if (i < other.digits_.size()) {
                digit += other.digits_[i] + buffer;
            } else {
                if (buffer == 0) {
                    break;
                }
                digit += buffer;
            }
            buffer = digit / BASE;
            digit %= BASE;
        }
        if (buffer) {
            digits_.push_back(buffer);
        }

        return *this;
    }

    std::string toString() const {
        std::stringstream ostream;
        for (auto it = digits_.rbegin(); it != digits_.rend(); ++it) {
            const std::string digit = std::to_string(*it);
            if (it != digits_.rbegin()) {
                for (size_t i = 0; i < POWER - digit.length(); ++i) {
                    ostream << "0";
                }
            }
            ostream << digit;
        }

        return ostream.str();
    }

private:
    std::vector<uint64_t> digits_;
};

const std::vector<std::vector<size_t>> NEXT_DIGITS {
    /* from 0 */ {4, 6},
    /* from 1 */ {6, 8},
    /* from 2 */ {7, 9},
    /* from 3 */ {4, 8},
    /* from 4 */ {0, 3, 9},
    /* from 5 */ {},
    /* from 6 */ {0, 1, 7},
    /* from 7 */ {2, 6},
    /* from 8 */ {1, 3},
    /* from 9 */ {2, 4},
};

UInt countNumbers(size_t length, const std::vector<size_t>& startWith) {
    std::vector<std::vector<UInt>> counters {
        std::vector<UInt>(10, 1)
    };
    counters.reserve(length);
    for (size_t currentLength = 2; currentLength <= length; ++currentLength) {
        std::vector<UInt> current(10, 0);
        for (size_t digit = 0; digit <= 9; ++digit) {
            for (const auto& next : NEXT_DIGITS[digit]) {
                current[digit] += counters.back()[next];
            }
        }
        counters.push_back(current);
    }

    UInt result = 0;
    for (const auto& digit : startWith) {
        result += counters.back()[digit];
    }
    return result;
}

int main() {
    size_t length;
    std::cin >> length;
    std::cout << countNumbers(length, {1, 2, 3, 4, 5, 6, 7, 9}).toString() << std::endl;
    return 0;
}
