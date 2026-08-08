---
title: From Shifting Letters to Modular Arithmetic
nav_title: Introduction
order: 1
description: Begin with ROT13 and discover the inverse problem behind modern cryptography.
---

# From Shifting Letters to Modular Arithmetic

Every time you initiate an SSH session, send a cryptocurrency transaction, or connect over HTTPS, your system relies on mathematical concepts developed over two thousand years before the microchip.

To many developers, cryptography feels like a dense wall of modular exponentiation, massive prime numbers, and intimidating notation. In reality, modern digital security is the formal evolution of basic arithmetic. The algorithms protecting global financial pipelines were not invented overnight; they are centuries of elementary number theory compiled into high-performance code.

To understand why, we do not need to start with 4096-bit RSA keys or elliptic curves. We can begin with one of the simplest ciphers in history: **ROT13**.

*(For a broader historical and mathematical overview linking elementary number theory to cryptography, see Koblitz (1994, Chapter 1) and Hoffstein et al. (2008, Chapter 1).)*

## A cipher hiding an algebraic system

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

## From addition to inverses

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

[Next: Modular Arithmetic →]({{ '/part-1/02-modular-arithmetic/' | relative_url }})
