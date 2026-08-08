---
title: Part 1 Experiments
permalink: /experiments/part-1/
kind: Supplementary experimental material
---

# Part 1 Experiments

The performance claims in Part 1 depend on the CPU, compiler, optimization flags, input distribution, and integer representation. Treat them as hypotheses to measure rather than universal conclusions.

## Reproducible baseline

`gcd_benchmark.cpp` compares the educational 64-bit Euclidean and binary GCD implementations over the same deterministic pseudo-random inputs.

```bash
c++ -std=c++20 -O3 experiments/part-1/gcd_benchmark.cpp -o /tmp/gcd_benchmark
/tmp/gcd_benchmark
```

Record the compiler and version, flags, target CPU, operating system, sample count, input distribution, and raw timing output when publishing results. Do not generalize fixed-width results to multi-precision arithmetic.

## Pending experiments

- Compare Fibonacci-shaped inputs with uniformly distributed pairs.
- Inspect generated code to identify the selected trailing-zero instruction.
- Add a separately reviewed multi-precision implementation before measuring BigInt claims.
- Report timing distributions across repeated trials rather than a single best run.
