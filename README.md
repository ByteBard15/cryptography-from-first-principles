# Cryptography From First Principles

A structured research site that develops cryptographic machinery from elementary number theory and small, testable C++ implementations.

This repository currently contains **Part 1 — The Number Theory Engine**. Published explanations live in `_part1/`; full proofs, code, experiments, and references are maintained separately.

## Read locally

```bash
bundle install
bundle exec jekyll serve
```

Then open `http://localhost:4000/cryptography-from-first-principles/`.

For deployment, set `url` in `_config.yml` to the GitHub Pages origin. The checked-in `baseurl` assumes the repository is named `cryptography-from-first-principles`; change it if the repository name differs. In repository settings, select GitHub Pages deployment from the publishing branch, or add a Pages Actions workflow later.

## Test the C++ code

```bash
cmake -S code/part-1 -B build/part-1
cmake --build build/part-1
ctest --test-dir build/part-1 --output-on-failure
```

The implementations use only the C++ standard library. They are educational demonstrations, not hardened cryptographic primitives.

The supplied Hashnode draft remains under the ignored `tmp/` migration workspace. Its Go snippets were identified during conversion and replaced by the C++ examples and algorithms above; no non-C++ implementation is published by the site.
