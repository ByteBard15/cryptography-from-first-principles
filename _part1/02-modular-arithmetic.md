---
title: Modular Arithmetic as an Algebraic System
nav_title: Modular Arithmetic
order: 2
description: Congruence, equivalence classes, quotient rings, and modular units.
---

# Modular Arithmetic as an Algebraic System

To move from cryptographic heuristics to precise reasoning, we need to formalize how integers behave inside a bounded set. That playground is the quotient ring $\mathbb{Z}/N\mathbb{Z}$, pronounced “Z mod N.”

*(For the source article’s suggested formal treatment, see Shoup (2008, Chapter 2).)*

## Equivalence classes, not just `%`

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

## Operations survive reduction

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

## The quotient ring $\mathbb{Z}/N\mathbb{Z}$

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

## Units: the elements we may divide by

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

[Next: Euclidean Algorithm →]({{ '/part-1/03-euclidean-algorithm/' | relative_url }})
