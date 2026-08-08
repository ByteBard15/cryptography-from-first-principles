---
title: Matrix Formulation and Accelerated EEA
nav_title: Matrix EEA
order: 6
description: Express Euclidean state transitions as 2-by-2 matrices and motivate Lehmer batching.
---

# Matrix Formulation and Accelerated EEA

Iterative EEA works well for machine integers. With multi-precision operands, repeated full-width division and coefficient updates become expensive. A matrix view lets us compose several scalar transitions before applying them to full-width values.

## One Euclidean step as a matrix

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

## Lehmer’s top-word idea

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

## Exact example: $26$ and $15$

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

## C++ implementation

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

## Part 1 summary

We started with a letter shift, turned it into arithmetic on residue classes, used gcd to characterize invertibility, recovered inverses with Bézout coefficients, and finally represented the entire recurrence as matrix multiplication.

From here, later parts can build on a stable foundation without mixing proofs, implementation details, or benchmark claims into the narrative.
