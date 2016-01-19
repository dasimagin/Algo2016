#include <cstdint>
#include <iostream>
#include <string>
#include <vector>


namespace factorial {

const std::vector<uint64_t> firstFiveValues {1, 1, 2, 6, 24};

uint64_t recursive(uint64_t n)
{
    if (n < 2) {
        return 1;
    }
    return n * recursive(n - 1);
}

uint64_t iterative(uint64_t n)
{
    uint64_t result = 1;

    for (; n > 1; --n) {
        result *= n;
    }

    return result;
}

void test(const std::string& name, std::function<uint64_t(uint64_t)> implementation)
{
    for (size_t n = 0; n < firstFiveValues.size(); ++n) {
        const uint64_t result = implementation(n);
        if (result != firstFiveValues[n]) {
            std::cout << "name: " << n << "! = " << firstFiveValues[n]
            << ", but return " << result << std::endl;
        }
    }

}

} // end of factorial namespace


int main() {

    factorial::test("Recursive implementation", &factorial::recursive);
    factorial::test("Iterative implementation", &factorial::iterative);

    return 0;
}
