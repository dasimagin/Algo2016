#include <iostream>
#include <algorithm>
#include <vector>

void sort(std::vector<int>& elements) {
    for (size_t i = 1; i != elements.size(); ++i) {
        for (size_t j = i; j != 0; --j) {
            if (elements[j] < elements[j - 1]) {
                std::swap(elements[j], elements[j - 1]);
            }
        }
    }
}

int main() {
    std::vector<int> elements;
    for (int x; std::cin >> x; ) {
        elements.push_back(x);
    }
    sort(elements);
    for (size_t n = 0; n != elements.size(); ++n) {
        std::cout << elements[n] << " ";
    }
}
