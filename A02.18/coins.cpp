#include<algorithm>
#include<iostream>
#include<map>
#include<vector>


std::vector<size_t> getCoinsSet(std::istream& in, size_t size) {
    std::vector<size_t> coins(size);
    for (size_t i = 0; i < size; ++i) {
        in >> coins[i];
    }
    return coins;
}


class Solver {

public:

    Solver(std::vector<size_t> coins): coins_(std::move(coins)) {
        std::sort(coins_.begin(), coins_.end());
    }

    size_t operator()(size_t sum) {
        size_t result = 0;
        for (const auto& coin: coins_) {
            result += (*this)(sum, coin);
        }
        return  result;
    }

    size_t operator()(size_t sum, size_t lastCoin) {
        if (cache_[sum].count(lastCoin)) {
            return cache_[sum][lastCoin];
        }

        if (sum < lastCoin) {
            return cache_[sum][lastCoin] = 0;
        } else if (sum == lastCoin) {
            return cache_[sum][lastCoin] = 1;
        }

        size_t result = 0;
        for (const auto& coin: coins_) {
            if (coin > lastCoin) {
                break;
            }
            result += (*this)(sum - lastCoin, coin);
        }

        return cache_[sum][lastCoin] = result;
    }

private:
    std::vector<size_t> coins_;
    std::map<size_t, std::map<size_t, size_t>> cache_;
};


int main() {
    size_t sum, size;
    std::cin >> sum >> size;

    std::cout << Solver{getCoinsSet(std::cin, size)}(sum) << std::endl;

    return 0;
}
