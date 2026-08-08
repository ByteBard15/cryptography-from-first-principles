---
title: Binary GCD Bit Complexity
kind: Supplementary proof
---

# Binary GCD (Stein’s Algorithm): Bit Complexity Analysis

While the classical Euclidean algorithm relies on integer division, Stein’s Binary GCD algorithm (1967) replaces division entirely with bitwise shifts, comparisons, and subtractions.

This section defines the underlying computational model, establishes the loop iteration bound, and proves the worst-case and average-case **$O(n^2)$** bit complexity.

---

## 1. Computational Model

To analyze bit complexity, we distinguish the asymptotic **bit-operation model** from fixed-width hardware costs:

1. **Input size:** Two positive $n$-bit integers $a$ and $b$, where

   $$
   n=\left\lfloor\log_2(\max(a,b))\right\rfloor+1.
   $$
2. **Primitive operations in the bit model:** comparison, subtraction, scanning for trailing zeros, and shifting an $n$-bit multi-word representation each cost at most $O(n)$ bit operations. On a fixed-width machine word, an instruction such as `TZCNT` may be constant latency, but that hardware fact is not the asymptotic cost model.



---

## 2. Algorithm Mechanics & Reduction Step

Stein's algorithm relies on three elementary arithmetic properties:

1. **Both Even:** $\gcd(2a, 2b) = 2 \cdot \gcd(a, b)$
2. **One Even, One Odd:** $\gcd(2a,b)=\gcd(a,b)$ if $b$ is odd
3. **Both Odd:** $\gcd(a,b)=\gcd(\lvert a-b\rvert,\min(a,b))$

```
Algorithm BinaryGCD(a, b):
  1. Remove common factors of 2: shift both until at least one is odd.
  2. While both a and b are non-zero:
      a. Shift out trailing zeros from a and b individually.
      b. If a > b, set a = (a - b) / 2.
         Else, set b = (b - a) / 2.
  3. Return stored power of 2 multiplied by remaining non-zero integer.

```

---

## 3. Deriving the Upper Bound on Iterations

We track the product $P = a \cdot b$ across main loop iterations where both $a$ and $b$ are odd.

When $a$ and $b$ are both odd, their difference $d = \lvert a-b\rvert$ is guaranteed to be even. Dividing $d$ by 2 produces a smaller replacement for the larger operand; stripping any additional powers of two can only reduce it further.

### Step Reduction Analysis

Suppose $a > b$. The new pair becomes $\left(\frac{a - b}{2}, b\right)$. We evaluate the reduction in the product:

$$
P_{\text{new}} = \left(\frac{a - b}{2}\right) \cdot b < \frac{a \cdot b}{2} = \frac{P_{\text{old}}}{2}
$$

Because the product $a \cdot b$ decreases by at least a factor of 2 in every main iteration:

* The initial product $P_0 = a \cdot b < 2^n \cdot 2^n = 2^{2n}$.
* After $k$ iterations, $P_k < 2^{2n - k}$.
* A nonterminal state has $P_k\ge1$, so it cannot persist through $2n$ such reductions.

Thus the number of main-loop subtraction iterations is bounded by

$$
k \le 2n
$$

---

## 4. Total Bit Complexity Proof

To find the total bit complexity, we multiply the number of iterations by the bit cost per iteration:

### 1. Cost per Iteration

* Performing an $n$-bit subtraction $a - b$ takes **O(n)** bit operations.
* Comparing two $n$-bit integers takes **O(n)** bit operations.
* Removing trailing zeros from an $n$-bit multi-word integer costs at most $O(n)$ bit operations in one iteration.

### 2. Total Worst-Case Bit Complexity

$$
\text{Total Operations} = (\text{Total Iterations}) \times (\text{Cost per Iteration})
$$

$$
T(n) = O(n) \text{ iterations} \times O(n) \text{ bit operations/iteration} = O(n^2)
$$

---

## 5. Average-Case Complexity & The Brent Citation

While the worst-case bound above is $2n$ subtraction iterations, the article cites **Richard P. Brent (2000)** for average-case analysis.

### Brent's Continuous Model

Brent modeled the state transitions of odd integers $(a, b)$ in the binary algorithm using a continuous probability density function on the unit interval $[0, 1]$.

* **Expected shift length:** In the random model, the probability that the difference of two odd integers has exactly $k$ trailing zero bits is $2^{-k}$ for $k\ge1$. Hence

$$
\sum_{k=1}^{\infty} k \cdot 2^{-k} = 2 \text{ bits}
$$

* **Expected iteration count:** Brent reports the established asymptotic constant for the right-shift binary algorithm under its stated random-input model:

$$
K_n \approx 0.70597 \cdot n
$$

Since each subtraction step operates on $O(n)$-bit integers on average, multiplying the expected iterations by the average bit length yields:

$$
T_{\text{avg}}(n) = (0.70597 \cdot n) \times O(n) = O(n^2)
$$

---

## 6. Summary Comparison: Classical vs. Binary GCD

| Metric | Classical Euclidean Algorithm | Binary GCD (Stein's Algorithm) |
| --- | --- | --- |
| **Primary Operation** | Integer Division (`a mod b`) | Subtraction & Bit Shifts (`a - b`, `>> 1`) |
| **Iteration Bound** | $O(n)$ divisions | $\le 2n$ subtraction iterations |
| **Cost Accounting** | Variable-size divisions amortize over the run | At most $O(n)$ bit operations per iteration |
| **Asymptotic Bit Complexity** | $O(n^2)$ | $O(n^2)$ |
| **Hardware performance** | Platform- and input-dependent | Platform- and input-dependent |

> **Subquadratic GCD:** The half-GCD approach cited in the article uses divide-and-conquer matrix reductions and has complexity $O(M(n)\log n)$, where $M(n)$ is the multiplication cost.

---

## References

* Brent (2000), cited by the source article for average-case binary-GCD analysis.
* Knuth (1998), Volume 2, Section 4.5.2, cited by the source article for GCD analysis.
