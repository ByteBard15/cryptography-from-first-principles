# Cryptography From First Principles

**Cryptography From First Principles** is a technical research and educational project that develops the mathematics behind modern cryptography from elementary ideas, executable algorithms, and carefully separated proofs.

Modern cryptographic systems are often introduced at the point where the notation is already dense: modular exponentiation, large primes, finite fields, key pairs, signatures, and security assumptions appear all at once. This project takes the opposite route. It begins with familiar operations, asks why they work, formalizes the underlying mathematics, and follows that mathematics until recognizable cryptographic machinery emerges.

The goal is not merely to present formulas or working code. Each topic is approached through several connected questions:

- What problem is the construction solving?
- What is the intuitive idea behind it?
- What mathematical statement makes it valid?
- What does the algorithm look like when implemented?
- What changes when the operands become cryptographically large?
- Which claims are formal results, and which require measurement or further verification?

The result is intended to remain approachable to developers without sacrificing the precision required by number theory and cryptographic engineering.

## The series

The project is organized as a multi-part progression. Each part introduces the mathematical and computational tools needed by the parts that follow.

### Part 1 — The Number Theory Engine

Part 1 builds the arithmetic foundation used throughout public-key cryptography. It begins with ROT13 as a small example of wraparound arithmetic and develops that observation into quotient rings, greatest-common-divisor algorithms, Bézout coefficients, modular inverses, and matrix-based Euclidean reductions.

The complete narrative is published as a single document: [Part 1 — The Number Theory Engine](_part1/part-1.md). Its sections cover:

1. **Introduction and shifting letters** — how a simple substitution cipher exposes modular structure and the inverse problem.
2. **Modular arithmetic** — congruence, residue classes, quotient rings, zero divisors, and multiplicative units.
3. **The Euclidean Algorithm** — computing greatest common divisors without factoring and understanding the division invariant.
4. **Binary GCD** — Stein’s shift-and-subtract formulation and its relationship to machine arithmetic.
5. **The Extended Euclidean Algorithm** — Bézout coefficients, modular inverses, indexing conventions, and iterative state tracking.
6. **Matrix EEA** — representing Euclidean transitions with $2\times2$ matrices and motivating Lehmer and half-GCD acceleration.

### Part 2 — From Inverses to Trapdoor Arithmetic

The planned second part builds on the Part 1 engine to study Euler’s totient function, Fermat’s little theorem, and the algebraic construction of RSA keys, encryption, and signatures. It will be added only after its articles, proofs, implementations, experiments, and references have been reviewed to the same standard as Part 1.

### Later parts

The repository is designed to accommodate further parts without restructuring the existing material. Their scope will be documented as the research develops; no unpublished Part 2 or Part 3 article content is currently represented as complete.

## What the repository contains

The repository separates the main reading path from the material used to support, test, and audit it.

### Explanatory articles

The [`_part1/part-1.md`](_part1/part-1.md) collection document contains the complete Part 1 narrative. Its sections preserve the conversational style of the original series: developer-oriented intuition, historical context, small examples, “why should I care?” explanations, and transitions from visual reasoning to formal mathematics.

### Mathematical proofs

The [`_proofs/part-1/`](_proofs/part-1/) collection contains longer formal arguments and complexity analyses. This keeps the main articles readable while allowing important claims to be examined independently. The current proof material includes:

- Euclidean Algorithm time and bit complexity
- Lamé’s theorem
- Binary GCD bit complexity
- Bézout’s identity
- Extended Euclidean Algorithm bit complexity
- Matrix EEA correctness and Lehmer quotient stability

The [proof archive](proofs/index.md) records which supplementary proofs are currently part of the published research path.

### C++ implementations

The [`code/part-1/`](code/part-1/) tree contains small C++20 implementations corresponding to the article:

- ROT13 and normalized modular arithmetic examples
- Classical Euclidean GCD
- Binary GCD using trailing-zero operations
- Extended GCD and modular inverse calculation
- Exact matrix EEA transition accumulation
- Basic tests that compare the algorithms and exercise edge cases

The code is intentionally dependency-free and educational. It exists to make the mathematics executable and inspectable; it is not presented as constant-time, side-channel-resistant, or production-ready cryptographic software.

### Experiments and benchmarks

The [`experiments/part-1/`](experiments/part-1/) material separates empirical performance questions from mathematical complexity claims. It provides a baseline for comparing word-sized Euclidean and binary GCD implementations and documents the information needed for reproducible measurements.

### References

The [Part 1 reference record](references/part-1.md) preserves the sources named by the article. References are kept separate from the narrative so that bibliographic details and unverified locators can be reviewed without obscuring the explanation.

### Research site

The Markdown collections, Jekyll configuration, navigation, and KaTeX layout turn the repository into a structured GitHub Pages research site. Articles, proofs, code, experiments, and references remain distinct parts of one project rather than being compressed into a chronological blog format.

## Guiding principles

- Begin with intuition, then state the mathematics precisely.
- Keep mathematical notation canonical and portable.
- Preserve the distinction between proofs, implementations, and empirical claims.
- Prefer small, readable implementations over unnecessary frameworks.
- Do not present benchmark observations as universal complexity results.
- Mark uncertain claims and incomplete references instead of silently inventing certainty.
- Let later parts build on earlier results without rewriting the project structure.

At its core, the project is an attempt to make cryptography feel less like a collection of magical formulas and more like what it is: a sequence of understandable mathematical ideas, transformed into algorithms and assembled into security systems.
