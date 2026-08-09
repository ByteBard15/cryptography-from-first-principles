---
title: "From Shifting Letters to Modular Arithmetic"
nav_title: "Part 1 — From Shifting Letters to Modular Arithmetic: The Number Theory Engine Behind RSA"
order: 1
permalink: /part-1/
description: Modular arithmetic, GCD algorithms, Bézout coefficients, modular inverses, and matrix EEA.
---

# From Shifting Letters to Modular Arithmetic

Every time you initiate an SSH session, send a cryptocurrency transaction, or connect over HTTPS, your system relies on mathematical concepts developed over two thousand years before the microchip.

To many developers, cryptography feels like a dense wall of modular exponentiation, massive prime numbers, and intimidating notation. In reality, modern digital security is the formal evolution of basic arithmetic. The algorithms protecting global financial pipelines were not invented overnight; they are centuries of elementary number theory compiled into high-performance code.

To understand why, we do not need to start with 4096-bit RSA keys or elliptic curves. We can begin with one of the simplest ciphers in history: **ROT13**.

*(For a broader historical and mathematical overview linking elementary number theory to cryptography, see Koblitz (1994, Chapter 1) and Hoffstein et al. (2008, Chapter 1).)*

### A cipher hiding an algebraic system

ROT13 (Rotate 13) shifts every letter 13 positions down the alphabet, wrapping around at the end:

$$
\text{A} \rightarrow \text{N}, \quad \text{B} \rightarrow \text{O}, \quad
\text{O} \rightarrow \text{B}, \quad \text{Z} \rightarrow \text{M}.
$$

Map each character to an integer index:

$$
\text{A}=0, \quad \text{B}=1, \quad \dots, \quad \text{Z}=25.
$$

Encryption becomes

$$
C \equiv P+13 \pmod{26},
$$

where $P$ is the plaintext index and $C$ is the ciphertext index. Arithmetic “wraps around” after 26 elements. Instead of the infinite integers $\mathbb{Z}$, we are working in the finite system $\mathbb{Z}/26\mathbb{Z}$.

ROT13 is self-invertible because applying it twice gives

$$
P+13+13 \equiv P+26 \equiv P \pmod{26}.
$$

The corresponding C++ keeps the modular addition visible:

```cpp
#include <string>

char rotate13(char character) {
    char base = 0;
    if (character >= 'A' && character <= 'Z') {
        base = 'A';
    } else if (character >= 'a' && character <= 'z') {
        base = 'a';
    } else {
        return character;
    }

    return static_cast<char>(base + (character - base + 13) % 26);
}

std::string rot13(std::string text) {
    for (char& character : text) {
        character = rotate13(character);
    }
    return text;
}
```

> **The flaw:** ROT13 offers no security. There are only 26 shifts; its fixed character mapping also preserves letter frequency.

The same code is available as the runnable [C++ ROT13 example]({{ '/code/part-1/examples/rot13/' | relative_url }}).

### From addition to inverses

A natural next step is replacing addition with multiplication:

$$
C \equiv kP \pmod N.
$$

Subtraction always undoes an additive shift. Reversing modular multiplication is possible only when a **multiplicative inverse** exists: a value $k^{-1}$ satisfying

$$
k k^{-1} \equiv 1 \pmod N.
$$

Such an inverse exists if and only if $k$ and $N$ share no common factor other than 1:

$$
\gcd(k,N)=1.
$$

That leaves us with the practical problem that drives Part 1: given $N$ and $k$, how do we efficiently test whether an inverse exists—and compute it without trying every integer up to $N$?

---

## Modular Arithmetic as an Algebraic System

To move from cryptographic heuristics to precise reasoning, we need to formalize how integers behave inside a bounded set. That playground is the quotient ring $\mathbb{Z}/N\mathbb{Z}$, pronounced “Z mod N.”

*(For the source article’s suggested formal treatment, see Shoup (2008, Chapter 2).)*

### Equivalence classes, not just `%`

A programmer might stop after computing a remainder:

```cpp
16 % 5 // 1
21 % 5 // 1
36 % 5 // 1
```

Number theory instead asks: **which numbers always produce the same remainder?** Let us work modulo 5:

| Integer | Remainder |
| ------: | --------: |
| 1 | 1 |
| 6 | 1 |
| 11 | 1 |
| 16 | 1 |
| 21 | 1 |
| 26 | 1 |

Although these integers are different, modulo 5 they behave exactly the same. Adding or subtracting a multiple of 5 never changes the remainder. We group them into one equivalence class:

$$
[1]=\{\ldots,-9,-4,1,6,11,16,21,26,\ldots\}.
$$

Likewise,

$$
\begin{aligned}
[0]&=\{\ldots,-10,-5,0,5,10,15,20,\ldots\},\\
[2]&=\{\ldots,-8,-3,2,7,12,17,22,\ldots\},\\
[3]&=\{\ldots,-7,-2,3,8,13,18,23,\ldots\},\\
[4]&=\{\ldots,-6,-1,4,9,14,19,24,\ldots\}.
\end{aligned}
$$

Together, these five sets partition $\mathbb{Z}$. Every integer belongs to exactly one of them, and no integer belongs to two different classes. This is precisely the grouping produced by an equivalence relation.

Two integers $a$ and $b$ are **congruent modulo $N$** precisely when their difference is a multiple of $N$:

$$
a \equiv b \pmod N
\iff
N \mid (a-b).
$$

For example,

$$
16 \equiv 1 \pmod 5
$$

because $16-1=15=3\cdot5$. Similarly, $21\equiv1\pmod5$ because $21-1=20=4\cdot5$. Computations in modular arithmetic act on these classes, not on isolated representatives.

> **Developer pitfall: negative remainders.** C++ uses truncated integer division, so `-3 % 26` is `-3`, not `23`. To select the representative in $[0,N-1]$, normalize the result:

```cpp
#include <cstdint>
#include <stdexcept>

std::int64_t math_mod(std::int64_t value, std::int64_t modulus) {
    if (modulus <= 0) {
        throw std::invalid_argument("modulus must be positive");
    }
    const std::int64_t remainder = value % modulus;
    return remainder < 0 ? remainder + modulus : remainder;
}
```

The same implementation is available as the runnable [modular-arithmetic example]({{ '/code/part-1/examples/modular-arithmetic/' | relative_url }}).

### Operations survive reduction

If $a\equiv b\pmod N$ and $c\equiv d\pmod N$, then

$$
\begin{aligned}
a\pm c &\equiv b\pm d \pmod N,\\
ac &\equiv bd \pmod N,\\
a^k &\equiv b^k \pmod N \quad (k\ge 0).
\end{aligned}
$$

This is why a computation can reduce intermediate results:

$$
(ab)\bmod N
=\bigl((a\bmod N)(b\bmod N)\bigr)\bmod N.
$$

### The quotient ring $\mathbb{Z}/N\mathbb{Z}$

Using the conventional representatives,

$$
\mathbb{Z}/N\mathbb{Z}=\{0,1,2,\dots,N-1\}.
$$

Modular addition is closed, associative, and commutative; it has identity $0$ and additive inverses. Modular multiplication is closed and associative, distributes over addition, and has identity $1$. Together these operations make a commutative ring.

Division needs more care. In $\mathbb{Z}/12\mathbb{Z}$,

$$
2\cdot4=8\equiv20=2\cdot10\pmod{12},
$$

but cancelling 2 would incorrectly say $4\equiv10\pmod{12}$. The element 2 is a zero divisor because $2\cdot6\equiv0\pmod{12}$.

### Units: the elements we may divide by

An element $a\in\mathbb{Z}/N\mathbb{Z}$ has a unique inverse if and only if

$$
\gcd(a,N)=1.
$$

The invertible elements, called **units**, form the multiplicative group

$$
(\mathbb{Z}/N\mathbb{Z})^\times
=\{a\in\mathbb{Z}/N\mathbb{Z}\mid\gcd(a,N)=1\}.
$$

If $N=p$ is prime, every nonzero class is a unit and the ring is the field $\mathbb{F}_p$. If $N=pq$ is composite, only elements coprime to both factors are units.

<!-- REVIEW: mathematical claim requires verification -->
The source article further says that finding non-units modulo an RSA modulus is equivalent to factoring $N$. The precise computational reduction and its conditions should be stated before publication.

To decide membership in this group and to compute inverses, we need Euclid’s algorithm.

---

## The Euclidean Algorithm

Naive prime factorization decomposes numbers into prime factors. For example, $24=2^3\cdot3$ and $36=2^2\cdot3^2$, so $\gcd(24,36)=2^2\cdot3=12$. However, integer factorization takes exponential time $O(2^b)$ relative to bit length $b$. Public-key security explicitly relies on factorization being intractable.

<!-- REVIEW: mathematical claim requires verification -->
The precise $O(2^b)$ factorization claim above requires verification and a stated computational model.

Euclid’s algorithm bypasses factorization entirely, solving GCD in a logarithmic number of division steps.

### The invariant

Euclid’s algorithm rests on a simple observation: if $d$ divides both $a$ and $b$, then it divides their difference. In fact,

$$
\gcd(a,b)=\gcd(a-b,b).
$$

#### Proof that $\gcd(a,b)=\gcd(a-b,b)$

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

#### C++ implementation

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

### Lamé’s theorem and complexity

Consecutive Fibonacci inputs give the worst case because the nonterminal quotients are 1. If $d$ is the number of decimal digits in the smaller positive input, Lamé’s theorem gives

$$
k\le5d,
\qquad
d=\left\lfloor\log_{10}(\min(a,b))\right\rfloor+1.
$$

<!-- REVIEW: corrected the source article's imprecise shorthand k <= 5 log10(min(a,b)); Lamé's theorem is stated in terms of the integer decimal-digit count. -->

It distinguishes the number of division steps, logarithmic in the input value, from the $O(n^2)$ bit-operation cost stated for classical division GCD on $n$-bit integers.

The supplementary research archive now contains the complete [Euclidean bit-complexity analysis]({{ '/proofs/part-1/euclidean-complexity/' | relative_url }}) and [Lamé’s theorem proof]({{ '/proofs/part-1/lame-theorem/' | relative_url }}).

### Why look beyond division?

While `a % b` looks simple in a high-level language, CPU execution tells a different story:

- **Single-word level:** bitwise operations such as `AND`, `XOR`, and `SHR` can be very cheap, while hardware division has much higher latency and may stall dependent execution.
- **Multi-precision level:** 2048-bit and 4096-bit integers do not fit in one hardware word. Division must operate over arrays of words using a multi-precision algorithm.

Because division creates a bottleneck, the source says high-performance cryptographic libraries avoid division-heavy modulo loops for large integers. This motivates a GCD based on parity, shifts, and subtraction.

<!-- REVIEW: mathematical claim requires verification -->
Exact instruction latencies and blanket claims about how OpenSSL, BoringSSL, and Go avoid division are platform- and implementation-dependent; they require benchmarks and source-specific verification.

---

## Binary GCD (Stein’s Algorithm)

In 1967, Josef Stein introduced **Binary GCD**, replacing general division with three machine-friendly primitives:

1. **Parity checks** (`a & 1 == 0`) test whether an integer is even or odd.
2. **Bit shifts** (`a >> 1`) divide an even integer by 2.
3. **Subtractions** (`a - b`) reduce odd numbers.

*(The source points to Menezes et al. (1996), HAC Algorithm 14.54, for pseudocode and to Brent (2000) for average-case analysis.)*

### Four identities

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

### Trace: $\gcd(24,15)$

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

### C++ implementation

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

### Trade-offs

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

---

## The Extended Euclidean Algorithm and Bézout’s Identity

Computing $\gcd(a,N)=1$ confirms that an inverse exists, but does not calculate it. To recover the inverse, we track linear coefficients through the Euclidean algorithm.

### Bézout’s identity

#### Question

Once $\gcd(a,b)$ confirms that two values are coprime, how can we recover the coefficients needed to compute a modular inverse?

#### Intuition

Every common divisor of $a$ and $b$ also divides every integer linear combination of those values. Bézout’s identity shows that one such combination equals the greatest common divisor itself.

#### Formal statement

For nonzero integers $a$ and $b$, there are integers $x$ and $y$ such that

$$
ax+by=\gcd(a,b).
$$

#### Proof by the well-ordering principle

Construct the set of all positive linear combinations of $a$ and $b$:

$$
S=\{as+bt\mid s,t\in\mathbb{Z},\ as+bt>0\}.
$$

1. **$S$ is nonempty.** Since $a\ne0$, either $a$ or $-a$ is positive, and either value is an integer linear combination of $a$ and $b$.
2. **$S$ has a minimum.** By the well-ordering principle, every nonempty set of positive integers has a smallest element. Write it as

   $$
   d=ax_0+by_0.
   $$

3. **$d$ divides $a$.** Apply the division algorithm:

   $$
   a=qd+r,\qquad0\le r<d.
   $$

   Rearranging gives

   $$
   r=a-qd=a(1-qx_0)+b(-qy_0).
   $$

   Thus $r$ is another integer linear combination of $a$ and $b$. If $r>0$, then $r\in S$, contradicting the minimality of $d$. Therefore $r=0$ and $d\mid a$. The identical argument gives $d\mid b$, so $d$ is a common divisor.
4. **$d$ is the greatest common divisor.** If $c$ is any common divisor of $a$ and $b$, then $c$ divides every linear combination of them. In particular,

   $$
   c\mid(ax_0+by_0),
   $$

   so $c\mid d$. Every common divisor divides $d$, and $d$ is itself a common divisor. Hence

   $$
   d=\gcd(a,b)=ax_0+by_0.
   $$

This is the complete proof supplied by the source article. A standalone version is available as [Bézout’s Identity]({{ '/proofs/part-1/bezout-identity/' | relative_url }}).

If $\gcd(a,N)=1$, Bézout gives

$$
ax+Ny=1.
$$

Reducing modulo $N$ removes $Ny$:

$$
ax\equiv1\pmod N.
$$

Thus $x\equiv a^{-1}\pmod N$. A negative coefficient can be normalized into $[0,N-1]$ with

$$
a^{-1}=((x\bmod N)+N)\bmod N.
$$

### Two indexing conventions

Mathematical presentations often label each newly generated remainder:

$$
q_i=\left\lfloor\frac{r_{i-2}}{r_{i-1}}\right\rfloor,
\qquad
r_i=r_{i-2}-q_i r_{i-1}
\quad(i\ge2).
$$

Code and matrix presentations often label the current state transition:

$$
q_i=\left\lfloor\frac{r_{i-1}}{r_i}\right\rfloor,
\qquad
r_{i+1}=r_{i-1}-q_i r_i
\quad(i\ge1).
$$

They describe the same sequence. State-oriented notation is useful in software because $q_i$ directly parameterizes one loop transition.

### Example: computing $15^{-1}\pmod{26}$

Euclid’s divisions are

$$
\begin{aligned}
26&=1(15)+11,\\
15&=1(11)+4,\\
11&=2(4)+3,\\
4&=1(3)+1.
\end{aligned}
$$

Substitute backwards:

$$
\begin{aligned}
1&=4-3\\
 &=3(4)-11\\
 &=3(15)-4(11)\\
 &=7(15)-4(26).
\end{aligned}
$$

Therefore $7\cdot15\equiv1\pmod{26}$ and

$$
15^{-1}\equiv7\pmod{26}.
$$

| $i$ | $r_i$ | $q_i$ | coefficient of 26 | coefficient of 15 |
| ---: | ---: | ---: | ---: | ---: |
| 0 | 26 | — | 1 | 0 |
| 1 | 15 | — | 0 | 1 |
| 2 | 11 | 1 | 1 | -1 |
| 3 | 4 | 1 | -1 | 2 |
| 4 | 3 | 2 | 3 | -5 |
| 5 | 1 | 1 | -4 | 7 |

### C++ implementation

```cpp
#include <cstdint>
#include <optional>

struct ExtendedGcdResult {
    std::int64_t gcd;
    std::int64_t x;
    std::int64_t y;
};

// Solves ax + by = gcd(a, b).
constexpr ExtendedGcdResult extended_gcd(std::int64_t a,
                                         std::int64_t b) {
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

// Returns a^-1 mod modulus, or no value when the inverse does not exist.
constexpr std::optional<std::int64_t> modular_inverse(
    std::int64_t a,
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
```

The same code and its tests remain in the [extended-GCD source directory]({{ '/code/part-1/algorithms/extended-gcd/' | relative_url }}).

The supplementary [EEA bit-complexity proof]({{ '/proofs/part-1/eea-complexity/' | relative_url }}) provides the complete amortized $O(n^2)$ analysis.

---

## Matrix Formulation and Accelerated EEA

Iterative EEA works well for machine integers. With multi-precision operands, repeated full-width division and coefficient updates become expensive. A matrix view lets us compose several scalar transitions before applying them to full-width values.

### One Euclidean step as a matrix

With the state-oriented indexing convention,

$$
\begin{pmatrix}r_i\\r_{i+1}\end{pmatrix}
=
\begin{pmatrix}0&1\\1&-q_i\end{pmatrix}
\begin{pmatrix}r_{i-1}\\r_i\end{pmatrix}.
$$

Write

$$
T_i=\begin{pmatrix}0&1\\1&-q_i\end{pmatrix}.
$$

Associativity lets us compose $k$ steps:

$$
\begin{pmatrix}r_k\\r_{k+1}\end{pmatrix}
=M\begin{pmatrix}r_0\\r_1\end{pmatrix},
\qquad
M=T_kT_{k-1}\cdots T_1.
$$

### Lehmer’s top-word idea

The source describes Lehmer’s technique in four moves:

1. Extract the leading machine words of large integers $A$ and $B$.
2. Estimate several Euclidean quotients using those word-sized approximations.
3. Accumulate their $2\times2$ transition matrices in registers.
4. Apply the cumulative matrix once to the full-width pair:

   $$
   \begin{aligned}
   A_{\text{new}}&=m_{11}A+m_{12}B,\\
   B_{\text{new}}&=m_{21}A+m_{22}B.
   \end{aligned}
   $$

<!-- REVIEW: mathematical claim requires verification -->
The source says lower digits “rarely” affect the quotient and suggests 30–60 register-only EEA steps. A correct Lehmer implementation needs quotient-stability checks; these empirical counts are not presented as guarantees.

For even larger operands, the source points onward to half-GCD and states a complexity of $O(M(n)\log n)$, where $M(n)$ is the cost of $n$-bit multiplication.

### Exact example: $26$ and $15$

The quotient sequence is $1,1,2,1$. Under the convention $M=T_4T_3T_2T_1$, multiplying its transition matrices gives

$$
M=
\begin{pmatrix}0&1\\1&-1\end{pmatrix}
\begin{pmatrix}0&1\\1&-2\end{pmatrix}
\begin{pmatrix}0&1\\1&-1\end{pmatrix}
\begin{pmatrix}0&1\\1&-1\end{pmatrix}
$$

and therefore

$$
M=\begin{pmatrix}3&-5\\-4&7\end{pmatrix}.
$$

Applying it to the initial state confirms

$$
\begin{pmatrix}3&-5\\-4&7\end{pmatrix}
\begin{pmatrix}26\\15\end{pmatrix}
=
\begin{pmatrix}3\\1\end{pmatrix}.
$$

The bottom row is Bézout’s identity,

$$
-4(26)+7(15)=1,
$$

so it again reveals $15^{-1}\equiv7\pmod{26}$.

### C++ implementation

This fixed-width implementation accumulates the exact transition matrix described above:

```cpp
#include <cstdint>
#include <stdexcept>

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

constexpr MatrixEeaResult matrix_eea(std::int64_t a,
                                     std::int64_t b) {
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
```

The same code and its tests are available in the [matrix EEA source directory]({{ '/code/part-1/algorithms/matrix-eea/' | relative_url }}). It demonstrates exact transition accumulation for fixed-width educational inputs and is intentionally not presented as a production Lehmer or half-GCD implementation.

The supplementary [matrix EEA correctness proof]({{ '/proofs/part-1/matrix-eea-correctness/' | relative_url }}) proves the row-linear-combination and determinant invariants and states the quotient-stability condition needed by Lehmer batching.

---

## Conclusion & Part 1 Summary

In this first installment, we built the foundational mathematical engine that powers modern public-key cryptography:

1. **Modular arithmetic as an algebraic system:** We moved from basic letter shifts to the quotient ring $\mathbb{Z}/N\mathbb{Z}$ and its multiplicative group of units $(\mathbb{Z}/N\mathbb{Z})^\times$.
2. **Efficient GCD computation:** We proved why $\gcd(a,b)=\gcd(a-b,b)$ and showed how **Binary GCD (Stein’s Algorithm)** replaces general division with parity checks, shifts, subtraction, and trailing-zero counts.
3. **Inverses and acceleration:** We proved **Bézout’s identity** using the well-ordering principle, resolved indexing conventions, calculated modular inverses with EEA, and showed how $2\times2$ transition matrices enable Lehmer-style batching for multi-precision arithmetic.

---

### What’s next in Part 2?

Now that we have the computational tools to evaluate modular inverses efficiently, we are ready to explore the trapdoor functions that protect global digital communication.

In **Part 2**, we will cover:

- **Euler’s totient function $\phi(N)$** and **Fermat’s little theorem**.
- Constructing the core RSA key pair $(e,d,N)$ from algebraic principles.
- Educational C++ implementations of RSA key generation, encryption, and digital signatures, developed alongside the relevant proofs and tests.
