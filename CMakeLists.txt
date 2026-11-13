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