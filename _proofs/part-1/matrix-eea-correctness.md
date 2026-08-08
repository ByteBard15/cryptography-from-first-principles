---
title: Matrix EEA Correctness
kind: Supplementary proof
---

# Correctness of the Matrix Extended Euclidean Algorithm (Matrix EEA) & Lehmer's Enhancement

The Extended Euclidean Algorithm (EEA) can be formulated as a sequence of $2\times2$ matrix transformations. This formulation simplifies implementation and forms the basis of **Lehmer’s Algorithm**, which accelerates the Euclidean algorithm on multi-precision integers by estimating quotients using single-precision leading digits.

---

## 1. Matrix Representation of the Extended Euclidean Algorithm

### Step Transition Matrix

Let $r_0=a$ and $r_1=b$. At transition $i\ge1$, dividing $r_{i-1}$ by $r_i$ produces quotient $q_i$ and remainder $r_{i+1}$:

$$
r_{i-1}=q_ir_i+r_{i+1}
\implies
r_{i+1}=r_{i-1}-q_ir_i.
$$

We express this transition as a matrix-vector product:

$$
\begin{pmatrix}r_i\\r_{i+1}\end{pmatrix}
=Q_i\begin{pmatrix}r_{i-1}\\r_i\end{pmatrix},
\qquad
Q_i=\begin{pmatrix}0&1\\1&-q_i\end{pmatrix}.
$$

### Accumulated Transformation Matrix

Let $r_0 = a$ and $r_1 = b$. Applying $k$ step transformations yields:

$$
\begin{pmatrix} r_k \\ r_{k+1} \end{pmatrix} = M_k \begin{pmatrix} a \\ b \end{pmatrix} \quad \text{where } M_k = Q_k Q_{k-1} \cdots Q_2 Q_1 = \begin{pmatrix} x_k & y_k \\ x_{k+1} & y_{k+1} \end{pmatrix}
$$

When the algorithm terminates at step $k$ with $r_{k+1}=0$, $r_k=\gcd(a,b)$. The **top row** of $M_k$ yields the Bézout coefficients:

$$
\begin{pmatrix} \gcd(a, b) \\ 0 \end{pmatrix} = \begin{pmatrix} x_k & y_k \\ x_{k+1} & y_{k+1} \end{pmatrix} \begin{pmatrix} a \\ b \end{pmatrix} \implies a x_k + b y_k = \gcd(a, b)
$$

---

## 2. Invariants and Proof of Correctness

To prove that the matrix formulation correctly computes the GCD and Bézout coefficients for any $a, b > 0$, we verify two key invariants by induction.

### Invariant 1: Row Linear Combinations

**Theorem:** For every remainder index $i\ge0$, $r_i=ax_i+by_i$.

* **Base Cases ($i = 0, 1$):**
* For $i = 0$: $r_0 = a = 1 \cdot a + 0 \cdot b$, so $(x_0, y_0) = (1, 0)$.
* For $i = 1$: $r_1 = b = 0 \cdot a + 1 \cdot b$, so $(x_1, y_1) = (0, 1)$.
* The initial matrix is $M_0 = I_2 = \begin{pmatrix} 1 & 0 \\ 0 & 1 \end{pmatrix}$.


* **Inductive step:** Assume the invariant holds for $i-1$ and $i$:
  * $r_{i-1}=ax_{i-1}+by_{i-1}$,
  * $r_i=ax_i+by_i$.


Applying $r_{i+1}=r_{i-1}-q_ir_i$ gives

$$
\begin{aligned}
r_{i+1}
&=(ax_{i-1}+by_{i-1})-q_i(ax_i+by_i)\\
&=a(x_{i-1}-q_ix_i)+b(y_{i-1}-q_iy_i).
\end{aligned}
$$


Setting $x_{i+1}=x_{i-1}-q_ix_i$ and $y_{i+1}=y_{i-1}-q_iy_i$ proves the invariant. $\blacksquare$

---

### Invariant 2: Determinant Invariant

**Theorem:** *$\det(M_i) = (-1)^i$ for all $i \ge 1$.*

* **Base Case:** $\det(Q_1) = \det \begin{pmatrix} 0 & 1 \\ 1 & -q_1 \end{pmatrix} = 0 - 1 = -1 = (-1)^1$.
* **Inductive Step:** Using the multiplicative property of determinants $\det(A B) = \det(A) \det(B)$:
  $$
  \det(M_i) = \det(Q_i M_{i-1}) = \det(Q_i) \det(M_{i-1}) = (-1) \cdot (-1)^{i-1} = (-1)^i
  $$



**Significance:** Since $\det(M_i) = x_i y_{i+1} - y_i x_{i+1} = \pm 1$, the rows of $M_i$ are coprime. This guarantees that $M_i$ is invertible over $\mathbb{Z}$, preserving the structural equivalence of the state transitions. $\blacksquare$

---

## 3. Lehmer’s Enhancement: Quotient Stability Condition

For multi-precision integers $a$ and $b$ (e.g., 4096-bit numbers), full precision division is computationally expensive. **Lehmer’s algorithm** extracts the high-order bits of $a$ and $b$ into single-precision machine words $\hat{a}$ and $\hat{b}$, computing partial quotients on these smaller values as long as they are guaranteed to match the exact quotients of full precision integers.

### Quotients from Truncated Bounds

Let $a$ and $b$ be approximated by single-precision intervals $[a_L, a_H]$ and $[b_L, b_H]$. The exact quotient at any step lies within the range:

$$
q_{\text{min}} = \left\lfloor \frac{a_L}{b_H} \right\rfloor \quad \text{and} \quad q_{\text{max}} = \left\lfloor \frac{a_H}{b_L} \right\rfloor
$$

### The Quotient Stability Rule

Provided $b_L>0$, equality of the endpoint quotients is a **sufficient stability test** for using $q$ with the full multi-precision values:

$$
\left\lfloor \frac{a_L}{b_H} \right\rfloor = \left\lfloor \frac{a_H}{b_L} \right\rfloor = q
$$

#### Lehmer Matrix Update Loop

1. Extract top machine words $a'$ and $b'$.
2. Initialize single-precision transformation matrix $A = \begin{pmatrix} 1 & 0 \\ 0 & 1 \end{pmatrix}$.
3. While $q_{\text{min}} == q_{\text{max}}$:
* Compute quotient $q = \lfloor a' / b' \rfloor$.
* Update single-precision state: $\begin{pmatrix} a' \\ b' \end{pmatrix} \leftarrow \begin{pmatrix} 0 & 1 \\ 1 & -q \end{pmatrix} \begin{pmatrix} a' \\ b' \end{pmatrix}$.
* Accumulate into $A \leftarrow \begin{pmatrix} 0 & 1 \\ 1 & -q \end{pmatrix} A$.


4. When $q_{\text{min}}\ne q_{\text{max}}$, apply the accumulated $2\times2$ matrix $A$ **once** to the multi-precision values $a$ and $b$:

$$
\begin{pmatrix} a_{\text{new}} \\ b_{\text{new}} \end{pmatrix} = A \begin{pmatrix} a \\ b \end{pmatrix}
$$

This batches several quotient steps before updating the full multi-precision values. The actual speedup and crossover point depend on the implementation and operand sizes; they are not fixed by the machine word size alone.

---

## 4. Summary: Matrix EEA Invariants

| Attribute | Standard Scalar EEA | Matrix EEA / Lehmer |
| --- | --- | --- |
| **State Vector** | $(r_{i-1}, r_i)^T$ | $M_i (a, b)^T$ |
| **Determinant** | N/A | $\det(M_i) = (-1)^i$ |
| **Bézout Invariant** | $r_i = a x_i + b y_i$ | $\begin{pmatrix} r_i \\ r_{i+1} \end{pmatrix} = \begin{pmatrix} x_i & y_i \\ x_{i+1} & y_{i+1} \end{pmatrix} \begin{pmatrix} a \\ b \end{pmatrix}$ |
| **Performance Edge** | Simple implementation | Single-word quotient work can amortize full-width updates |

---

## References

* Jebelean (1995), cited by the source article for Lehmer-style acceleration.
* Menezes et al. (1996), Section 14.4.2, cited by the source article.
* Knuth (1998), Volume 2, Section 4.5.2, cited by the source article.
