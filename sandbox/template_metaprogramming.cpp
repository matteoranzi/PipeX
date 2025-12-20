//
// Created by Matteo Ranzi on 12/12/25.
//

#include <iostream>

template <unsigned int n>
struct Factorial {
    constexpr static unsigned int value = Factorial<n - 1>::value * n;
};

template<>
struct Factorial<0> {
    constexpr static unsigned int value = 1;
};

int main() {
    constexpr unsigned int num = 5;
    std::cout << "Factorial of " << num << " is: " << Factorial<num>::value << std::endl;

    return 0;
}