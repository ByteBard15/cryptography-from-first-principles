---
title: The Euclidean Algorithm
nav_title: Euclidean Algorithm
order: 3
description: Compute greatest common divisors without factoring.
---

# The Euclidean Algorithm

Naive prime factorization decomposes numbers into prime factors. For example, $24=2^3\cdot3$ and $36=2^2\cdot3^2$, so $\gcd(24,36)=2^2\cdot3=12$. However, integer factorization takes exponential time $O(2^b)$ relative to bit length $b$. Public-key security explicitly relies on factorization being intractable.

<!-- REVIEW: mathematical claim requires verification -->
The precise $O(2^b)$ factorization claim above requires verification and a stated computational model.

Euclid’s algorithm bypasses factorization entirely, solving GCD in a logarithmic number of division steps.

## The invariant

Euclid’s algorithm rests on a simple observation: if $d$ divides both $a$ and $b$, then it divides their difference. In fact,

$$
\gcd(a,b)=\gcd(a-b,b).
$$

### Proof that $\gcd(a,b)=\gcd(a-b,b)$

Let $D(a,b)$ represent the set of common divisors of $a$ and $b$.

1. Suppose $c\in D(a,b)$. By definition, $c\mid a$ and $c\mid b$. Since $c$ divides both numbers, it divides their difference: $c\mid(a-b)$. Therefore $c\in D(a-b,b)$.
2. Conversely, suppose $c\in D(a-b,b)$. Then $c\mid(a-b)$ and $c\mid b$. Divisibility is preserved under addition, so

   $$
   c\mid((a-b)+b),
   $$

   which gives $c\mid a$. Thus $c\in D(a,b)$.

Because $D(a,b)=D(a-b,b)$, the pairs have identical common divisors and therefore the same greatest common divisor:

$$
\gcd(a,b)=\gcd(a-b,b).
$$

Replacing repeated subtraction with quotient-based remainders, write

$$
a=qb+r,
$$

and obtain

$$
\gcd(a,b)=\gcd(b,a\bmod b),
\qquad
a\bmod b=a-\left\lfloor\frac ab\right\rfloor b.
$$

Each nonzero remainder is smaller than the preceding divisor. The non-negative remainders therefore descend until one is zero; the last nonzero remainder is the gcd.

```text
gcd(252, 105) → gcd(105, 42)
              → gcd(42, 21)
              → gcd(21, 0)
              = 21
```

### C++ implementation

```cpp
#include <cstdint>

// Computes GCD using division remainders.
// The implementation uses O(log(min(a, b))) division steps.
constexpr std::uint64_t euclidean_gcd(std::uint64_t a,
                                      std::uint64_t b) {
    while (b != 0) {
        const std::uint64_t remainder = a % b;
        a = b;
        b = remainder;
    }
    return a;
}
```

The implementation and its tests also remain available in the [Euclidean source directory]({{ '/code/part-1/algorithms/euclidean/' | relative_url }}).

## Lamé’s theorem and complexity

Consecutive Fibonacci inputs give the worst case because the nonterminal quotients are 1. If $d$ is the number of decimal digits in the smaller positive input, Lamé’s theorem gives

$$
k\le5d,
\qquad
d=\left\lfloor\log_{10}(\min(a,b))\right\rfloor+1.
$$

<!-- REVIEW: corrected the source article's imprecise shorthand k <= 5 log10(min(a,b)); Lamé's theorem is stated in terms of the integer decimal-digit count. -->

It distinguishes the number of division steps, logarithmic in the input value, from the $O(n^2)$ bit-operation cost stated for classical division GCD on $n$-bit integers.

The supplementary research archive now contains the complete [Euclidean bit-complexity analysis]({{ '/proofs/part-1/euclidean-complexity/' | relative_url }}) and [Lamé’s theorem proof]({{ '/proofs/part-1/lame-theorem/' | relative_url }}).

## Why look beyond division?

While `a % b` looks simple in a high-level language, CPU execution tells a different story:

- **Single-word level:** bitwise operations such as `AND`, `XOR`, and `SHR` can be very cheap, while hardware division has much higher latency and may stall dependent execution.
- **Multi-precision level:** 2048-bit and 4096-bit integers do not fit in one hardware word. Division must operate over arrays of words using a multi-precision algorithm.

Because division creates a bottleneck, the source says high-performance cryptographic libraries avoid division-heavy modulo loops for large integers. This motivates a GCD based on parity, shifts, and subtraction.

<!-- REVIEW: mathematical claim requires verification -->
Exact instruction latencies and blanket claims about how OpenSSL, BoringSSL, and Go avoid division are platform- and implementation-dependent; they require benchmarks and source-specific verification.

[Next: Binary GCD →]({{ '/part-1/04-binary-gcd/' | relative_url }})
