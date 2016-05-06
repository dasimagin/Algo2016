#include <algorithm>
#include <cstdint>
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <vector>


class UInt {

public:

    static constexpr uint64_t BASE = 1000000000000000000;
    static constexpr uint64_t POWER = 18;

    UInt(uint64_t value) {
        if (value < BASE) {
            digits_ = {value};
        } else {
            digits_ = {value % BASE, value / BASE};
        }
    }

    UInt operator+(const UInt& other) const {
        UInt result = *this;

        if (other.digits_.size() > result.digits_.size()) {
            result.digits_.resize(other.digits_.size());
        }

        uint64_t buffer = 0;
        for (size_t i = 0; i < result.digits_.size(); ++i) {
            uint64_t& digit = result.digits_[i];
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
            result.digits_.push_back(buffer);
        }

        return result;
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


typedef std::list<std::string> Bin;
typedef std::vector<Bin> HashTable;


HashTable precomputeFibonacciNumbers(size_t maxLength) {
    HashTable table(maxLength + 1);

    for (UInt previous = 0, current = 1; ; ) {
        UInt next = previous + current;
        std::string representation = next.toString();

        if (representation.length() > maxLength) {
            break;
        }

        table[representation.size()].push_back(std::move(representation));
        previous = std::move(current);
        current = std::move(next);
    }

    return table;
}


const size_t MAX_LENGTH = 5000;


int main() {
    const HashTable fibonacciNumbers = precomputeFibonacciNumbers(MAX_LENGTH);

    size_t n;
    std::cin >> n;

    for (std::string number; n; --n) {
        std::cin >> number;
        const Bin& bin = fibonacciNumbers[number.size()];
        if (std::find(bin.begin(), bin.end(), number) != bin.end()) {
            std::cout << "Yes\n";
        } else {
            std::cout << "No\n";
        }
    }

    return 0;
}
