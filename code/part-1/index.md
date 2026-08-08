---
title: Part 1 C++ Source
permalink: /code/part-1/
kind: Supplementary implementation material
---

# Part 1 C++ Source

The examples and algorithms use C++20 and the standard library only.

## Examples

- [`rot13`](examples/rot13/): letter rotation as modular addition
- [`modular-arithmetic`](examples/modular-arithmetic/): normalized residues

## Algorithms

- [`euclidean`](algorithms/euclidean/): classical remainder GCD
- [`binary-gcd`](algorithms/binary-gcd/): Stein’s algorithm using `std::countr_zero`
- [`extended-gcd`](algorithms/extended-gcd/): Bézout coefficients and modular inverse
- [`matrix-eea`](algorithms/matrix-eea/): exact transition-matrix accumulation

[`tests`](tests/) exercises all four algorithm implementations. These are deliberately small educational programs, not constant-time or hardened cryptographic code.
