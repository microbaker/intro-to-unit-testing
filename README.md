# Intro to Unit Testing for Test-Driven Development

Theory & Applications with Google Test

A practical guide for intermediate C++ developers who want to write solid, maintainable unit tests with the Google Test framework.

## Read the book

The rendered book is available at:  
**https://microbaker.github.io/intro-to-unit-testing/**

(Or clone the repo and run `mdbook serve` locally.)

## What’s inside

- Complete, copy-pasteable examples (CMake + FetchContent starter included)
- Fixtures, parameterized tests, typed tests, and death tests
- Coverage measurement and continuous-integration patterns
- Practical guidance on testing private code, security-relevant logic, and best practices
- Scaffolded final project

## Building the book locally

```bash
# Install mdBook (one time)
cargo install mdbook

# Or download a binary from https://github.com/rust-lang/mdBook/releases

# Serve with live reload
mdbook serve

# Or just build
mdbook build
# Output is in ./book
