---
title: Home
permalink: /
---

# RSA From First Principles

RSA can look like a wall of modular exponentiation, enormous primes, and forbidding notation. This project builds its machinery from elementary arithmetic, one proof and one executable implementation at a time.

## Part 1 — From Shifting Letters to Modular Arithmetic

[Read the complete Part 1 article →]({{ '/part-1/' | relative_url }})

The article moves through six sections: introduction, modular arithmetic, the Euclidean Algorithm, binary GCD, the Extended Euclidean Algorithm, and matrix EEA.

## Part 2: Constructing the Trapdoor

[Read the complete Part 2 article →]({{ '/part-2/' | relative_url }})

Part 2 turns the arithmetic tools from Part 1 into RSA. It develops the asymmetry requirement, modular exponentiation, multiplicative order, Euler's theorem, the factorization trapdoor, complete RSA correctness, the Chinese Remainder Theorem, and CRT-optimized private-key operations.

The first two articles form the published path from elementary modular arithmetic to RSA architecture. Their supporting material remains separate in the [proof archive]({{ '/proofs/' | relative_url }}), [Part 1 C++ source]({{ '/code/part-1/' | relative_url }}), [Part 1 references]({{ '/references/part-1/' | relative_url }}), and [Part 2 references]({{ '/references/part-2/' | relative_url }}).
