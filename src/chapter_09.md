# Chapter 9: Test Coverage and Continuous Integration

Code coverage measures which parts of the production code are exercised by tests. Continuous integration runs those tests (and the coverage analysis) automatically on every change.

## 9.1 Understanding Code Coverage Metrics

Common metrics include:

- **Line coverage**: percentage of executable lines executed at least once.
- **Branch coverage**: percentage of decision outcomes taken.
- **Function coverage**: percentage of functions entered.

Coverage is a useful signal, not a goal. High coverage with weak assertions gives false confidence.

## 9.2 Integrating Unit Tests into Continuous Integration Pipelines

For GCC/Clang the instrumentation flags are `--coverage`.

**Minimal CMake addition:**

```cmake
option(ENABLE_COVERAGE "Enable coverage reporting" OFF)
if(ENABLE_COVERAGE)
  target_compile_options(math_operations PRIVATE --coverage)
  target_link_options(math_operations PRIVATE --coverage)
endif()
```

Generate a report:

```bash
cmake -DENABLE_COVERAGE=ON -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
ctest --output-on-failure
lcov --capture --directory . --output-file coverage.info
lcov --remove coverage.info '/usr/*' --output-file coverage.info
genhtml coverage.info --output-directory coverage_html
```

**Representative CI fragment:**

```yaml
- name: Configure and build with coverage
  run: |
    cmake -DENABLE_COVERAGE=ON -DCMAKE_BUILD_TYPE=Debug -B build
    cmake --build build
- name: Run tests
  run: ctest --test-dir build --output-on-failure
- name: Generate coverage report
  run: |
    lcov --capture --directory build --output-file coverage.info
```

## 9.3 Interpreting Coverage Reports for Quality Assessment

Look for untested error-handling branches and complex functions with low coverage. Ignore generated code and third-party headers. Combine coverage runs with sanitizers in at least one CI configuration.

## 9.4 Tracking Code Coverage Trends Over Time

Record coverage of changed lines for each build. Prefer “coverage of new or modified code must not decrease” over rigid global percentage gates.

## 9.5 Case Study: Boosting Code Quality with Coverage and CI

A small math library starts with basic tests. Coverage reveals untested negative-input paths. Adding parameterized cases raises branch coverage. CI runs the tests under normal and AddressSanitizer builds and posts the coverage delta.

## Common Pitfalls

- Treating a coverage percentage as a quality score.
- Excluding hard-to-test files instead of improving their design.
- Running coverage only locally.
- Instrumenting optimized builds.
- Forgetting to exclude system headers.

## Chapter 9 Summary

Coverage metrics reveal which lines and branches the tests execute. Integrate instrumented builds into continuous integration. Interpret reports by focusing on critical paths and changed code, and combine coverage with sanitizers.

## Questions and Exercises

1. What is the difference between line coverage and branch coverage?
2. Why should coverage instrumentation normally be applied to a Debug build?
3. A pull request adds an untested error-handling path. What action should the author take?
4. True or false: 100 % line coverage guarantees the absence of defects. Explain.
5. Why is it useful to run at least one CI configuration with both coverage and AddressSanitizer?
