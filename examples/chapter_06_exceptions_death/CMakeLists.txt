cmake_minimum_required(VERSION 3.14)
project(chapter_06_exceptions_death LANGUAGES CXX)

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

add_executable(exception_death_tests test/test_exceptions_death.cpp)
target_link_libraries(exception_death_tests PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(exception_death_tests)
