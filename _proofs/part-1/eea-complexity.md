---
title: Extended Euclidean Algorithm Bit Complexity
kind: Supplementary proof
---

# Extended Euclidean Algorithm: Bit Complexity Analysis

The Extended Euclidean Algorithm (EEA) computes not only the greatest common divisor $\gcd(a,b)$, but also the Bézout coefficients $x$ and $y$ satisfying:

$$
a x + b y = \gcd(a, b)
$$

While the step count of Euclid's algorithm is $O(n)$ by Lamé's Theorem (where $n$ is the bit length of the inputs), evaluating the **bit complexity** requires accounting for multi-precision division and multi-precision multiplication at every step.

---

## 1. Computational Model & Input Assumptions

1. **Input Size:** Two positive integers $a$ and $b$ with $a > b > 0$, where $n = \operatorname{len}(a) = \lceil \log_2(a + 1) \rceil$. Both numbers fit in $n$ bits.
2. **Classical Arithmetic Model:**
* **Division:** Dividing a $k_1$-bit integer by a $k_2$-bit integer (producing a quotient $q$ of length $k_1 - k_2 + 1$ bits) using standard long division requires $O(\operatorname{len}(q) \cdot k_2)$ bit operations.
* **Multiplication:** Multiplying a $k_1$-bit integer by a $k_2$-bit integer takes $O(k_1 \cdot k_2)$ bit operations under naive $O(n^2)$ multiplication.



---

## 2. Algorithm Sequence & Recurrences

Executing EEA produces remainders, quotients, and coefficient vectors satisfying

$$
\begin{aligned}
r_0&=a, & r_1&=b,\\
r_{i-1}&=q_ir_i+r_{i+1}, & 0&\le r_{i+1}<r_i,\\
x_0&=1, & x_1&=0, & x_{i+1}&=x_{i-1}-q_ix_i,\\
y_0&=0, & y_1&=1, & y_{i+1}&=y_{i-1}-q_iy_i.
\end{aligned}
$$

The algorithm terminates after transition $k$ when $r_{k+1}=0$, giving $r_k=\gcd(a,b)$.

---

## 3. Proof of $O(n^2)$ Bit Complexity

The work performed by the Extended Euclidean Algorithm breaks into two distinct components per step $i$:

1. **The division step:** computing $q_i=\lfloor r_{i-1}/r_i\rfloor$ and $r_{i+1}=r_{i-1}\bmod r_i$.
2. **The coefficient update:** computing $x_{i+1}=x_{i-1}-q_ix_i$ and $y_{i+1}=y_{i-1}-q_iy_i$.

---

### Part A: Division Cost Analysis

At step $i$, dividing $r_{i-1}$ by $r_i$ yields quotient $q_i$, whose length is bounded by the drop in operand length plus one.

The bit operations required for this single long division are bounded by:

$$
\operatorname{Cost}_{\mathrm{div}}(i)
=O\bigl(\operatorname{len}(q_i)\operatorname{len}(r_i)\bigr).
$$

Since every $r_i<2^n$, use the global bound $\operatorname{len}(r_i)\le n$:

$$
\text{Cost}_{\text{div}}(i) \le C \cdot \operatorname{len}(q_i) \cdot n
$$

Summing over all $k$ steps of the algorithm gives the total division cost:

$$
\text{Total Div Cost} = \sum_{i=1}^k \text{Cost}_{\text{div}}(i) \le C \cdot n \sum_{i=1}^k \operatorname{len}(q_i)
$$

#### Bounding the Sum of Quotient Bit Lengths

From the sequence of division steps, we observe:

$$
a = r_0 > q_1 r_1 > q_1 q_2 r_2 > \dots > \left( \prod_{i=1}^k q_i \right) r_k \ge \prod_{i=1}^k q_i
$$

Taking the base-2 logarithm of both sides:

$$
\sum_{i=1}^k \log_2(q_i) < \log_2(a) \le n
$$

Since $\operatorname{len}(q_i) \le \log_2(q_i) + 1$, summing across all $k$ steps yields:

$$
\sum_{i=1}^k \operatorname{len}(q_i) \le \sum_{i=1}^k \big(\log_2(q_i) + 1\big) < n + k
$$

By Lamé’s theorem, the number of steps is $k=O(n)$. Therefore:

$$
\sum_{i=1}^k \operatorname{len}(q_i) = O(n)
$$

Substituting this back into our sum gives:

$$
\text{Total Div Cost} = O(n) \cdot O(n) = O(n^2)
$$

---

### Part B: Coefficient Update Cost Analysis

At step $i$, updating $x_{i+1}=x_{i-1}-q_ix_i$ requires multiplying $q_i$ by $x_i$; the update for $y_{i+1}$ is analogous.

#### Size Bound on Bézout Coefficients

By induction on the linear combination $a x_i + b y_i = r_i$, the absolute values of the intermediate coefficients $x_i$ and $y_i$ are strictly bounded by the inputs:

$$
\lvert x_i\rvert\le\frac{b}{\gcd(a,b)}<2^n,
\qquad
\lvert y_i\rvert\le\frac{a}{\gcd(a,b)}<2^n.
$$

Thus the coefficient lengths never exceed $O(n)$ bits.

#### Summing Update Costs

The cost to multiply $q_i$ by either current coefficient is $O(\operatorname{len}(q_i)n)$. Summing across all $k$ steps:

$$
\text{Total Update Cost} = \sum_{i=1}^k O\big(\operatorname{len}(q_i) \cdot n\big) = O\left( n \sum_{i=1}^k \operatorname{len}(q_i) \right) = O(n^2)
$$

---

## 4. Total Bit Complexity Statement

Combining division costs and coefficient update costs:

$$
T_{\text{EEA}}(n) = \text{Total Div Cost} + \text{Total Update Cost} = O(n^2) + O(n^2) = O(n^2)
$$

The Extended Euclidean Algorithm on two $n$-bit integers executes in **$O(n^2)$ bit operations** using classical $O(n^2)$ integer arithmetic.

---

## 5. Shoup citation locator

The source article refers to Victor Shoup’s *A Computational Introduction to Number Theory and Algebra* and names Theorem 4.5.

### Theorem Locator Verification

In the available editions, Chapter 4 covers Euclid’s algorithm and Section 4.2 covers the extended algorithm. Theorem 4.5 states the coefficient recurrences, Bézout invariant, determinant identity, and coefficient-size bounds used by the complexity analysis; the surrounding discussion supplies the running-time analysis. The article’s wording that Theorem 4.5 itself is solely a complexity theorem is therefore too narrow.

The telescoping argument above establishes $O(\operatorname{len}(a)\operatorname{len}(b))$ for the classical arithmetic model, and hence $O(n^2)$ for two $n$-bit inputs.

---

## 6. Summary Comparison: Standard vs. Fast EEA

| Algorithm Variant | Primary Multiplication Method | Asymptotic Bit Complexity |
| --- | --- | --- |
| **Standard Extended Euclid** | Classical $O(n^2)$ | **$O(n^2)$** |
| **Half-GCD / Fast EEA** | Multiplication cost $M(n)$ | **$O(M(n)\log n)$** |

> **Practical takeaway:** Concrete crossover points depend on the implementation, multiplication algorithms, target CPU, and operand distribution. They should be established by measurement rather than a universal bit-length threshold.

---

## References

* Shoup (2008), Chapter 4 and Theorem 4.5, as cited by the source article.
* Menezes et al. (1996), Algorithm 14.61, as cited by the source article.
* Knuth (1998), Volume 2, Section 4.5.2, as cited by the source article.
