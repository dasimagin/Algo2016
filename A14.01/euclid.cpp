#include <iostream>
#include <cassert>

int evklid_(unsigned int a, unsigned int b){
    if (b == 0){
        return a;
    } else {
        return evklid_(b, a % b);
    }
}

int evklid(unsigned int a, unsigned int b){
    assert(a != 0 && b != 0);
    if (a < b) {
        std::swap(a, b);
    }
    return evklid_(a, b);
}
void test(unsigned int a, unsigned int b, unsigned int answ){
    const unsigned int result = evklid(a,b);
    if (result != answ){
        std::cout << "Error: " << "gcd(" << a <<',' << b << ") = "
        << answ << ", but return " << result
        << std::endl;
    }
}
int main(){
    test(30, 18, 6);
    test(124, 2, 2);
    test(85, 561, 17);
    test(7, 3, 1);
    test(16, 8, 8);
    return 0;
}
