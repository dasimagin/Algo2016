#include <iostream>

void hanoi(int n, int i, int j, int k) {
    if (n > 0) {
        hanoi(n - 1, i, k, j);
        std::cout << i << " " << j << std::endl;
        hanoi(n - 1, k, j, i);
    }
}

int main()
{
    hanoi(3, 1, 2, 3);
    return 0;
}
