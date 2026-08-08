---
title: Bézout’s Identity
kind: Proof from source
---

# Bézout’s Identity

For nonzero integers $a$ and $b$, there exist $x,y\in\mathbb{Z}$ such that

$$
ax+by=\gcd(a,b).
$$

Consider the set of positive linear combinations

$$
S=\{as+bt\mid s,t\in\mathbb{Z},\ as+bt>0\}.
$$

The set is nonempty: since $a\ne0$, either $a$ or $-a$ is positive and is an integer linear combination of $a$ and $b$. By the well-ordering principle, $S$ has a smallest element. Write it as

$$
d=ax_0+by_0.
$$

Divide $a$ by $d$:

$$
a=qd+r,\qquad0\le r<d.
$$

Then

$$
r=a-qd=a(1-qx_0)+b(-qy_0).
$$

If $r>0$, then $r\in S$, contradicting the minimality of $d$. Thus $r=0$ and $d\mid a$. The same argument gives $d\mid b$, so $d$ is a common divisor.

If $c$ is any common divisor of $a$ and $b$, then it divides every linear combination, including $d=ax_0+by_0$. Therefore every common divisor divides $d$, and $d=\gcd(a,b)$. Hence

$$
\gcd(a,b)=ax_0+by_0.
$$

The source cites Shoup (2008), Theorem 1.8, and Hoffstein et al. (2008), Section 1.2; both citation details remain marked for verification in the reference list.
