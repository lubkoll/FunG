
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

enable_testing()
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  find_package(GTest REQUIRED)
  include_directories( ${GTEST_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR} )
  set(GTEST_LIBS ${GTEST_LIBRARIES})
else(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  include(conanbuildinfo.cmake)
  conan_basic_setup()
  set(GTEST_LIBS ${CONAN_LIBS})
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")


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
    
target_link_libraries(
    unit_tests
    ${GTEST_LIBS}
    pthread
    )
 
include(CTest)
enable_testing()

add_test(fung_unit_test ${PROJECT_BINARY_DIR}/unit_tests)
