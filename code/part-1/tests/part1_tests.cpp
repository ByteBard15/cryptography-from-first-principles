#include "../algorithms/binary-gcd/gcd.hpp"
#include "../algorithms/euclidean/gcd.hpp"
#include "../algorithms/extended-gcd/extended_gcd.hpp"
#include "../algorithms/matrix-eea/matrix_eea.hpp"

#include <cassert>
#include <cstdint>
#include <numeric>

int main() {
    static_assert(cffp::euclidean_gcd(252, 105) == 21);
    static_assert(cffp::binary_gcd(24, 15) == 3);
    static_assert(cffp::euclidean_gcd(0, 0) == 0);
    static_assert(cffp::binary_gcd(0, 27) == 27);

    for (std::uint64_t a = 0; a < 200; ++a) {
        for (std::uint64_t b = 0; b < 200; ++b) {
            const std::uint64_t expected = std::gcd(a, b);
            assert(cffp::euclidean_gcd(a, b) == expected);
            assert(cffp::binary_gcd(a, b) == expected);
        }
    }

    const cffp::ExtendedGcdResult extended = cffp::extended_gcd(26, 15);
    assert(extended.gcd == 1);
    assert(26 * extended.x + 15 * extended.y == extended.gcd);
    assert(cffp::modular_inverse(15, 26) == 7);
    assert(!cffp::modular_inverse(6, 15).has_value());

    const cffp::MatrixEeaResult matrix = cffp::matrix_eea(26, 15);
    assert(matrix.gcd == 1);
    assert(matrix.transform.m11 == -4);
    assert(matrix.transform.m12 == 7);
    assert(matrix.transform.m11 * 26 + matrix.transform.m12 * 15 == 1);
}
