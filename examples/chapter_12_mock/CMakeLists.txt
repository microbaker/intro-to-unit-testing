cmake_minimum_required(VERSION 3.14)
project(chapter_12_mock LANGUAGES CXX)

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

add_executable(mock_tests test/test_mock.cpp)
target_link_libraries(mock_tests PRIVATE GTest::gmock_main)

include(GoogleTest)
gtest_discover_tests(mock_tests)
