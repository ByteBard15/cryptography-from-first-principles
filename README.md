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

## The RSA series

The project is structured as a three-part progression from elementary number theory to RSA’s construction, deployment, and failure modes. The aim is to balance deep mathematical explanations with code-backed implementations and the security consequences of getting the details wrong.

### Part 1 — The Number Theory Engine

**Article:** *Demystifying RSA Math: From Modular Arithmetic to Accelerated Extended Euclidean Algorithms*

Part 1 builds the arithmetic engine RSA depends on. It begins with a familiar example of cyclic arithmetic and develops the tools needed to test coprimality, recover modular inverses, and perform Euclidean reductions efficiently.

The complete narrative is published as [Part 1 — The Number Theory Engine](_part1/part-1.md). Its sections cover:

1. **Why ancient number theory powers modern cryptography** — the motivation for building RSA’s mathematics from first principles.
2. **Modular arithmetic foundations** — congruence, residue classes, the quotient ring $\mathbb{Z}/n\mathbb{Z}$, and multiplicative units.
3. **The Euclidean Algorithm and Binary GCD** — computing greatest common divisors without factoring and comparing division-based and shift-based reductions.
4. **The Extended Euclidean Algorithm and Bézout’s identity** — recovering coefficients satisfying $ax+by=\gcd(a,b)$ and using them to compute modular inverses.
5. **Accelerating EEA** — expressing Euclidean transitions as $2\times2$ matrices and motivating Lehmer and half-GCD techniques for large operands.
6. **C++ implementations** — executable versions of the algorithms, supported by tests, complexity analyses, and focused proofs.

### Part 2 — The Inversion Magic and RSA Architecture

**Article:** *Under the Hood of Asymmetric Cryptography: Modular Inverses, Euler’s Theorem, and RSA*

Part 2 will connect the arithmetic engine to RSA’s trapdoor structure:

1. **The nature of the modular inverse** — why EEA makes inversion efficient when the required coprimality conditions hold and how hidden factorization shapes the surrounding security assumptions.
2. **Euler’s totient theorem and Fermat’s little theorem** — the bridge to the congruence $a^{\phi(n)}\equiv1\pmod n$ under the appropriate hypotheses.
3. **Building the trapdoor** — constructing the public key $(e,n)$ and private key $(d,n)$ from primes $p$ and $q$.
4. **Implementation walkthrough** — educational C++ implementations of RSA key generation, encryption, and decryption, developed alongside the relevant proofs and tests.

### Part 3 — Prime Generation, Applications, and Vulnerabilities

**Article:** *From Primes to Exploits: How RSA Works in Practice and Where It Fails*

Part 3 will take the mathematical construction into real systems and adversarial settings:

1. **Generating secure primes** — probabilistic primality testing, including Miller–Rabin, and the requirements placed on randomness and candidate selection.
2. **Real-world applications** — RSA signatures, certificate authentication, key encapsulation, and RSA’s relationship with symmetric cryptography and TLS deployments.
3. **Low-public-exponent attacks** — how unsafe parameter choices or protocol constructions can make small $e$ values exploitable.
4. **Poor prime generation** — reused primes, insufficient entropy, and weak random-number generators.
5. **Malleability and padding** — why textbook RSA is unsafe and how schemes such as OAEP protect encryption against structural attacks.
6. **From theory to defensive engineering** — connecting the number theory of Parts 1 and 2 to implementation review, protocol design, and secure operational practice.

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

The [`experiments/part-1/`](experiments/part-1/) material separates empirical performance questions from mathematical complexity claims. It provides a baseline for comparing word-sized Euclidean and binary GCD implementations and documents the information needed for reproducible measurements. Experiments remain part of the repository without occupying the primary site navigation.

### References

The project’s [references](references/part-1.md) collect the mathematical, algorithmic, and cryptographic works supporting the current article.

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
