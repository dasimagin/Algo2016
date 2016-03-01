#include <iostream>
#include <vector>

#include "nth_element.h"


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    
    size_t k;
    size_t l;
    std::cin >> l >> k;
    std::vector<int> nums(l);
    for (auto &i: nums) {
        std::cin >> i;
    }
    
    std::cout << *qselect::nth_element(nums.begin(), nums.begin() + k, nums.end()) << "\n";
    
    return 0;
}
