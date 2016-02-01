#include <iostream>
#include <vector>

typedef std::vector<size_t> Stick;
typedef std::vector<Stick> Sticks;


std::ostream& operator<<(std::ostream& out, const Stick& stick) {
    std::cout << "[ ";
    for (const auto& ring: stick) {
        out << ring << " ";
    }
    return out << "]" << std::endl;
}

std::ostream& operator<<(std::ostream& out, const Sticks& sticks) {
    for (const auto& stick: sticks) {
        std::cout << stick;
    }

    return out << std::endl;
}


void hanoi(Sticks& sticks, size_t n, size_t from, size_t to, size_t helper) {
    if (n > 0) {
        hanoi(sticks, n - 1, from, helper, to);

        sticks[to].push_back(sticks[from].back());
        sticks[from].pop_back();
        std::cout << sticks;

        hanoi(sticks, n - 1, helper, to, from);
    }
}

void hanoi(size_t n) {
    Sticks sticks(3);
    for (size_t i = n; i > 0; --i) {
        sticks[0].push_back(i);
    }

    std::cout << sticks;
    hanoi(sticks, n, 0, 1, 2);
}

int main() {
    hanoi(5);
    return 0;
}
