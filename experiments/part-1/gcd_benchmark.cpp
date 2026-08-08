#include "../../code/part-1/algorithms/binary-gcd/gcd.hpp"
#include "../../code/part-1/algorithms/euclidean/gcd.hpp"

#include <chrono>
#include <cstdint>
#include <iostream>
#include <random>
#include <utility>
#include <vector>

using Input = std::pair<std::uint64_t, std::uint64_t>;

template <typename Gcd>
std::chrono::nanoseconds measure(const std::vector<Input>& inputs, Gcd gcd) {
    volatile std::uint64_t checksum = 0;
    const auto start = std::chrono::steady_clock::now();
    for (const auto& [a, b] : inputs) {
        checksum = checksum ^ gcd(a, b);
    }
    const auto stop = std::chrono::steady_clock::now();
    std::cout << "checksum: " << checksum << '\n';
    return std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
}

int main() {
    constexpr std::size_t sample_count = 1'000'000;
    std::mt19937_64 generator(0xC0FFEE);
    std::vector<Input> inputs;
    inputs.reserve(sample_count);
    for (std::size_t i = 0; i < sample_count; ++i) {
        inputs.emplace_back(generator(), generator());
    }

    const auto euclidean = measure(inputs, cffp::euclidean_gcd);
    const auto binary = measure(inputs, cffp::binary_gcd);
    std::cout << "Euclidean: " << euclidean.count() << " ns\n";
    std::cout << "Binary:    " << binary.count() << " ns\n";
}
