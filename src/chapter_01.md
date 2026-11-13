# Chapter 1: Introduction to Unit Testing and Google Test

Unit testing is the practice of testing individual units or components of code in isolation to verify their correctness. A unit is typically a function, method, or small class. Done well, unit tests form an early and effective line of defense against defects, support refactoring, and give developers rapid feedback.

This chapter explains why unit testing matters, introduces the Google Test framework (gtest), and provides a complete, buildable starter project so you can write and run your first tests immediately.

## 1.1 Understanding the Importance of Unit Testing

### 1.1.1 Defining unit testing and its role

Unit testing verifies that a small piece of code behaves as specified when exercised independently of the rest of the system. Tests typically call the unit with controlled inputs and assert expected outputs, state changes, or error conditions. Because the unit is isolated, failures are usually easy to localize.

Unit tests are most effective when the code under test is designed for testability (small functions, clear interfaces, limited hidden state, and injectable dependencies). They do not replace integration, system, or acceptance testing; they complement them by catching many logic and boundary errors early.

### 1.1.2 Benefits of unit testing

Well-written unit tests help:

- Catch logic errors, off-by-one mistakes, incorrect boundary handling, and simple regressions while the code is still fresh in the developer’s mind.
- Reduce the time spent debugging by providing a fast, repeatable signal that a change broke existing behavior.
- Document intended behavior in executable form.
- Enable safer refactoring and evolution of the code.
- Support continuous integration by giving automated, fast feedback on every change.

Industry studies consistently show that defects cost far more to fix the later they are discovered. The classic NIST Planning Report 02-3 (2002) estimated the annual U.S. economic cost of inadequate software testing infrastructure at roughly $59.5 billion, with a large share borne by users. Cost-of-change data from Boehm and others indicate that fixing a defect found in production can cost tens to hundreds of times more than fixing the same class of defect during design or early coding. Exact multipliers vary by domain and severity, but the direction is clear: earlier detection is cheaper.

Unit testing is one practical way to shift detection left. It is not a silver bullet—requirements misunderstandings, integration issues, concurrency problems, and environmental failures still require other techniques—but it is one of the highest-leverage practices available to individual developers and teams.

### 1.1.3 Real-world examples and what they actually illustrate

Several well-known failures are often cited in discussions of testing. Careful reading of the official investigations shows that unit testing of individual functions would not, by itself, have guaranteed prevention. The incidents highlight the need for clear requirements, appropriate testing at multiple levels, validation of reused components under new conditions, and complementary practices (system testing, reviews, hardware safeguards, etc.).

**Ariane 5 Flight 501 (1996).** The maiden flight was lost when an unhandled conversion of a 64-bit floating-point value to a 16-bit signed integer overflowed inside the inertial reference system. The software had been reused from Ariane 4; the conversion was unprotected, and the alignment function continued to run after lift-off under trajectory conditions that differed from those of Ariane 4. The inquiry board identified specification and design errors plus inadequate testing of the reused software under Ariane 5 flight conditions. Thorough unit tests that exercised the conversion with values outside the assumed range could have flagged the unprotected operation *if* the test data reflected the actual operating envelope. The deeper issues were incomplete requirements for the new vehicle and insufficient validation of reused code.

**Mars Pathfinder (1997).** After landing, the spacecraft experienced repeated system resets. The root cause was priority inversion in the VxWorks real-time operating system. This is a classic real-time scheduling and resource-sharing issue. Isolated unit tests of individual application functions would not have revealed it.

**Therac-25 radiation therapy accidents (1985–1987).** Several patients received massive overdoses. Contributing factors included race conditions in the control software, removal of earlier hardware interlocks, and inadequate software engineering practices. Unit tests of individual routines could have exercised some of the mode-logic paths, but they would not have substituted for system-level testing or independent safety analysis.

The practical lesson is consistent: write focused unit tests for logic, boundaries, and error paths; combine them with higher-level tests and other quality practices; and treat reused or safety-critical code with extra scrutiny of its assumptions.

## 1.2 Introducing the Google Test Framework

Google Test (gtest) is a widely used open-source C++ testing framework originally developed at Google. It provides a straightforward way to write, organize, and run unit tests.

Key capabilities include:

- Test suites and test cases (`TEST`, `TEST_F`)
- Assertions (`ASSERT_*` and `EXPECT_*`)
- Test fixtures
- Parameterized and typed tests
- Death tests
- Rich failure reporting
- Straightforward integration with CMake and CI

## 1.3 Setting Up the Development Environment

The following minimal project uses CMake’s `FetchContent` to obtain Google Test.

**include/math_operations.h**

```cpp
#ifndef MATH_OPERATIONS_H
#define MATH_OPERATIONS_H

namespace math {

int add(int a, int b);
int subtract(int a, int b);

}  // namespace math

#endif
```

**src/math_operations.cpp**

```cpp
#include "math_operations.h"

namespace math {

int add(int a, int b) { return a + b; }

int subtract(int a, int b) { return a - b; }

}  // namespace math
```

**test/test_math_operations.cpp**

```cpp
#include <gtest/gtest.h>
#include "math_operations.h"

TEST(MathOperationsTest, AddPositiveNumbers) {
  EXPECT_EQ(math::add(2, 3), 5);
  EXPECT_EQ(math::add(0, 0), 0);
}

TEST(MathOperationsTest, AddNegativeNumbers) {
  EXPECT_EQ(math::add(-1, -1), -2);
  EXPECT_EQ(math::add(-5, 3), -2);
}

TEST(MathOperationsTest, Subtract) {
  EXPECT_EQ(math::subtract(10, 4), 6);
  EXPECT_EQ(math::subtract(0, 5), -5);
}
```

**CMakeLists.txt**

```cmake
cmake_minimum_required(VERSION 3.14)
project(math_example LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_library(math_operations src/math_operations.cpp)
target_include_directories(math_operations PUBLIC include)

add_executable(math_tests test/test_math_operations.cpp)
target_link_libraries(math_tests PRIVATE math_operations GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(math_tests)
```

Build and run:

```bash
mkdir build && cd build
cmake ..
cmake --build .
ctest --output-on-failure
```

## Chapter 1 Summary

Unit testing isolates small units of code to verify behavior early. Google Test supplies the macros, fixtures, and reporting needed to do this effectively in C++. A minimal CMake + FetchContent project lets you start immediately.

## Questions and Exercises

1. What is the primary goal of a unit test, and what does “in isolation” mean in practice?
2. Why can a defect found in production cost dramatically more to fix than the same defect found during unit testing?
3. Build the starter project. Add a `multiply` function and at least three tests for it.
4. True or false: Google Test’s `ASSERT_*` macros abort the entire test binary when they fail. Explain.
