# Chapter 13: Case Studies and Real-world Examples

Real projects show both the value and the limits of unit testing. The cases below illustrate strategies that worked, pitfalls that appeared, and how teams adapted Google Test without treating any single technique as sufficient.

## 13.1 Analyzing Unit Testing Strategies in Real Projects

**Chromium / Google Test itself.** Large C++ code bases such as Chromium rely on Google Test for unit and component tests. Tests are organized by component, run under continuous integration with sanitizers, and frequently use fixtures and parameterized tests. Coverage is tracked but not treated as an absolute gate.

**A typical service-refactoring story.** A team extracting a pricing module introduced an interface for the tax service, wrote Google Mock tests for the pricing rules, and kept the original integration tests. Unit tests caught several boundary errors before the extraction was complete.

**Open-source libraries.** Many C++ libraries publish Google Test suites that emphasize parameterized tests for input matrices and death tests for precondition violations.

## 13.2 Learning from Successes and Challenges

**Success patterns**

- Tests written in the same change as the production code stayed aligned.
- Parameterized tests and small fixtures kept suites readable.
- Sanitizer-enabled CI builds surfaced memory issues that pure assertions missed.
- Public-interface tests survived internal refactors.

**Challenge patterns**

- Legacy modules with heavy internal coupling resisted unit testing until a narrow interface was extracted.
- Suites that depended on execution order or global state became flaky.
- Over-mocking of internal calls produced brittle tests.
- Coverage targets without assertion quality led to tests that verified little.

## 13.3 Adapting Strategies to Your Own Projects

Compare a case study with your code base along three axes: dependency structure, risk profile, and existing test maturity. Adopt only the practices that address a real gap. Document the adaptation so later maintainers understand why a given style was chosen.

## 13.4 Navigating Trade-offs and Decision-Making

Common trade-offs include:

- Breadth versus speed.
- Isolation versus realism.
- Public-only versus selective white-box.
- Example-based versus property-based.

Decide explicitly, record the rationale, and revisit when the code or risk profile changes.

## 13.5 Inspirational Stories of Testing Impact

Teams repeatedly report that the first time a unit test failed in CI and prevented a broken change from merging, the cost of the testing investment became obvious. In one library refactor, parameterized boundary tests revealed an off-by-one error that had existed for years. The common thread is feedback speed: defects found minutes after introduction are cheap to fix.

## Chapter 13 Summary

Real-world suites succeed when they stay fast, focused on observable behavior, and integrated into everyday workflow and CI. Challenges are addressed incrementally by improving testability, removing shared state, and preferring stable public contracts. Adapt the techniques to your dependency structure and risk profile.

## Questions and Exercises

1. Identify two practices from the case studies you could apply to a module you currently maintain.
2. A legacy class opens a real network connection inside its constructor. Describe a minimal seam and the first test you would write.
3. Why did tests that reached private helpers tend to break during refactors?
4. List one situation in which you would accept a slower integration test in addition to unit tests.
5. True or false: Successful projects achieved high reliability from unit tests alone. Explain.
