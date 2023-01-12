cmake_minimum_required(VERSION 2.8)
project(SkipList)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -ggdb -std=c++11 -Wno-unused-function")

include_directories(.)

set(
    LIB
    pthread
)

add_executable(test_stl tests/test_stl.cc)

add_executable(test tests/test.cc)
target_link_libraries(test ${LIB})

add_executable(stress_test tests/stress_test.cc)
target_link_libraries(stress_test ${LIB})

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)