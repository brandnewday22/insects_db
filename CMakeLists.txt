cmake_minimum_required(VERSION 3.14)

project(2Q_cache)

set(CMAKE_CXX_FLAGS "-std=c++20 -O2 -g")

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing() 

add_executable( 
  TESTS
  src/tests.cpp
)

add_executable(
  MAIN
  src/main.cpp
)

add_executable(
  TEST_GEN
  src/test_gen.cpp
)

add_executable(
  IDEAL_CACHE
  src/ideal_cache.cpp
)

target_include_directories(IDEAL_CACHE PUBLIC include/)
target_include_directories(TESTS PUBLIC include/)
target_include_directories(MAIN PUBLIC include/)

target_link_libraries(
  TESTS
  GTest::gtest_main 
)

include(GoogleTest)
gtest_discover_tests(TESTS)
