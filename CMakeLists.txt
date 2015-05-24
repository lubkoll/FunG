
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(rffgen)

enable_language(C CXX)

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake
    )

include(ConfigSafeGuards)
include(Coveralls)
coveralls_turn_on_coverage()
if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
    set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -std=c++1y")
#    set(CMAKE_CXX_LINK_FLAGS "-fprofile-arcs -ftest-coverage")
endif()

include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/external/googletest
    ${PROJECT_SOURCE_DIR}/external/googletest/include
    )

add_custom_target(
    git_update
    COMMAND git submodule init
    COMMAND git submodule update
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    )

add_library(
    gtest
    ${PROJECT_SOURCE_DIR}/external/googletest/src/gtest-all.cc
    ${PROJECT_SOURCE_DIR}/external/googletest/src/gtest_main.cc
    )

add_dependencies(gtest git_update)

set_source_files_properties(${PROJECT_SOURCE_DIR}/external/googletest/src/gtest-all.cc  PROPERTIES GENERATED 1)
set_source_files_properties(${PROJECT_SOURCE_DIR}/external/googletest/src/gtest_main.cc PROPERTIES GENERATED 1)

aux_source_directory(test SRC_LIST)
add_subdirectory(test/CMath)
#aux_source_directory(test/CMath SRC_LIST)

set(COVERAGE_SRCS test/test.cpp test/CMath/arcsine_unittest.cpp test/CMath/sine_unittest.cpp test/CMath/cosine_unittest.cpp test/CMath/pow_unittest.cpp test/CMath/log_unittest.cpp test/CMath/exp_unittest.cpp test/CMath/tan_unittest.cpp)

coveralls_setup( "${COVERAGE_SRCS}" ON "${PROJECT_SOURCE_DIR}/CMakeModules")

add_executable(
    unit_tests
    ${SRC_LIST}
    )

add_dependencies(unit_tests gtest)

target_link_libraries(
    unit_tests
    gtest
    pthread
    gcov
    )
 
#SETUP_TARGET_FOR_COVERAGE(unit_tests_coverage unit_tests coverage)
#include(CTest)
enable_testing()

add_test(unit ${PROJECT_BINARY_DIR}/unit_tests)
