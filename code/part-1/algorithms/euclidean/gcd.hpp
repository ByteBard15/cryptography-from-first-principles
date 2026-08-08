#pragma once

#include <cstdint>

namespace cffp {

constexpr std::uint64_t euclidean_gcd(std::uint64_t a, std::uint64_t b) {
    while (b != 0) {
        const std::uint64_t remainder = a % b;
        a = b;
        b = remainder;
    }
    return a;
}

}  // namespace cffp
