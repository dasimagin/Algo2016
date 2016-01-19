#include <iostream>
#include <string>


void print_(std::string& prefix, size_t n)
{
    if (!n) {
        std::cout << prefix << std::endl;
    } else {
        prefix.push_back('0');
        print_(prefix, n - 1);
        prefix.pop_back();
        prefix.push_back('1');
        print_(prefix, n - 1);
        prefix.pop_back();
    }
}

void print(size_t n)
{
    std::string prefix;
    prefix.reserve(n);
    print_(prefix, n);
}

int main() {

    print(3);

    return 0;
}
