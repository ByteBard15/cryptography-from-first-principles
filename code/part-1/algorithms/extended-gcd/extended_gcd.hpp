#pragma once

#include <cstdint>
#include <optional>

namespace cffp {

struct ExtendedGcdResult {
    std::int64_t gcd;
    std::int64_t x;
    std::int64_t y;
};

constexpr ExtendedGcdResult extended_gcd(std::int64_t a, std::int64_t b) {
    std::int64_t old_r = a;
    std::int64_t r = b;
    std::int64_t old_x = 1;
    std::int64_t x = 0;
    std::int64_t old_y = 0;
    std::int64_t y = 1;

    while (r != 0) {
        const std::int64_t quotient = old_r / r;

        const std::int64_t next_r = old_r - quotient * r;
        old_r = r;
        r = next_r;

        const std::int64_t next_x = old_x - quotient * x;
        old_x = x;
        x = next_x;

        const std::int64_t next_y = old_y - quotient * y;
        old_y = y;
        y = next_y;
    }

    return {old_r, old_x, old_y};
}

constexpr std::optional<std::int64_t> modular_inverse(std::int64_t a,
                                                       std::int64_t modulus) {
    if (a < 0 || modulus <= 1) {
        return std::nullopt;
    }
    const ExtendedGcdResult result = extended_gcd(a, modulus);
    if (result.gcd != 1) {
        return std::nullopt;
    }
    return (result.x % modulus + modulus) % modulus;
}

}  // namespace cffp
