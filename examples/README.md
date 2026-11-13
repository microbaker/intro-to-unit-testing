# Examples

Self-contained, compilable projects that accompany the book chapters.

Each subdirectory is an independent CMake project. To build any of them:

```bash
cd examples/chapter_XX_name
mkdir build && cd build
cmake ..
cmake --build .
ctest --output-on-failure
```

Or run the test binary directly.

## Included examples

- `chapter_01_starter` — Basic library + tests (Chapter 1)
- `chapter_03_assert_expect` — ASSERT vs EXPECT and preconditions
- `chapter_05_parameterized` — Value-parameterized and typed tests
- `chapter_06_exceptions_death` — Exceptions and death tests
- `chapter_08_fixtures` — Test fixtures with SetUp/TearDown
- `chapter_10_security` — Validation and bounds-checking tests
- `chapter_12_mock` — Google Mock example

All examples use CMake FetchContent to obtain Google Test, so no system-wide installation is required.
