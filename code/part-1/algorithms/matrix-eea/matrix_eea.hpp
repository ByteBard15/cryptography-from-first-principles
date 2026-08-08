#pragma once

#include <cstdint>
#include <stdexcept>

namespace cffp {

struct Matrix2x2 {
    std::int64_t m11;
    std::int64_t m12;
    std::int64_t m21;
    std::int64_t m22;
};

struct MatrixEeaResult {
    std::int64_t gcd;
    Matrix2x2 transform;
};

constexpr Matrix2x2 multiply(const Matrix2x2& left,
                             const Matrix2x2& right) {
    return {
        left.m11 * right.m11 + left.m12 * right.m21,
        left.m11 * right.m12 + left.m12 * right.m22,
        left.m21 * right.m11 + left.m22 * right.m21,
        left.m21 * right.m12 + left.m22 * right.m22,
    };
}

constexpr MatrixEeaResult matrix_eea(std::int64_t a, std::int64_t b) {
    if (a < 0 || b < 0) {
        throw std::invalid_argument("matrix_eea expects non-negative inputs");
    }

    Matrix2x2 transform{1, 0, 0, 1};
    while (b != 0) {
        const std::int64_t quotient = a / b;
        const Matrix2x2 transition{0, 1, 1, -quotient};
        transform = multiply(transition, transform);

        const std::int64_t remainder = a % b;
        a = b;
        b = remainder;
    }
    return {a, transform};
}

}  // namespace cffp
