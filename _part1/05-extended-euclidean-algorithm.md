---
title: The Extended Euclidean Algorithm and Bézout’s Identity
nav_title: Extended Euclidean Algorithm
order: 5
description: Recover Bézout coefficients and modular inverses.
---

# The Extended Euclidean Algorithm and Bézout’s Identity

Computing $\gcd(a,N)=1$ confirms that an inverse exists, but does not calculate it. To recover the inverse, we track linear coefficients through the Euclidean algorithm.

## Bézout’s identity

### Question

Once $\gcd(a,b)$ confirms that two values are coprime, how can we recover the coefficients needed to compute a modular inverse?

### Intuition

Every common divisor of $a$ and $b$ also divides every integer linear combination of those values. Bézout’s identity shows that one such combination equals the greatest common divisor itself.

### Formal statement

For nonzero integers $a$ and $b$, there are integers $x$ and $y$ such that

$$
ax+by=\gcd(a,b).
$$

### Proof by the well-ordering principle

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

## Two indexing conventions

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

## Example: computing $15^{-1}\pmod{26}$

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

## C++ implementation

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

[Next: Matrix EEA →]({{ '/part-1/06-matrix-eea/' | relative_url }})
