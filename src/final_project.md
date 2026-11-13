# Final Project: Comprehensive Unit Test Suite

Apply the practices from the entire book to a small but complete C++ component.

## Suggested Component

A `TemperatureConverter` class with methods `celsius_to_fahrenheit`, `fahrenheit_to_celsius`, and a validation helper that rejects NaN and out-of-range inputs. Or correct and test the six-degree-of-freedom physics sketch from earlier drafts. Or choose a component of similar scope from your own work.

## Required Deliverables

1. `CMakeLists.txt` that fetches Google Test (as in Chapter 1) and builds both the library and the test executable.
2. Header and source for the component, designed with testability in mind.
3. A test file containing:
   - Ordinary tests for core happy-path values.
   - At least one parameterized test (`TEST_P` / `INSTANTIATE_TEST_SUITE_P`) covering a range of inputs including boundaries.
   - At least one fixture if shared setup appears.
   - Appropriate use of `EXPECT` versus `ASSERT`.
   - Custom failure messages on at least two assertions.
   - An exception or death test if the component has specified failure modes.
   - Clear, consistent naming and formatting.
4. A short README section listing the exact commands to configure, build, run tests, and (optionally) generate a coverage report.
5. A brief commentary (as comments or a separate note) explaining one design choice that improved testability and one trade-off you accepted.

## Partially Worked Checklist

- [ ] Project builds with the Chapter 1 CMake pattern.
- [ ] All tests pass under `ctest --output-on-failure`.
- [ ] `--gtest_filter` can select a meaningful subset.
- [ ] At least one deliberate failure produces a message that includes input values.
- [ ] Coverage report (if enabled) shows the main branches exercised.
- [ ] No shared mutable state; tests are independent.
- [ ] Code and tests follow the formatting standards of Chapter 2.

## Optional Extensions

- Add a Google Mock test if you introduce an interface.
- Enable AddressSanitizer in a second build configuration.
- Include a property-style loop or note how RapidCheck could be added later.

Completing the project demonstrates that you can set up, organize, and maintain a realistic Google Test suite that applies the techniques developed throughout the book.
