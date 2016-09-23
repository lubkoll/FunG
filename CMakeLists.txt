
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(fung)

enable_language(C CXX)

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake
    )

include(ConfigSafeGuards)

set(CMAKE_BUILD_TYPE Debug)

enable_testing()
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  add_subdirectory(googletest-release-1.8.0)
  include_directories(${gtest_SOURCE_DIR}/include)
  link_directories(${PROJECT_SOURCE_DIR}/build/googletest-release-1.8.0/googlemock/gtest)
  set(GTEST_LIBRARIES gtest_main gtest)
#  find_package(GTest REQUIRED 
#               HINTS $ENV{GTEST_DIR})
#  include_directories( ${GTEST_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR} )
#  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wno-deprecated-declarations")
#  set(GTEST_LIBS ${CONAN_LIBS})
else(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  set(CMAKE_CXX_STANDARD 14)
  find_package(GTest REQUIRED)
  include_directories( ${GTEST_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR} )
  set(CMAKE_CXX_FLAGS "-Wall -Wwrite-strings -Wpedantic -Woverloaded-virtual -Wno-unknown-pragmas -Wno-sign-compare -Wno-unused ${CMAKE_CXX_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")


find_package(Eigen3)
if(EIGEN3_FOUND)
  include_directories(${EIGEN3_INCLUDE_DIR})

# On travis the examples do not compile due to a strange ambiguity with functionality of Eigen:
#  aux_source_directory(test/examples SRC_LIST)
  aux_source_directory(test/linear_algebra SRC_LIST)
endif()

include_directories(.)
aux_source_directory(test/cmath SRC_LIST)
aux_source_directory(test/fung SRC_LIST)
aux_source_directory(test/mathematical_operations SRC_LIST)

add_executable(
    unit_tests
    test/test.cpp
    ${SRC_LIST}
    )
    
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  target_link_libraries(unit_tests ${GTEST_LIBRARIES})
else(${CMAKE_SYSTEM_NAME} MATCHES "Windows") 
  target_link_libraries(unit_tests ${GTEST_LIBRARIES} pthread)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
include(CTest)
enable_testing()

add_test(fung_unit_test ${PROJECT_BINARY_DIR}/unit_tests)
