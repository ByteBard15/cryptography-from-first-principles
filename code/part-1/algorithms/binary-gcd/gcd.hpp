#pragma once

#include <bit>
#include <cstdint>
#include <utility>

namespace cffp {

constexpr std::uint64_t binary_gcd(std::uint64_t a, std::uint64_t b) {
    if (a == 0) {
        return b;
    }
    if (b == 0) {
        return a;
    }

    const unsigned shared_power = std::countr_zero(a | b);
    a >>= std::countr_zero(a);

    while (b != 0) {
        b >>= std::countr_zero(b);
        if (a > b) {
            std::swap(a, b);
        }
        b -= a;
    }

    return a << shared_power;
}

}  // namespace cffp
