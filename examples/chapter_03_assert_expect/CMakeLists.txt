cmake_minimum_required(VERSION 3.14)
project(chapter_03_assert_expect LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_library(string_utils src/string_utils.cpp)
target_include_directories(string_utils PUBLIC include)

add_executable(assert_expect_tests test/test_assert_expect.cpp)
target_link_libraries(assert_expect_tests PRIVATE string_utils GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(assert_expect_tests)
