#include <cstdint>
#include <iostream>
#include <stdexcept>

std::int64_t math_mod(std::int64_t value, std::int64_t modulus) {
    if (modulus <= 0) {
        throw std::invalid_argument("modulus must be positive");
    }
    const std::int64_t remainder = value % modulus;
    return remainder < 0 ? remainder + modulus : remainder;
}

int main() {
    std::cout << "-3 mod 26 = " << math_mod(-3, 26) << '\n';
}
