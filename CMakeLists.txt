cmake_minimum_required(VERSION 2.8)
project(SkipList)

include_directories(.)

set(
    LIB
    pthread
)

add_executable(test_stl tests/test_stl.cc)

add_executable(test tests/test.cc)
target_link_libraries(test ${LIB})

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)