---
title: Binary GCD (Stein’s Algorithm)
nav_title: Binary GCD
order: 4
description: Replace division with parity tests, shifts, and subtraction.
---

# Binary GCD (Stein’s Algorithm)

In 1967, Josef Stein introduced **Binary GCD**, replacing general division with three machine-friendly primitives:

1. **Parity checks** (`a & 1 == 0`) test whether an integer is even or odd.
2. **Bit shifts** (`a >> 1`) divide an even integer by 2.
3. **Subtractions** (`a - b`) reduce odd numbers.

*(The source points to Menezes et al. (1996), HAC Algorithm 14.54, for pseudocode and to Brent (2000) for average-case analysis.)*

## Four identities

The algorithm is built from these identities:

1. Base cases:

   $$
   \gcd(0,b)=b,
   \qquad
   \gcd(a,0)=a.
   $$

2. If both inputs are even, factor out their shared 2:

   $$
   \gcd(2k,2m)=2\gcd(k,m).
   $$

3. If exactly one input is even, discard its unshared factor of 2:

   $$
   \gcd(2k,m)=\gcd(k,m) \quad\text{for odd }m.
   $$

4. If both are odd, their difference is even:

   $$
   \gcd(a,b)=\gcd(|a-b|,\min(a,b)).
   $$

## Trace: $\gcd(24,15)$

1. $\gcd(24,15)\rightarrow\gcd(12,15)$ because 24 is even and 15 is odd.
2. $\gcd(12,15)\rightarrow\gcd(6,15)$.
3. $\gcd(6,15)\rightarrow\gcd(3,15)$.
4. Both are odd, so $\gcd(3,15)=\gcd(12,3)$.
5. Strip powers of 2: $\gcd(12,3)\rightarrow\gcd(6,3)\rightarrow\gcd(3,3)$.
6. Subtract the equal odd values: $\gcd(3,3)=\gcd(0,3)$.
7. The base case gives 3.

On modern C++, `std::countr_zero` can strip all factors of 2 represented by trailing zero bits at once:

```text
176 = 0b10110000
176 >> 4 = 11
```

## C++ implementation

```cpp
#include <bit>
#include <cstdint>
#include <utility>

// Computes GCD using shifts, subtraction, and trailing-zero counts.
constexpr std::uint64_t binary_gcd(std::uint64_t a,
                                   std::uint64_t b) {
    if (a == 0) {
        return b;
    }
    if (b == 0) {
        return a;
    }

    // Count and preserve the powers of 2 shared by both inputs.
    const unsigned shared_power = std::countr_zero(a | b);

    // Reduce a to an odd number.
    a >>= std::countr_zero(a);

    while (b != 0) {
        // Reduce b to an odd number.
        b >>= std::countr_zero(b);

        // Both are odd. Keep a <= b so the subtraction is non-negative.
        if (a > b) {
            std::swap(a, b);
        }

        // The difference of two odd numbers is even.
        b -= a;
    }

    return a << shared_power;
}
```

The complete implementation and [tests]({{ '/code/part-1/tests/' | relative_url }}) are also available in the [binary GCD source directory]({{ '/code/part-1/algorithms/binary-gcd/' | relative_url }}).

## Trade-offs

| Metric | Classical Euclidean Algorithm | Binary GCD |
| --- | --- | --- |
| Core primitives | Integer modulo (`%`) | Shifts (`>>`), subtraction (`-`), trailing zeros |
| Branch predictability | High, with a regular loop | Medium, with parity checks and swaps |
| 64-bit performance claimed by source | Fast for native integers | Up to 2× faster with hardware CTZ |
| Big-integer claim in source | Division-heavy | Shift/subtraction primitives avoid general division |
| Source-stated bit complexity | $O(n^2)$ | $O(n^2)$ average-case |

<!-- REVIEW: mathematical claim requires verification -->
The source claims binary GCD can be “up to 2x faster” for 64-bit inputs and “extremely fast” for big integers. Those empirical statements are not universal; the [experiment plan]({{ '/experiments/part-1/' | relative_url }}) preserves them as hypotheses to measure.

The supplementary [binary GCD bit-complexity proof]({{ '/proofs/part-1/binary-gcd-complexity/' | relative_url }}) establishes the $O(n^2)$ bound under an explicit bit-operation model.

[Next: Extended Euclidean Algorithm →]({{ '/part-1/05-extended-euclidean-algorithm/' | relative_url }})
