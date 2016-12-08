cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(fung)

set (FUNG_MAJOR_VERSION 1)
set (FUNG_MINOR_VERSION 4)
set (FUNG_PATCH_VERSION 0)
set (FUNG_VERSION ${FUNG_MAJOR_VERSION}.${FUNG_MINOR_VERSION}.${FUNG_PATCH_VERSION})

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake
    )

include(ConfigSafeGuards)


include(GenerateExportHeader)

file(GLOB_RECURSE Headers *.hh)
MACRO(INSTALL_HEADERS_WITH_DIRECTORY HEADER_LIST)

FOREACH(HEADER ${${HEADER_LIST}})
STRING(REGEX MATCH "${CMAKE_CURRENT_SOURCE_DIR}[/\](.*)[/\]" DIR ${HEADER})
STRING(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" DIR ${DIR})
INSTALL(FILES ${HEADER} DESTINATION include/${DIR})
ENDFOREACH(HEADER)

ENDMACRO(INSTALL_HEADERS_WITH_DIRECTORY)

INSTALL_HEADERS_WITH_DIRECTORY(Headers)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/cmake/FunGConfigVersion.cmake"
  VERSION ${FUNG_VERSION}
  COMPATIBILITY AnyNewerVersion
)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/FunGConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake/FunGConfig.cmake"
  COPYONLY
)

set(ConfigPackageLocation lib/cmake/FunG)
install(
  FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/FunGConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake/FunGConfigVersion.cmake"
  DESTINATION
    ${ConfigPackageLocation}
  COMPONENT
    Devel
)

option(BuildTest "Test" OFF)

if(BuildTest)
  set(CMAKE_BUILD_TYPE Debug)

  enable_testing()
  if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    find_package(GTest REQUIRED)
    include_directories(${GTEST_INCLUDE_DIRS})
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
endif()

# add a target to generate API documentation with Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
add_custom_target(doc
${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
COMMENT "Generating API documentation with Doxygen" VERBATIM
)
endif(DOXYGEN_FOUND)

