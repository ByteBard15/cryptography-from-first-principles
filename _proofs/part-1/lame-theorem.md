---
title: Lamé’s Theorem
kind: Supplementary proof
---

# Lamé’s Theorem: Proof & Upper Bound Analysis

Lamé’s theorem (1844) establishes a worst-case step bound for the Euclidean algorithm. If $d$ is the number of decimal digits in the smaller positive input $b$, the number of divisions $k$ is at most $5d$:

$$
k\le 5d,
\qquad
d=\lfloor\log_{10}b\rfloor+1.
$$

---

## 1. Linear Recurrence & Binet’s Formula

The worst-case behavior of the Euclidean algorithm occurs when every nonterminal quotient $q_i$ is 1. To model this bound, analyze the Fibonacci sequence $F_m$ defined by

$$
F_m = F_{m-1} + F_{m-2} \quad \text{with } F_0 = 0, F_1 = 1
$$

### Characteristic Equation & Solution Space

Hypothesizing a solution of the form $F_m=r^m$, the recurrence becomes

$$
r^m = r^{m-1} + r^{m-2} \implies r^2 - r - 1 = 0
$$

Solving this quadratic equation via the quadratic formula yields two characteristic roots:

$$
\phi = \frac{1 + \sqrt{5}}{2} \approx 1.6180339, \quad \psi = \frac{1 - \sqrt{5}}{2} \approx -0.6180339
$$

Because the recurrence is linear, any linear combination of $\phi^m$ and $\psi^m$ is also a solution. The general solution is

$$
F_m = A\phi^m + B\psi^m
$$

Using the initial conditions $F_0 = 0$ and $F_1 = 1$, we solve for constants $A$ and $B$:

$$
A + B = 0 \implies B = -A
$$

$$
A\phi + B\psi = 1 \implies A(\phi - \psi) = 1 \implies A = \frac{1}{\sqrt{5}}
$$

This yields **Binet’s Formula**:

$$
F_m = \frac{\phi^m - \psi^m}{\sqrt{5}}
$$

Since $\lvert\psi\rvert\approx0.618<1$, the term $\psi^m$ decays as $m$ grows. This gives the strict lower bound for $m\ge3$:

$$
F_m > \phi^{m-2}
$$

---

## 2. Bounding the Euclidean Algorithm Steps

Let $R_0=a$ and $R_1=b$, where $a>b>0$. If the algorithm takes $k$ divisions, write

$$
R_{i-1}=q_iR_i+R_{i+1}
\quad(1\le i\le k),
$$

where $R_k=\gcd(a,b)$ and $R_{k+1}=0$.

Because $R_k\ge1$, every $q_i\ge1$, and the final quotient $q_k\ge2$, reconstructing the smallest possible remainders backwards gives

$$
\begin{aligned}
R_k&\ge1=F_2,\\
R_{k-1}&\ge2R_k\ge2=F_3,\\
R_{k-2}&\ge R_{k-1}+R_k\ge F_3+F_2=F_4,
\end{aligned}
$$

Continuing to $R_1=b$ yields

$$
b\ge F_{k+1}.
$$

---

## 3. Deriving the digit bound

Substituting the Fibonacci growth inequality $F_{k+1} > \phi^{(k+1)-2} = \phi^{k-1}$ into $b \ge F_{k+1}$:

$$
b > \phi^{k-1}
$$

Taking the base-$\phi$ logarithm on both sides:

$$
\log_\phi b > k - 1 \implies k < 1 + \log_\phi b
$$

If $b$ has $d$ decimal digits, then $b<10^d$. Converting the logarithm base using $\log_\phi 10\approx4.78497$ gives

$$
\log_\phi b<\log_\phi(10^d)=d\log_\phi10<4.785d.
$$

Substituting this back into our step inequality produces the continuous upper bound:

$$
k<1+4.785d.
$$

---

## 4. The discrete step to $k\le5d$

To prove that $k<1+4.785d$ implies $k\le5d$ for every integer digit count $d\ge1$, partition the domain of $d$.

### Case A: $d\ge5$

We determine when $1+4.785d\le5d$ holds algebraically:

$$
1\le(5-4.785)d
\iff
d\ge\frac{1}{0.215}\approx4.651.
$$

Since $d$ is an integer, $1+4.785d\le5d$ for all $d\ge5$. Combined with the previous bound:

$$
k<1+4.785d\le5d.
$$

Because $k$ and $5d$ are integers, this implies $k\le5d$.

### Case B: $1\le d\le4$

For the remaining digit counts, evaluate the integer consequence directly:

* $d=1$: $k<5.785$, so $k\le5$.
* $d=2$: $k<10.570$, so $k\le10$.
* $d=3$: $k<15.355$, so $k\le15$.
* $d=4$: $k<20.140$, so $k\le20$.

---

## Conclusion

Across all $d\ge1$, the upper bound holds:

$$
k\le5d.
$$

This completes the proof. The Euclidean algorithm runs in logarithmic time relative to its inputs, with a worst-case step count proportional to 5 times the number of decimal digits in the smaller input.

---

## References & Further Reading

* Knuth (1998), Volume 2, Section 4.5.2, cited by the source article.
* Lamé (1844), named by the source article.
