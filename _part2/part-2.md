---
title: "Constructing the Trapdoor"
nav_title: "Part 2: Constructing the Trapdoor with Euler's Theorem, RSA, and CRT"
order: 2
permalink: /part-2/
description: Building RSA from modular exponentiation, Euler's theorem, hidden factorization, and the Chinese Remainder Theorem.
---

# Constructing the Trapdoor

---

## What Are We Actually Trying to Build?

Before diving into number theory, prime factorization, or modular exponentiation, we need to step back and define the exact functional blueprint of the system we want to construct.

If we remove all historical context and mathematical implementation details, public-key cryptography boils down to a structural requirements problem.

---

### The Primitive Transformations

Suppose we want to transmit a message *M* securely across an untrusted network. To do this, we need an **encryption transformation** *F*:

$$F(M, x) = C$$

where:

* *M* is the original message (plaintext), represented as data.
* *C* is the transformed output (ciphertext).
* *x* is the mathematical parameter supplied to the encryption function.

To recover the original message *M* from *C*, we need a corresponding **decryption transformation** *E*:

$$E(C, y) = M$$

where *y* contains whatever information is required to reverse the transformation applied by *F*.

Combining both transformations yields the fundamental correctness identity for our system:

$$E(F(M, x), y) = M$$

---

### The Asymmetry Requirement

In a classical symmetric system, *x* and *y* are identical (or trivially derived from one another). Anyone who knows how to lock the box also knows how to unlock it.

To achieve asymmetric (public-key) communication, we impose a strict structural constraint on the distribution of information:

> **The Asymmetry Condition:** The information *x* required to compute the forward transformation *F* must be made entirely public, while the information *y* required to compute the inverse transformation *E* must remain strictly private.

```
       [ Public Key x ]                 [ Private Key y ]
              │                                │
              ▼                                ▼
Plaintext M ──► [ F(M, x) ] ──► Ciphertext C ──► [ E(C, y) ] ──► Plaintext M

```

This condition introduces a severe computational paradox:

1. Every observer on the network knows the exact algorithm *F*.
2. Every observer on the network knows the exact public key *x*.
3. Any observer can compute $C = F(M, x)$ for any message *M* of their choice.

Yet, despite having full access to *F* and *x*, an observer should have no known efficient method for evaluating the inverse function $F^{-1}(C)$ to recover *M* under the system's security assumptions.

---

### Trapdoor One-Way Functions (TOWF)

This requirement moves us beyond standard invertible mathematical functions.

We already know how to build functions that are easy to invert, such as linear equations, matrix multiplications, simple permutations, and standard XOR operations. However, in all those classical structures, knowing how to compute $y = f(x)$ automatically reveals how to compute $x = f^{-1}(y)$.

To build an asymmetric cipher, we need a **Trapdoor One-Way Function**.

A Trapdoor One-Way Function is a family of invertible functions $f_x$ that satisfy three specific computational properties:

1. **Efficient Forward Computation:** Given an input *M* and public parameter *x*, computing $C = f_x(M)$ is computationally cheap; it runs in polynomial time $O(n^k)$.
2. **Hard Inversion (Without Trapdoor):** Given only the output *C* and public parameter *x*, no known efficient algorithm can recover a valid input *M* with non-negligible success under the relevant security assumptions.
3. **Easy Inversion (With Trapdoor):** There exists a piece of secret information *y* (the "trapdoor"). Given *C*, *x*, and *y*, computing $M = E(C, y)$ is computationally cheap; it runs in polynomial time $O(n^k)$.

---

### The Core Architectural Problem

Our central goal in constructing an asymmetric cryptosystem is **not** simply finding a function that has an inverse. We do that every day in basic algebra.

Our goal is finding an invertible function where **the capability to compute the function forward does not grant the capability to compute its inverse.**

In the next section, we will explore the specific mathematical domain of modular arithmetic and algebraic groups that allows us to build a physical "trapdoor" into arithmetic operations.

---

## Our First Attempt: Multiplication

Now that we have established the abstract requirement of a Trapdoor One-Way Function, let's attempt to build one using the simplest algebraic operation available in modular arithmetic: **linear multiplication**.

---

### A Linear Modular Cipher

Suppose we want to encrypt a plaintext message *M* (represented as an integer modulo *N*). We choose a public encryption parameter *x* and define our encryption transformation as:

$$C \equiv M \cdot x \pmod N$$

To decrypt the ciphertext *C* and recover *M*, we need a transformation *E(C, y)* that reverses multiplication by *x*:

$$E(C, y) \equiv C \cdot y \equiv (M \cdot x) \cdot y \pmod N$$

For $(M \cdot x) \cdot y \equiv M \pmod N$ to hold for every message *M*, the parameters *x* and *y* must satisfy:

$$x \cdot y \equiv 1 \pmod N$$

In other words, the decryption parameter *y* must be the **modular multiplicative inverse** of *x*:

$$y \equiv x^{-1} \pmod N$$

This gives us a neatly defined cipher pair:

* **Encryption:** $C \equiv M \cdot x \pmod N$
* **Decryption:** $M \equiv C \cdot x^{-1} \pmod N$

There is nothing cryptographically useless about multiplication itself. Multiplication by an invertible value is a perfectly valid reversible transformation and appears throughout cryptography. The problem is specifically that it does **not create asymmetry** here. The same public values $x$ and $N$ that let anyone encrypt also let anyone compute $x^{-1}$ and decrypt.

---

### The Solution Provided by Part 1

At first glance, this algebraic system appears complete and fully functional. We have a clear forward transformation and a deterministic inverse.

Furthermore, we already know how to compute $x^{-1} \pmod N$. In Part 1, we analyzed the Extended Euclidean Algorithm (EEA) and proved that provided $\gcd(x, N) = 1$, the EEA computes the exact modular inverse $x^{-1} \pmod N$ in $O(n^2)$ bit operations.

The EEA remains efficient as the bit length of $N$ grows, so increasing the modulus size does not turn this linear construction into a trapdoor function.

---

### Why Linear Multiplication Fails the Asymmetry Condition

Now, let's test this construction against the **Asymmetry Condition** established earlier: *The information required to perform encryption must be public, but the information required to reverse encryption must remain strictly private.*

Let's evaluate the three possible ways to distribute the key parameters *x* and $x^{-1}$:

#### Scenario 1: Publish $x$, Keep $x^{-1}$ Secret

If we publish *x* and *N* as our public key so anyone can encrypt messages to us, an attacker observing $C \equiv M \cdot x \pmod N$ can read every message.

Because the attacker knows both *x* and *N*, they simply run the Extended Euclidean Algorithm on *x* and *N* to obtain $x^{-1} \pmod N$ in $O(n^2)$ time. They then compute $M \equiv C \cdot x^{-1} \pmod N$.

**Result:** There is no trapdoor. The ability to encrypt trivially grants the ability to decrypt.

#### Scenario 2: Keep $x$ Secret

If we keep *x* secret to prevent attackers from running the EEA, legitimate users can no longer encrypt messages to us. The system devolves into a standard symmetric cipher, destroying public-key accessibility entirely.

#### Scenario 3: Publish $x^{-1}$, Keep $x$ Secret

What if we flip the keys? Suppose we publish $y = x^{-1} \bmod N$ as the public encryption key and keep *x* private for decryption.

This changes nothing. Because modular inversion is symmetric, $(x^{-1})^{-1} \equiv x \pmod N$, anyone given $y$ and *N* can run the EEA on *y* to recover *x* in $O(n^2)$ operations.

---

### The Crucial Takeaway: Part 1 as a Limitation Tool

Notice the structural irony: **the very algorithmic efficiency we established in Part 1 is precisely what destroys linear multiplication as a public-key cipher.**

The issue is not merely that multiplication is reversible. Reversibility is required for decryption. The issue is that **the public information needed for the forward transformation also reveals, through an efficient calculation, the information needed for the inverse transformation**. The Extended Euclidean Algorithm is *too efficient* to let this construction separate public encryption from private decryption.

```
       Linear Multiplication: C ≡ M · x (mod N)
                          │
                          ▼
            [ Public Encryption Key x ]
                          │
                          │ Extended Euclidean Algorithm
                          │ Cost: O(n²), Extremely Fast!
                          ▼
            [ Private Decryption Key x⁻¹ ]

```

If we want a system where the ability to execute the forward function does **not** grant the ability to derive its inverse, we need a transformation whose public description does not expose an efficient inverse so directly. This motivates our next experiment: **modular exponentiation**.

## What If We Use Exponentiation?

With multiplication, the public transformation

$$C \equiv Mx \pmod N$$

immediately exposes the form of its inverse:

$$M \equiv Cx^{-1} \pmod N.$$

What if the public transformation did not expose its inverse so directly?

Instead of using a key *x* as a **multiplier**, what if we use an exponent *e* as a **power**?

---

### Shifting from Multiplication to Exponents

We define our encryption function *F* using a public exponent *e*:

$$F(M, e) \equiv M^e \pmod N$$

Unlike the multiplication experiment, the inverse is not presented to us as one obvious arithmetic operation on $e$. We hypothesize that decryption might use another exponent $d$:

$$E(C, d) \equiv C^d \pmod N$$

Substituting the encryption function into the decryption function yields:

$$E(F(M, e), d) \equiv (M^e)^d \equiv M^{ed} \pmod N$$

To satisfy the fundamental correctness requirement $E(F(M, e), d) \equiv M \pmod N$, our entire problem becomes:

> **The Identity Condition:** Can we find an exponent $d$ such that raising every valid message $M$ to the combined exponent $ed$ leaves it unchanged modulo $N$?

$$M^{ed} \equiv M \pmod N$$

---

### The Trivial Trap vs. Cyclic Exponentiation

How do we force $M^{ed} \equiv M \pmod N$ to hold?

In standard real or integer arithmetic, the only way $(M^e)^d = M^1$ holds for all $M$ is if the exponents themselves satisfy the literal equation $ed = 1$. But over the positive integers, the only solution to $ed = 1$ is $e = 1$ and $d = 1$. This is completely useless for cryptography because it means the encryption function is $C \equiv M^1 \pmod N$, which does not transform the message at all.

However, we are not operating over the infinite set of real numbers; we are operating inside the finite algebraic system $\mathbb{Z}/N\mathbb{Z}$.

In a finite modular system, powers do not grow without bound. Because only finitely many residue classes exist, the sequence $M^1, M^2, M^3, \dots \pmod N$ must eventually repeat. Repetition alone, however, does not guarantee that the sequence reaches $1$. For example, modulo $8$,

$$2^r \not\equiv 1 \pmod 8 \quad \text{for every positive integer } r.$$

The correct domain is the set of **invertible residues**, also called the group of units:

$$(\mathbb{Z}/N\mathbb{Z})^\times = \{M \pmod N : \gcd(M,N)=1\}.$$

If $M$ is a unit, its powers do return to $1$. The smallest positive exponent with this property is the **multiplicative order** of $M$ modulo $N$:

$$r = \operatorname{ord}_N(M) \quad \Longleftrightarrow \quad M^r \equiv 1 \pmod N.$$

Different units can have different orders. To choose one exponent pair $(e,d)$ that works for every unit, we therefore seek a **universal exponent** $R$ that is divisible by the order of every $M \in (\mathbb{Z}/N\mathbb{Z})^\times$.

---

### The Structural Break: Setting $ed = 1 + kR$

If such a universal exponent $R$ exists, we do **not** need $ed = 1$ in the integers. We only need the product $ed$ to land one step past a multiple of $R$.

Mathematically, we set:

$$ed = 1 + kR \quad \text{for some integer } k \ge 0$$

Now, watch what happens when we evaluate $M^{ed} \pmod N$ under this condition:

$$\begin{aligned} M^{ed} &\equiv M^{1 + kR} \pmod N \\        &\equiv M^1 \cdot M^{kR} \pmod N \\        &\equiv M \cdot (M^R)^k \pmod N \end{aligned}$$

If $M^R \equiv 1 \pmod N$, we substitute 1 directly into the equation:

$$M^{ed} \equiv M \cdot (1)^k \equiv M \cdot 1 \equiv M \pmod N$$

---

### The Pivot to Number Theory

By shifting from linear multiplication to modular exponentiation, we have reframed the cryptographic problem:

```
        CRITICAL SHIFT IN PROBLEM FORMULATION
        
  Old Problem (Multiplication):
  Find x⁻¹ such that x · x⁻¹ ≡ 1 (mod N)
  └─► Destroyed by EEA: Extremely easy for anyone to compute.

  New Problem (Exponentiation):
  Find an exponent pair (e, d) such that ed ≡ 1 (mod R)
  └─► Requires a universal exponent R for the units modulo N.

```

We no longer need to hide a modular inverse calculation. We need a public modulus whose relevant group exponent can be computed efficiently by the key holder but for which no efficient public computation is known.

* If an observer knows a suitable universal exponent $R$, they can compute $d \equiv e^{-1} \pmod R$ using the Extended Euclidean Algorithm, provided $\gcd(e,R)=1$.
* RSA obtains the required asymmetry from the hidden factorization $N=pq$. Knowing $p$ and $q$ makes a suitable exponent easy to compute; knowing only $N$ does not provide any known efficient route to the same information.

The search for a trapdoor function has transformed into a fundamental number-theoretic question: **Which exponent is guaranteed to return every unit modulo $N$ to $1$, and how can the factorization of $N$ make that exponent available only to the key generator?**

This is the exact point where **Euler’s Totient Function** and **Euler's Theorem** enter our construction.

## The Multiplicative Structure of $\mathbb{Z}/N\mathbb{Z}$

We have reduced the entire problem to finding an exponent that makes modular exponentiation come back around. If we can find a universal exponent $R$ such that

$$M^R \equiv 1 \pmod N$$

for every $M \in (\mathbb{Z}/N\mathbb{Z})^\times$, then choosing $ed\equiv1\pmod R$ gives $M^{ed}\equiv M\pmod N$ for every unit.

The question is no longer whether exponentiation might work. It is now precise:

> **What value of $R$ works reliably across the multiplicative system modulo $N$?**

To answer it, we must examine the algebraic structure of integers modulo $N$.

---

Not all integers modulo $N$ behave identically under multiplication. Specifically, **not every element in $\mathbb{Z}_N$ has a multiplicative inverse**.

Consider $N = 8$. The set of integers modulo 8 is $\{0, 1, 2, 3, 4, 5, 6, 7\}$. Let's check which elements possess a multiplicative inverse modulo 8:

| Element $a$ | $\gcd(a, 8)$ | Invertible Modulo 8? | Inverse $a^{-1} \pmod 8$ |
| --- | --- | --- | --- |
| **1** | $1$ | **Yes** | $1$ |
| **2** | $2 \ne 1$ | No | None ($2 \cdot x$ is always even) |
| **3** | $1$ | **Yes** | $3$ (since $3 \times 3 = 9 \equiv 1$) |
| **4** | $4 \ne 1$ | No | None |
| **5** | $1$ | **Yes** | $5$ (since $5 \times 5 = 25 \equiv 1$) |
| **6** | $2 \ne 1$ | No | None |
| **7** | $1$ | **Yes** | $7$ (since $7 \times 7 = 49 \equiv 1$) |

Only the elements coprime to $N$, those satisfying $\gcd(a, N) = 1$, possess multiplicative inverses. These invertible elements form the **multiplicative group of units modulo $N$**, denoted as $(\mathbb{Z}/N\mathbb{Z})^\times$.

Non-invertible elements (like $2, 4, 6 \pmod 8$) fall into zero-divisor sub-structures. They collapse under multiplication and cannot generate clean, reversible cycle lengths for all elements.

---

## Euler's Totient Function $\phi(N)$

Because only coprime elements participate in this invertible algebraic structure, we need a function that counts them.

**Euler's Totient Function** $\phi(N)$ is defined as the number of integers in $\{1, 2, \dots, N-1\}$ that are relatively prime to $N$:

$$\phi(N) = \big\vert{} \{ a \in \mathbb{Z} : 1 \le a < N \text{ and } \gcd(a, N) = 1 \} \big\vert{}$$

> **Motivation:** $\phi(N)$ does not appear in public-key cryptography by arbitrary choice. It appears because **$\phi(N)$ measures the exact size of the multiplicative group $(\mathbb{Z}/N\mathbb{Z})^\times$**. It counts precisely the elements that preserve multiplicative structure.

---

## Euler's Theorem: Full Proof

We now prove that the group size $\phi(N)$ gives us a universal exponent for the units modulo $N$.

At this point, we could simply quote Euler's theorem and move on. But that would defeat the purpose of building cryptography from first principles. In Part 1, we did not merely use the Extended Euclidean Algorithm; we opened it up and established why it works. We will do the same here.

### Euler's Theorem

> For any positive integers $a$ and $N$ such that $\gcd(a, N) = 1$:
> $$a^{\phi(N)} \equiv 1 \pmod N$$
>
>

### Proof

Let $\mathcal{R}$ be the set of all $\phi(N)$ distinct reduced residue classes modulo $N$:

$$\mathcal{R} = \{ r_1, r_2, \dots, r_{\phi(N)} \}$$

where each $r_i$ satisfies $1 \le r_i < N$ and $\gcd(r_i, N) = 1$.

Now, multiply every element in $\mathcal{R}$ by $a$ (where $\gcd(a, N) = 1$), forming a new set $a\mathcal{R}$:

$$a\mathcal{R} = \{ a r_1, a r_2, \dots, a r_{\phi(N)} \}$$

We claim that $a\mathcal{R} \pmod N$ is simply a **permutation** of the original set $\mathcal{R}$:

1. **Every element in $a\mathcal{R}$ is coprime to $N$:** Since $\gcd(a, N) = 1$ and $\gcd(r_i, N) = 1$, their product satisfies $\gcd(a r_i, N) = 1$. Thus, every element in $a\mathcal{R}$ reduces to one of the elements in $\mathcal{R}$.
2. **All elements in $a\mathcal{R}$ are distinct modulo $N$:** Suppose $a r_i \equiv a r_j \pmod N$. Because $\gcd(a, N) = 1$, $a$ has a multiplicative inverse $a^{-1} \pmod N$ (which we know how to calculate via the EEA from Part 1). Multiplying both sides by $a^{-1}$ yields:
   $$a^{-1}(a r_i) \equiv a^{-1}(a r_j) \implies r_i \equiv r_j \pmod N$$


Since all $r_i$ in $\mathcal{R}$ are distinct, all $a r_i$ in $a\mathcal{R}$ must also be distinct modulo $N$.

Since $a\mathcal{R} \pmod N$ contains $\phi(N)$ distinct elements, all of which are coprime to $N$, $a\mathcal{R} \pmod N$ must be the set $\mathcal{R}$ reordered.

Because the set of elements is identical, the product of all elements in $aR$ must equal the product of all elements in $R$ modulo $N$:

$$(a r_1) \cdot (a r_2) \cdots (a r_{\phi(N)}) \equiv r_1 \cdot r_2 \cdots r_{\phi(N)} \pmod N$$

Factoring out $a$ from each of the $\phi(N)$ terms on the left side:

$$a^{\phi(N)} \left( \prod_{i=1}^{\phi(N)} r_i \right) \equiv \left( \prod_{i=1}^{\phi(N)} r_i \right) \pmod N$$

Let $P = \prod_{i=1}^{\phi(N)} r_i$. Because every $r_i$ is coprime to $N$, their product $P$ is also coprime to $N$, meaning $\gcd(P, N) = 1$.

Therefore, $P$ possesses a modular multiplicative inverse $P^{-1} \pmod N$. Multiplying both sides of the equivalence by $P^{-1}$:

$$a^{\phi(N)} \cdot P \cdot P^{-1} \equiv P \cdot P^{-1} \pmod N$$

$$a^{\phi(N)} \equiv 1 \pmod N \quad \blacksquare$$

---

### From Group Order to a Universal Exponent

Euler's theorem gives us a universal exponent that works for every invertible residue modulo $N$:

$$M^{\phi(N)} \equiv 1 \pmod N \quad \text{for every } M \in (\mathbb{Z}/N\mathbb{Z})^\times.$$

It need not be the smallest exponent that works for a particular residue. If

$$r = \operatorname{ord}_N(M),$$

then the multiplicative order satisfies

$$r \mid \phi(N).$$

To see why, divide $\phi(N)$ by $r$: write $\phi(N)=qr+s$ with $0\le s<r$. Euler's theorem and the definition of $r$ give

$$1\equiv M^{\phi(N)}=M^{qr+s}\equiv\left(M^r\right)^qM^s\equiv M^s\pmod N.$$

Because $r$ is the smallest positive exponent that sends $M$ to $1$, the remainder must be $s=0$. Hence $r\mid\phi(N)$.

Thus, $r$ is the actual cycle length of $M$, while $\phi(N)$ is a universal exponent supplied by Euler's theorem and used in our introductory construction. This distinction between an element's order and Euler's universal exponent is developed further in the [Part 2 number-theory references]({{ '/references/part-2/' | relative_url }}#number-theory).

If we choose exponents $e$ and $d$ such that:

$$ed \equiv 1 \pmod{\phi(N)} \implies ed = 1 + k\phi(N)$$

Then for any message $M$ coprime to $N$:

$$M^{ed} \equiv M^{1 + k\phi(N)} \equiv M \cdot (M^{\phi(N)})^k \equiv M \cdot (1)^k \equiv M \pmod N$$

We now have the core algebraic engine for exponent-based encryption over the units modulo $N$. The remaining task is to construct a modulus $N$ for which the key generator can compute $\phi(N)$ efficiently from secret information, while no efficient method is known for computing it from the public modulus alone.

## From Euler's Theorem to RSA

We established the fundamental algebraic identity required for exponent-based decryption:

$$M^{ed} \equiv M \pmod N$$

Euler's theorem then provided a universal exponent for elements in $(\mathbb{Z}/N\mathbb{Z})^\times$:

$$M^{\phi(N)} \equiv 1 \pmod N \quad \text{for } \gcd(M, N) = 1$$

We can now connect these two results directly. If we choose exponents $e$ and $d$ such that their product is congruent to $1$ modulo $\phi(N)$:

$$ed \equiv 1 \pmod{\phi(N)}$$

Then, by definition of modular congruence, there exists some non-negative integer $k$ such that:

$$ed = 1 + k\phi(N)$$

Substituting this relation into $M^{ed} \pmod N$ yields:

$$\begin{aligned} M^{ed} &\equiv M^{1 + k\phi(N)} \pmod N \\        &\equiv M \cdot \left( M^{\phi(N)} \right)^k \pmod N \\        &\equiv M \cdot (1)^k \pmod N \\        &\equiv M \pmod N \end{aligned}$$

> **Important Distinction:** At this point, **we have not constructed RSA yet**. We have shown only that choosing $ed\equiv1\pmod{\phi(N)}$ is sufficient to reverse exponentiation for messages in the unit group. We still need a suitable modulus, a way to compute $d$, and a proof that RSA also works for non-units.

---

## Choosing $N=pq$: The Factorization Trapdoor

We have discovered that knowing a suitable group exponent makes it easy to derive the inverse exponent. RSA therefore needs a public modulus whose relevant group order can be computed efficiently by the key holder but not by a known efficient method from the public modulus alone. The hidden information that provides this asymmetry is the factorization $N=pq$, not $\phi(N)$ by itself.

However, this introduces a new design dilemma: **What structure should $N$ take?**

To fulfill the Asymmetry Condition established earlier, our modulus $N$ must satisfy three conflicting properties:

1. Constructing $N$ must be computationally easy.
2. Computing $\phi(N)$ must be easy for the key generator (who holds secret parameters).
3. No efficient algorithm should be known for computing the required private exponent from the public information $(N,e)$.

---

### The Composite Modulus $N = pq$

If we choose $N$ to be a prime $p$, then $\phi(p) = p - 1$. Since $p = N$, anyone given the public key $N$ could immediately compute $\phi(N) = N - 1$ and run the EEA to derive $d$.

Instead, we construct $N$ as the product of two large, distinct prime numbers $p$ and $q$:

$$N = p \cdot q$$

---

### Deriving $\phi(pq) = (p-1)(q-1)$ via Inclusion-Exclusion

To evaluate $\phi(pq)$, we must count how many integers in $\{1, 2, \dots, pq-1\}$ are coprime to $pq$. We prove this using the **Principle of Inclusion-Exclusion (PIE)**.

1. **Total Candidates:** The set contains $pq-1$ integers.
2. **Multiples of $p$:** These are $\{p,2p,\dots,(q-1)p\}$, giving exactly $q-1$ terms.
3. **Multiples of $q$:** These are $\{q,2q,\dots,(p-1)q\}$, giving exactly $p-1$ terms.
4. **Overlap:** No integer below $pq$ is divisible by both distinct primes $p$ and $q$.

By the Principle of Inclusion-Exclusion, the count of integers that share a factor with $N = pq$ is:

$$\text{Non-coprime elements} = (q-1)+(p-1)=p+q-2$$

Subtracting these non-coprime elements from the total set $pq$:

$$\begin{aligned} \phi(pq) &= (pq-1)-(p+q-2) \\          &= pq-p-q+1 \\          &= (p-1)(q-1) \end{aligned}$$

$$\phi(pq) = (p - 1)(q - 1)$$

---

### The Asymmetric Flow of Information

This simple algebraic identity creates our required trapdoor:

```
        SECRET DOMAIN                               PUBLIC DOMAIN
  (Known only to Key Generator)                   (Published to World)

    Primes p, q (Large)
         │
         ├───► Multiply: N = p · q ──────────────────────► Modulus N
         │
         ├───► Compute: φ(N) = (p - 1)(q - 1)
         │          │
    Choose e ───────┴─► Extended Euclidean Alg.
    gcd(e, φ(N)) = 1    d ≡ e⁻¹ (mod φ(N))
                        │
                        └───────────────────────────────► Public Exponent e
                                                                │
   Private Exponent d ◄─────────────────────────────────────────┘

```

* **With $p$ and $q$:** Computing $N = pq$ and $\phi(N) = (p-1)(q-1)$ is efficient.
* **Without $p$ and $q$:** No efficient classical algorithm is known for factoring a properly generated large RSA modulus. For a two-prime modulus, knowing $\phi(N)$ also reveals the factorization because $p+q=N-\phi(N)+1$ and $pq=N$.

---

## Computing $d$ with the Extended Euclidean Algorithm

The requirement $ed \equiv 1 \pmod{\phi(N)}$ means that the decryption exponent $d$ is the **modular multiplicative inverse** of the encryption exponent $e$, computed modulo $\phi(N)$:

$$d \equiv e^{-1} \pmod{\phi(N)}$$

From Part 1, we know precisely when and how this inverse exists:

1. **Existence:** The exponent $d$ exists if and only if $\gcd(e, \phi(N)) = 1$.
2. **Computation:** The Extended Euclidean Algorithm (EEA) calculates $d = e^{-1} \bmod \phi(N)$ in $O(\operatorname{len}(\phi(N))^2)$ bit operations under the cost model developed in Part 1.

Here we encounter the core architectural callback of our construction:

> **The Architectural Callback:** In Part 1, we analyzed the Extended Euclidean Algorithm to understand *how* to compute modular inverses efficiently. Here, we discover *why* public-key cryptography requires them.

### A Note on the RSA Exponent: $\phi(N)$ vs. $\lambda(N)$

The construction above uses $\phi(N)$ because Euler's theorem gives us a direct and well-motivated universal exponent. This is mathematically correct, but it is not the tightest exponent available.

The **Carmichael function** $\lambda(N)$ is the smallest positive universal exponent such that

$$M^{\lambda(N)} \equiv 1 \pmod N$$

for every $M\in(\mathbb{Z}/N\mathbb{Z})^\times$. For ordinary two-prime RSA,

$$\lambda(N)=\operatorname{lcm}(p-1,q-1),$$

whereas

$$\phi(N)=(p-1)(q-1).$$

Because $\lambda(N)\mid\phi(N)$, using $\phi(N)$ remains sufficient. It simply uses a possibly larger universal exponent than necessary. For pedagogical purposes, we will keep the main construction in terms of $\phi(N)$ because it follows directly from the theorem we proved.

Production RSA specifications use the tighter condition

$$\gcd(e,\lambda(N))=1, \qquad ed\equiv1\pmod{\lambda(N)}.$$

These are the validity conditions used for the public and private exponents in PKCS #1; see the [standards reference]({{ '/references/part-2/' | relative_url }}#standards). The distinction completes the progression we began earlier: $\operatorname{ord}_N(M)$ is the cycle length of one unit, $\lambda(N)$ is the smallest universal exponent for all units, and $\phi(N)$ is a universal exponent supplied directly by Euler's theorem.

---

## Formal RSA Construction

Having established the mathematical components, we can now state the pedagogical $\phi(N)$-based specification of the RSA algorithm. As noted above, the production specification may instead choose $d$ modulo $\lambda(N)$. The construction can be compared with the original RSA paper and the MIT RSA notes in the [foundational references]({{ '/references/part-2/' | relative_url }}#foundational-and-historical) and [number-theory references]({{ '/references/part-2/' | relative_url }}#number-theory).

### Algorithm Specification: RSA

#### Key Generation

1. Select two large prime numbers $p$ and $q$ ($p \ne q$).
2. Compute the composite modulus $N = p \cdot q$.
3. Compute the Euler totient value $\phi(N) = (p - 1)(q - 1)$.
4. Choose an encryption exponent $e$ such that $1 < e < \phi(N)$ and:

$$\gcd(e, \phi(N)) = 1$$


5. Compute the decryption exponent $d$, unique modulo $\phi(N)$, using the Extended Euclidean Algorithm:

$$d \equiv e^{-1} \pmod{\phi(N)}$$


6. **Public Key:** $(N, e)$
7. **Mathematical Private Exponent:** $d$ is sufficient to describe the basic decryption operation.
8. **Production Private-Key Material:** Implementations commonly retain $N$, $e$, $d$, $p$, $q$, $d_P=d \bmod (p-1)$, $d_Q=d \bmod (q-1)$, and $q_{\mathrm{inv}}=q^{-1}\bmod p$. These values enable CRT-optimized private-key operations and must all be protected as secret key material. This is the two-prime representation specified by [PKCS #1]({{ '/references/part-2/' | relative_url }}#standards).

#### Encryption

Given a plaintext message $M \in \{0, 1, \dots, N-1\}$ and public key $(N, e)$:

$$C \equiv M^e \pmod N$$

#### Decryption

Given a ciphertext $C$ and private key $d$:

$$M \equiv C^d \pmod N$$

---

## Why Exponentiation Preserves Congruence

Before proving RSA correct, we must justify a step that the proof will use. From

$$C \equiv M^e \pmod N,$$

may we conclude that

$$C^d \equiv (M^e)^d \equiv M^{ed} \pmod N?$$

### Proof: Congruence Survives Exponentiation

Suppose $a \equiv b \pmod N$. Then $a=b+kN$ for some integer $k$. For any positive integer $i$, the Binomial Theorem gives

$$a^i=(b+kN)^i=b^i+\sum_{j=1}^{i}\binom{i}{j}b^{i-j}k^jN^j.$$

Every term in the sum contains a factor of $N$, so $N\mid(a^i-b^i)$. Therefore,

$$a^i \equiv b^i \pmod N \quad \blacksquare$$

Thus, raising congruent values to the same positive integer power preserves congruence, and

$$C^d \equiv (M^e)^d \equiv M^{ed} \pmod N.$$

---

## Why RSA Works

RSA correctness requires

$$C^d \equiv (M^e)^d \equiv M^{ed} \equiv M \pmod N$$

for every message $M\in\mathbb{Z}/N\mathbb{Z}$.

### First Correctness Proof for Invertible Messages

Because $ed\equiv1\pmod{\phi(N)}$, there is an integer $k\ge0$ such that $ed=1+k\phi(N)$. If $\gcd(M,N)=1$, Euler's theorem applies:

$$M^{ed}=M^{1+k\phi(N)}\equiv M\left(M^{\phi(N)}\right)^k\equiv M\pmod N.$$

For every invertible message, decryption works exactly as required.

---

### But There Is a Problem

Euler's theorem requires $\gcd(M,N)=1$. RSA, however, does not get to reject a residue merely because it shares a factor with $N$. A message may satisfy $p\mid M$ or $q\mid M$.

> **Have we proved that RSA works, or only that it works for a convenient subset of messages?**

So far, we have proved only the second claim. To remove the restriction, we must use information that is special to the RSA modulus. Fortunately, $N$ is not arbitrary: we deliberately constructed it as $N=pq$. That lets us study the problem locally modulo $p$ and modulo $q$, where the moduli are prime, and then use the Chinese Remainder Theorem to put the results back together.

### Fermat's Little Theorem Appears Exactly When We Need It

We have reached the prime-modulus case of Euler's theorem. If the modulus is a prime $p$, every nonzero residue modulo $p$ is coprime to $p$, and

$$\phi(p)=p-1.$$

Substituting this into Euler's theorem gives **Fermat's Little Theorem**:

$$a^{p-1}\equiv1\pmod p \quad \text{when } p\nmid a.$$

Equivalently, for every residue $a$ modulo $p$, including $a\equiv0\pmod p$,

$$a^p\equiv a\pmod p.$$

Fermat's Little Theorem is not appearing as an unrelated historical result. We have already built it. Euler's theorem describes arbitrary moduli; when the modulus is prime, the totient becomes $p-1$, giving exactly the local result our RSA proof now needs. The relationship is covered by the [Part 2 number-theory sources]({{ '/references/part-2/' | relative_url }}#number-theory).

---

### Full Proof Using $p$, $q$, and the Chinese Remainder Theorem

In our pedagogical construction, $\phi(N)=(p-1)(q-1)$ and $ed\equiv1\pmod{\phi(N)}$. Therefore,

$$ed\equiv1\pmod{p-1} \quad \text{and} \quad ed\equiv1\pmod{q-1}.$$

The same two congruences hold for the PKCS #1 formulation using $ed\equiv1\pmod{\lambda(N)}$, because $\lambda(N)=\operatorname{lcm}(p-1,q-1)$ is divisible by both $p-1$ and $q-1$.

#### Evaluate Modulo $p$

We split the argument into two cases.

**Case 1: $p\nmid M$.** Fermat's Little Theorem gives $M^{p-1}\equiv1\pmod p$. Since $ed=1+k_p(p-1)$ for some integer $k_p\ge0$,

$$M^{ed}=M^{1+k_p(p-1)}\equiv M\left(M^{p-1}\right)^{k_p}\equiv M\pmod p.$$

**Case 2: $p\mid M$.** Then $M\equiv0\pmod p$. Because $ed>0$,

$$M^{ed}\equiv0\equiv M\pmod p.$$

Therefore, $M^{ed}\equiv M\pmod p$ in both cases.

#### Evaluate Modulo $q$

Again, split the argument into two cases.

**Case 1: $q\nmid M$.** Fermat's Little Theorem gives $M^{q-1}\equiv1\pmod q$. Since $ed=1+k_q(q-1)$ for some integer $k_q\ge0$,

$$M^{ed}=M^{1+k_q(q-1)}\equiv M\left(M^{q-1}\right)^{k_q}\equiv M\pmod q.$$

**Case 2: $q\mid M$.** Then $M\equiv0\pmod q$, and because $ed>0$,

$$M^{ed}\equiv0\equiv M\pmod q.$$

Therefore, $M^{ed}\equiv M\pmod q$ in both cases.

#### Combine the Results with the Chinese Remainder Theorem

We have shown that

$$p\mid(M^{ed}-M) \quad \text{and} \quad q\mid(M^{ed}-M).$$

Since $p$ and $q$ are distinct primes, they are coprime. Their product therefore divides the same difference:

$$pq\mid(M^{ed}-M).$$

With $N=pq$, this is exactly

$$M^{ed}\equiv M\pmod N.$$

Consequently,

$$C^d\equiv(M^e)^d\equiv M^{ed}\equiv M\pmod N$$

for **every** message $M\in\mathbb{Z}/N\mathbb{Z}$, whether or not $M$ is a unit. $\blacksquare$

---

## From Correctness to Performance

We now have a mathematically complete and proven asymmetric cryptosystem. However, when we transition from theoretical mathematics to practical software engineering, we run into a major performance bottleneck.

### The Asymmetry of Execution Time

In typical RSA implementations:

* The **public exponent** $e$ is usually chosen to be small and binary-friendly, most commonly $e = 65537 = 2^{16} + 1$.
* The **private exponent** $d$ is generally much larger than the public exponent, so a direct private-key exponentiation requires substantially more modular multiplication work.

Under the schoolbook multiplication model used in this chapter, exponentiation with an $n$-bit exponent modulo an $n$-bit modulus takes $O(n^3)$ bit operations. Exact timing ratios depend on the implementation, hardware, multiplication algorithm, and countermeasures in use, so no universal encryption-to-decryption ratio is claimed here.

---

## CRT as a Residue Fingerprint

Notice a fundamental asymmetry in what the two parties know:

* **The Public (Encryptor):** Knows only $N$ and $e$.
* **The Private Key Holder (Decryptor):** Knows $d$, but *also* knows the secret prime factors $p$ and $q$ such that $N = pq$.

Can we exploit our knowledge of $p$ and $q$ to accelerate decryption without changing the final output message $M$?

Yes. Instead of computing one massive exponentiation modulo $N$, we compute two much smaller exponentiations modulo $p$ and modulo $q$, then combine the results using the **Chinese Remainder Theorem (CRT)**.

---

When optimizing RSA decryption, we transitioned from computing operations modulo $N$ to computing them modulo the prime factors $p$ and $q$, and then reassembling the results.

This optimization relies entirely on the **Chinese Remainder Theorem (CRT)**. Rather than treating the CRT as a black-box algebraic tool, we will construct it from the ground up, starting with an intuitive look at modular "fingerprints" and moving to a full, rigorous proof of existence and uniqueness. Additional treatments are collected in the [CRT references]({{ '/references/part-2/' | relative_url }}#chinese-remainder-theorem).

---

### The Residue Fingerprint Analogy

Suppose we choose a composite modulus:

$$N = 105 = 3 \cdot 5 \cdot 7$$

Instead of manipulating a large integer $x = 23$ directly, let's examine its remainder (residue) when divided by each of the prime factors:

* $23 \equiv 2 \pmod 3$
* $23 \equiv 3 \pmod 5$
* $23 \equiv 2 \pmod 7$

We can represent the integer $23$ not as a single scalar value, but as a **residue fingerprint tuple**:

$$23 \implies (2, 3, 2) \quad \text{where each coordinate is evaluated } \pmod 3, \pmod 5, \text{ and } \pmod 7$$

### Does Information Get Lost?

At first glance, it feels like mapping a large integer into a tuple of smaller residues must discard information. After all, infinitely many integers share this exact same fingerprint.

Consider the sequence of integers:

$$23, \quad 128, \quad 233, \quad 338, \quad 443, \quad \dots$$

Every single one of these numbers produces the identical fingerprint $(2, 3, 2)$. Why? Because any two numbers in this sequence differ by a multiple of the least common multiple of the moduli, which for pairwise coprime factors $3, 5,$ and $7$ is their product:

$$\text{lcm}(3, 5, 7) = 3 \cdot 5 \cdot 7 = 105$$

Thus, the fingerprint does not identify an absolute integer; **it uniquely identifies an equivalence class modulo 105**.

This connects directly to our foundational discussion of equivalence classes: working inside the direct product ring $\mathbb{Z}_3 \times \mathbb{Z}_5 \times \mathbb{Z}_7$ is structurally isomorphic to working inside $\mathbb{Z}_{105}$.

---

## CRT Proof

### Formal Statement of the Chinese Remainder Theorem

To formalize this behavior for any system of moduli, we state the Chinese Remainder Theorem.

#### Theorem (The Chinese Remainder Theorem)

> Let $m_1, m_2, \dots, m_k$ be pairwise coprime positive integers (meaning $\gcd(m_i, m_j) = 1$ for all $i \ne j$).
> For any arbitrary integers $a_1, a_2, \dots, a_k$, the system of simultaneous linear congruences:
> $$> \begin{aligned} > x &\equiv a_1 \pmod{m_1} \\ > x &\equiv a_2 \pmod{m_2} \\ > &\;\;\vdots \\ > x &\equiv a_k \pmod{m_k} > \end{aligned} >$$
>
>
> has a **unique solution** modulo $N = m_1 m_2 \cdots m_k$.
> That is, there exists one and only one integer $x$ in the range $0 \le x < N$ that satisfies all congruences simultaneously.

---

### Proof of Existence (Constructive Proof)

To prove that *at least one* solution $x$ always exists, we can explicitly construct it using a generalized version of Lagrange's interpolation method.

#### Step-by-Step Construction

1. **Compute the Total Modulus:** Let $N = \prod_{i=1}^k m_i$.
2. **Compute Partial Moduli:** For each $i \in \{1, 2, \dots, k\}$, define $N_i$ as the product of all moduli *except* $m_i$:

$$N_i = \frac{N}{m_i} = m_1 m_2 \cdots m_{i-1} m_{i+1} \cdots m_k$$


3. **Isolate Coprime Pairs:** Because all original moduli $m_1, \dots, m_k$ are pairwise coprime, it follows that $N_i$ and $m_i$ are coprime:

$$\gcd(N_i, m_i) = 1$$


4. **Compute Modular Inverses:** Since $\gcd(N_i, m_i) = 1$, we can use the Extended Euclidean Algorithm (from Part 1) to find a modular inverse $e_i$ for $N_i$ modulo $m_i$:

$$e_i \equiv N_i^{-1} \pmod{m_i} \implies N_i e_i \equiv 1 \pmod{m_i}$$


5. **Construct Basis Terms:** Define basis polynomials $u_i = N_i e_i$. Notice how $u_i$ behaves under modular reduction:
* If we evaluate $u_i \pmod{m_i}$:

$$u_i \equiv N_i e_i \equiv 1 \pmod{m_i}$$


* If we evaluate $u_i \pmod{m_j}$ where $j \ne i$: Because $m_j$ is a factor of $N_i$, $N_i$ is a multiple of $m_j$, meaning:

$$u_i \equiv 0 \pmod{m_j}$$




6. **Synthesize the Solution $x$:** We construct our global solution $x$ by summing each target value $a_i$ weighted by its basis term $u_i$:

$$x = \sum_{i=1}^k a_i u_i = \sum_{i=1}^k a_i N_i e_i$$



#### Verifying the Constructed Solution

Let's test what happens to our constructed $x$ when reduced modulo any specific modulus $m_j$:

$$x \pmod{m_j} = \left( \sum_{i=1}^k a_i u_i \right) \pmod{m_j}$$

Because all terms where $i \ne j$ evaluate to $0 \pmod{m_j}$, only the $j$-th term survives:

$$x \pmod{m_j} \equiv a_j u_j \pmod{m_j} \equiv a_j \cdot 1 \equiv a_j \pmod{m_j}$$

Thus, our explicit construction $x = \sum a_i N_i e_i$ satisfies every single congruence in the system. **Existence is proven.**

---

### Proof of Uniqueness

We must now prove that this solution $x$ is **unique modulo $N$**, meaning that if there are any other integer solutions, they must differ from $x$ only by a multiple of $N$.

#### Proof by Contradiction

Suppose there exist two distinct integers $x$ and $y$ that both satisfy the entire system of congruences.

For every modulus $m_i$ (where $1 \le i \le k$):

$$x \equiv a_i \pmod{m_i} \quad \text{and} \quad y \equiv a_i \pmod{m_i}$$

By the transitive property of congruence:

$$x \equiv y \pmod{m_i} \implies m_i \mid (x - y)$$

This means that every single modulus $m_1, m_2, \dots, m_k$ divides the difference $(x - y)$.

Because all moduli $m_1, m_2, \dots, m_k$ are **pairwise coprime**, their least common multiple is their product $N$:

$$\text{lcm}(m_1, m_2, \dots, m_k) = m_1 m_2 \cdots m_k = N$$

A fundamental theorem of divisibility states that if a set of pairwise coprime integers each divide a number, their product must also divide that number:

$$N \mid (x - y)$$

Expressing this divisibility as a modular congruence:

$$x \equiv y \pmod N$$

Therefore, any two solutions $x$ and $y$ to the system must be identical modulo $N$. There is **exactly one unique solution** within the canonical range $0 \le x < N$. **Uniqueness is proven.**

---

### Application to RSA Decryption

Returning to our RSA decryption pipeline, the Chinese Remainder Theorem guarantees that when we compute:

$$M_p \equiv C^{d_P} \pmod p$$

$$M_q \equiv C^{d_Q} \pmod q$$

There exists one and only one value $M$ modulo $N = pq$ that produces the fingerprint $(M_p, M_q)$. CRT lets us obtain the two residues using exponentiations over roughly half-size moduli and then reconstruct that unique value. The reduced exponentiation work, not the reconstruction formula by itself, produces the approximate fourfold speedup derived below.

## RSA-CRT

Having established the Chinese Remainder Theorem, we can now apply it directly to solve our engineering problem: accelerating RSA decryption.

Instead of computing the high-precision operation $M \equiv C^d \pmod{pq}$ over a full $n$-bit modulus $N$, we split the computation into two independent sub-problems modulo the prime factors $p$ and $q$.

---

### Exponent Reduction Modulo $(p-1)$ and $(q-1)$

Define the positive CRT exponents

$$d_P \equiv d \pmod{p-1}, \qquad d_Q \equiv d \pmod{q-1}.$$

To justify the first reduction, write $d=d_P+k(p-1)$.

* If $p\nmid C$, Fermat's Little Theorem gives $C^{p-1}\equiv1\pmod p$, so

  $$C^d=C^{d_P+k(p-1)}\equiv C^{d_P}\left(C^{p-1}\right)^k\equiv C^{d_P}\pmod p.$$

* If $p\mid C$, both $C^d$ and $C^{d_P}$ are congruent to $0$ modulo $p$ because $d>0$ and $d_P>0$.

Thus, $C^d\equiv C^{d_P}\pmod p$ for every ciphertext residue $C$. The same two-case argument modulo $q$ gives

$$C^d\equiv C^{d_Q}\pmod q.$$

---

### The Optimized Decryption Pipeline

This reduces RSA decryption to three straightforward steps:

1. **Evaluate the $p$-share:**

$$M_p \equiv C^{d_P} \pmod p$$


2. **Evaluate the $q$-share:**

$$M_q \equiv C^{d_Q} \pmod q$$


3. **Reconstruct the full message $M$:**
   Find the unique $M \pmod N$ satisfying:

$$\begin{cases}    M \equiv M_p \pmod p \\    M \equiv M_q \pmod q    \end{cases}$$



Using the two-prime CRT reconstruction formula specified by [PKCS #1]({{ '/references/part-2/' | relative_url }}#standards), compute:

$$h = q_{\text{inv}} \cdot (M_p - M_q) \bmod p$$

$$M = M_q + h \cdot q$$

---

## Why CRT Is Approximately Four Times Faster

To understand the magnitude of this optimization, let me derive the speedup rigorously from the bit size of the operands rather than simply asserting a performance claim.

---

### The Cost of Standard Exponentiation

Let $n$ be the bit length of the RSA modulus $N = pq$. In direct RSA decryption, we evaluate $C^d \pmod N$, where:

* The modulus $N$ is $n$ bits long.
* The exponent $d$ is approximately $n$ bits long.

Under standard schoolbook multiplication and the square-and-multiply exponentiation model used here (see the [algorithms reference]({{ '/references/part-2/' | relative_url }}#algorithms-and-computation)):

1. **Multiplication Cost:** Multiplying two $k$-bit integers requires $O(k^2)$ bit operations.
2. **Exponentiation Cost:** Performing a Square-and-Multiply modular exponentiation with a $k$-bit exponent requires approximately $O(k)$ modular multiplications.

Combining these factors yields the total computational complexity for a $k$-bit exponentiation over a $k$-bit modulus:

$$\text{Complexity}(k) = O(k) \times O(k^2) = O(k^3)$$

For direct RSA decryption ($k = n$), the operation count scales as:

$$\text{Cost}_{\text{Direct}} = O(n^3)$$

---

### The Cost of CRT-Optimized Exponentiation

When $N = pq$, the two prime factors $p$ and $q$ are roughly equal in size, meaning each prime has a bit length of approximately $\frac{n}{2}$.

Under CRT decryption, we perform two separate exponentiations ($M_p$ and $M_q$). For each exponentiation:

* The modulus ($p$ or $q$) has a length of $\frac{n}{2}$ bits.
* The reduced exponent ($d_P$ or $d_Q$) has a length of at most $\frac{n}{2}$ bits.

Let's evaluate the work required for a single share using $k = \frac{n}{2}$:

$$\text{Cost}(M_p) = O\left( \left( \frac{n}{2} \right)^3 \right) = O\left( \frac{n^3}{8} \right) = \frac{1}{8} O(n^3)$$

Since we must evaluate two such shares ($M_p$ and $M_q$), the total exponentiation work is:

$$\text{Cost}_{\text{CRT}} = \text{Cost}(M_p) + \text{Cost}(M_q) = \frac{1}{8} O(n^3) + \frac{1}{8} O(n^3) = \frac{1}{4} O(n^3)$$

```
                         COMPUTATIONAL WORK COMPARISON
  
  Direct Decryption:
  [ n-bit Exponentiation over n-bit Modulus ]
  Complexity: O(n³) ═════════════════════════════════════════════════► Baseline (100%)

  CRT Decryption:
  ├─ Share 1 (Mp): (n/2)-bit exponentiation ──► Cost: (1/8) O(n³)
  ├─ Share 2 (Mq): (n/2)-bit exponentiation ──► Cost: (1/8) O(n³)
  └─ CRT Reconstruction                     ──► Cost: O(n²), lower-order overhead
  Total Work: (1/4) O(n³) ═══════════════════════════════════════════► Speedup: ~4x

```

---

### A Rigorous Nuance: Constant Factors vs. Asymptotic Class

It is crucial to state this distinction precisely: **CRT does not change the asymptotic complexity class of RSA exponentiation.**

Under schoolbook multiplication, CRT decryption remains $O(n^3)$. In this model, decomposing the modulus and performing two half-size exponentiations reduces the dominant exponentiation work to roughly one quarter of the direct computation. CRT reconstruction adds comparatively smaller $O(n^2)$ overhead.

The $\sim 4\times$ figure is a model-derived approximation, not a universal benchmark. Real implementations use different multiplication algorithms and incur additional recombination, validation, and side-channel-protection costs.

---

## Summary: The Integrated Architecture of RSA

At this point, we have completed the core mathematical and operational engine of RSA. Everything required to understand why RSA decrypts correctly and how it executes efficiently has been derived.

We can view the complete architecture across three distinct layers:

```
┌─────────────────────────────────────────────────────────────────────────────┐
│ LAYER 1: PURE NUMBER THEORY                                                 │
│ Equivalence Classes ──► Modular Inverses ──► Euler's Totient ──► EEA ──► CRT│
└──────────────────────────────────────┬──────────────────────────────────────┘
                                       │
                                       ▼
┌─────────────────────────────────────────────────────────────────────────────┐
│ LAYER 2: CRYPTOGRAPHIC CONSTRUCTION                                         │
│ Key Gen: d ≡ e⁻¹ (mod φ(N)) ──► Enc: C ≡ Mᵉ (mod N) ──► Dec: M ≡ Cᵈ (mod N) │
└──────────────────────────────────────┬──────────────────────────────────────┘
                                       │
                                       ▼
┌─────────────────────────────────────────────────────────────────────────────┐
│ LAYER 3: SOFTWARE & PERFORMANCE ENGINEERING                                 │
│ Modular Exponentiation Bottleneck ──► CRT Decomposition ──► ~4x Acceleration │
└─────────────────────────────────────────────────────────────────────────────┘

```

---

## RSA Correctness Is Not RSA Security

We started with nothing more than a structural requirement: construct a function that can be evaluated publicly but reversed only with secret information.

That requirement led us from linear multiplication to exponentiation, from exponentiation to cyclic behavior, from cyclic behavior to Euler's theorem, and from Euler's theorem to the modular inverse that gives us RSA's private exponent via the Extended Euclidean Algorithm.

We then discovered that mathematical correctness was only half of the engineering problem. The same algebraic structure that makes RSA work also provides the key to making it fast: because the private key owner holds the prime factorization of $N$, we can use the Chinese Remainder Theorem to decompose one massive computation into two significantly smaller ones.

But none of this has answered the most critical question.

We have proven that the legitimate recipient holding $d$ can recover $M$. We have **not** proven that an eavesdropper, given only the public key $(N,e)$ and the ciphertext $C$, cannot do the same.

**RSA correctness is not RSA security.**

RSA security is expressed through computational assumptions related to problems such as RSA inversion and integer factorization. Factoring $N$ is sufficient to recover the private exponent in this construction, but the correctness proof does not prove that factoring is necessary, nor does it prove an exponential lower bound for any attack. It establishes only that the authorized decryption computation returns the original message.

That distinction is where pure mathematics transforms into cryptanalysis, and it is where we will begin **Part 3**.

---

The complete bibliography supporting this article is available in the [Part 2 references]({{ '/references/part-2/' | relative_url }}).
