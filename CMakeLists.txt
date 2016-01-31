
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(fung)

enable_language(C CXX)

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake
    )

include(ConfigSafeGuards)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS         "-Wpedantic -Wall -Wno-unknown-pragmas -Wno-sign-compare -Woverloaded-virtual -Wwrite-strings -Wno-unused")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage -std=c++1y")

include_directories(
    ${PROJECT_SOURCE_DIR}
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

find_package(Eigen3)
if(EIGEN3_FOUND)
  include_directories(${EIGEN3_INCLUDE_DIR})

# On travis the examples do not compile due to a strange ambiguity with functionality of Eigen:
#  aux_source_directory(test/examples SRC_LIST)
  aux_source_directory(test/linear_algebra SRC_LIST)
endif()

aux_source_directory(test/cmath SRC_LIST)
aux_source_directory(test/fung SRC_LIST)
aux_source_directory(test/mathematical_operations SRC_LIST)

add_executable(
    unit_tests
    test/test.cpp
    ${SRC_LIST}
    )
    
add_dependencies(unit_tests gtest)

target_link_libraries(
    unit_tests
    gtest
    pthread
    )
 
include(CTest)
enable_testing()

add_test(fung_unit_test ${PROJECT_BINARY_DIR}/unit_tests)
