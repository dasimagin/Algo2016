#include <cstdint>
#include <iostream>
#include <string>
#include <vector>


namespace fibonacci {

    const std::vector<uint64_t> firstFiveValues {1, 1, 2, 3, 5};

    uint64_t multiply_recursive(uint64_t n)
    {
        if (n < 2) {
            return 1;
        }
        return multiply_recursive(n - 1) + multiply_recursive(n - 2);
    }

    std::pair<uint64_t, uint64_t> single_recursive_(uint64_t n)
    {
        if (n == 0) {
            return {1, 1};
        }

        const auto pair = single_recursive_(n - 1);

        return {pair.second + pair.first, pair.first};
    }

    uint64_t single_recursive(uint64_t n)
    {
        return single_recursive_(n).second;
    }

    void test(const std::string& name, std::function<uint64_t(uint64_t)> implementation)
    {
        for (size_t n = 0; n < firstFiveValues.size(); ++n) {
            const uint64_t result = implementation(n);
            if (result != firstFiveValues[n]) {
                std::cout << name << ": F" << n << " = " << firstFiveValues[n]
                << ", but return " << result << std::endl;
            }
        }
    }

} // end of fibonacci namespace


int main() {

    fibonacci::test("Recursive implementation", &fibonacci::multiply_recursive);
    fibonacci::test("Iterative implementation", &fibonacci::single_recursive);

    return 0;
}
