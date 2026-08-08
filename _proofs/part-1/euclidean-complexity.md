---
title: Euclidean Algorithm Time and Bit Complexity
kind: Supplementary proof
---

# Classical Euclidean Algorithm: Time & Bit Complexity

The classical Euclidean algorithm computes the greatest common divisor $\gcd(a,b)$ of two positive integers $a$ and $b$ using a sequence of long divisions:

$$
r_{i-1}=q_ir_i+r_{i+1}
\quad\text{where}\quad
0\le r_{i+1}<r_i,
$$

where $r_0 = a$ and $r_1 = b$.

While Lamé's Theorem proves that the algorithm executes in a **logarithmic number of division steps** $O(n)$, determining the total **bit complexity** requires analyzing the multi-precision division cost at each step. Naively multiplying the step count by the maximum $O(n^2)$ division cost yields an overestimation of $O(n^3)$.

This section proves that when accounting for quotient length amortization, the classical Euclidean algorithm runs in **$O(n^2)$ bit operations**.

---

## 1. Computational Model & Input Parameters

1. **Input Size:** Two positive integers $a$ and $b$ with $a > b > 0$, where $n = \operatorname{len}(a) = \lceil \log_2(a + 1) \rceil$. Both numbers fit in $n$ bits.
2. **Division Cost Model:** Dividing a $k_1$-bit integer $u$ by a $k_2$-bit integer $v$ to produce quotient $q$ (length $\operatorname{len}(q) \approx k_1 - k_2 + 1$) and remainder $r$ using standard long division requires:

$$
\text{Cost}_{\text{div}} = O\big(\operatorname{len}(q) \cdot \operatorname{len}(v)\big) \text{ bit operations}
$$

---

## 2. Part 1: Step Complexity Analysis — $O(n)$ Steps

We first establish that the total number of division steps $k$ is strictly linear in the bit length $n$ (i.e., logarithmic in the input values).

### Halving Property of Remainders

Consider a division step $r_{i-1}=q_ir_i+r_{i+1}$. We show that the remainder is reduced by **at least half** every two remainder indices: $r_{i+1}<r_{i-1}/2$.

* **Case 1 ($r_i\le r_{i-1}/2$):** Since $r_{i+1}<r_i$, it follows that $r_{i+1}<r_{i-1}/2$.
* **Case 2 ($r_i>r_{i-1}/2$):** Here $r_{i-1}/r_i<2$, so $q_i=1$. Thus $r_{i+1}=r_{i-1}-r_i<r_{i-1}/2$.

In all cases:

$$
r_{i+1}<\frac{r_{i-1}}{2}.
$$

### Bound on Total Steps

Applying this reduction recursively across $k$ steps:

$$
r_{2j} < \frac{r_0}{2^j} = \frac{a}{2^j}
$$

The algorithm terminates when the remainder becomes 0. Since the smallest non-zero integer remainder is 1, the maximum number of pairs of steps $j$ satisfies $a / 2^j \ge 1 \implies j \le \log_2(a) \le n$.

Therefore, the total number of division steps $k$ is bounded by:

$$
k \le 2n = O(n) \text{ steps}
$$

*(Lamé’s theorem gives at most five times the number of decimal digits of $b$. Since an $n$-bit integer has $O(n)$ decimal digits, this confirms $k=O(n)$.)*

---

## 3. Part 2: Bit Complexity Proof — $O(n^2)$ Operations

A common pitfall is to multiply the maximum step count $O(n)$ by the worst-case cost of dividing two $n$-bit integers $O(n^2)$, arriving at $O(n^3)$. This bound is loose because **large quotients are rare, and the size of the operands decreases at each step**.

### Step-by-Step Amortization

At step $i$, dividing $r_{i-1}$ by $r_i$ yields quotient $q_i$ and remainder $r_{i+1}$. The quotient length is at most $\operatorname{len}(r_{i-1})-\operatorname{len}(r_i)+1$ up to the usual leading-bit boundary.

The bit cost for step $i$ is:

$$
\operatorname{Cost}(i)
=O\bigl(\operatorname{len}(q_i)\operatorname{len}(r_i)\bigr).
$$

Since every intermediate remainder is less than $a<2^n$, we can bound $\operatorname{len}(r_i)\le n$ globally:

$$
\operatorname{Cost}(i)\le C\operatorname{len}(q_i)n.
$$

Summing this cost over all $k$ division steps gives the total bit complexity $T(n)$:

$$
T(n) = \sum_{i=1}^k \text{Cost}(i) \le C \cdot n \sum_{i=1}^k \operatorname{len}(q_i)
$$

---

### Telescoping Sum of Quotient Bit Lengths

To complete the proof, we compute the sum of quotient bit lengths $\sum_{i=1}^k \operatorname{len}(q_i)$.

From the sequence of division equations:

$$
\begin{aligned} a = r_0 &\ge q_1 r_1 \\ &\ge q_1 (q_2 r_2) \\ &\ge q_1 q_2 q_3 r_3 \\ &\;\:\vdots \\ &\ge \left( \prod_{i=1}^k q_i \right) r_k \ge \prod_{i=1}^k q_i \end{aligned}
$$

Taking the base-2 logarithm of both sides:

$$
\sum_{i=1}^k \log_2(q_i) \le \log_2(a) \le n
$$

Since the bit length of an integer $q_i$ satisfies $\operatorname{len}(q_i) \le \log_2(q_i) + 1$, we sum across all $k$ steps:

$$
\sum_{i=1}^k \operatorname{len}(q_i) \le \sum_{i=1}^k \big(\log_2(q_i) + 1\big) = \left( \sum_{i=1}^k \log_2(q_i) \right) + k \le n + k
$$

Using our step count bound $k = O(n)$:

$$
\sum_{i=1}^k \operatorname{len}(q_i) \le n + O(n) = O(n)
$$

---

### Final Cost Calculation

Substituting $\sum_{i=1}^k \operatorname{len}(q_i) = O(n)$ back into the total bit cost equation:

$$
T(n) \le C \cdot n \cdot O(n) = O(n^2)
$$

The classical Euclidean algorithm on two $n$-bit integers runs in **$O(n^2)$ bit operations**.

---

## 4. Complexity Comparison Across Models

| Complexity Metric | Naive Overestimate | Exact Classical Bound | Asymptotic Fast Variant (Half-GCD) |
| --- | --- | --- | --- |
| **Arithmetic Steps (Divisions)** | $O(n)$ | **$O(n)$** | $O(\log n)$ matrix steps |
| **Bit Operations per Step** | $O(n^2)$ | **Amortized $O(\operatorname{len}(q_i)n)$** | Governed by multiplication cost $M(n)$ |
| **Total Bit Complexity** | $O(n^3)$ | **$O(n^2)$** | **$O(M(n)\log n)$** |

> **Key Takeaway:** The $O(n^2)$ bit complexity of the Euclidean algorithm matches the time required for a single long division of $a$ by $b$. Computing the GCD of two $n$-bit integers is asymptotically no more expensive than dividing them once.

---

## References

* Knuth (1998), Volume 2, Section 4.5.2, cited by the source article.
* Möller (2008) and the GMP Manual, cited by the source article for half-GCD.
