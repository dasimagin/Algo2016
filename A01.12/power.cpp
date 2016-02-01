#include <cassert>
#include <iostream>


int power(int n, size_t p) {

    assert(n);

    if (p == 0) {
        return 1;
    } else if (p == 1) {
        return n;
    }

    const int bufer = power(n, p/2);

    return (p % 2) ? (n * bufer * bufer) : (bufer * bufer);

}

void test(int n, size_t p, int answer)
{
    const int result = power(n , p);
    if (result != answer) {
        std::cout << "Error: " << n << "^" << p << " = " << answer
        << ", but return " << result << std::endl;
    }
}

int main() {

    test(3, 0, 1);
    test(2, 1, 2);
    test(-1, 100, 1);
    test(2, 6,  64);
    test(3, 11,  177147);

    return 0;
}
