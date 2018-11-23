cmake_minimum_required(VERSION 3.5)

project(fung VERSION 1.4.0 LANGUAGES CXX)

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${CMAKE_SOURCE_DIR}/cmake
    )

add_library(fung INTERFACE)

target_include_directories(fung INTERFACE
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_compile_options(fung INTERFACE $<$<CXX_COMPILER_ID:GNU>:-Wall -std=c++1y>)

include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/FunG)

install(TARGETS fung
    EXPORT fung-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

#This is required so that the exported target has the name FunG and not fung
set_target_properties(fung PROPERTIES EXPORT_NAME FunG)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT fung-targets
  FILE
    FunGTargets.cmake
  NAMESPACE
    FunG::
  DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/FunG
)

#Create a ConfigVersion.cmake file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/FunGConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/FunGConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/FunGConfig.cmake
    INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

#Install the config, configversion and custom find modules
install(FILES
    ${CMAKE_CURRENT_LIST_DIR}/cmake/FindFunG.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/FunGConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/FunGConfigVersion.cmake
    DESTINATION ${INSTALL_CONFIGDIR}
)

##############################################
## Exporting from the build tree
configure_file(${CMAKE_CURRENT_LIST_DIR}/cmake/FindFunG.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/FindFunG.cmake
    COPYONLY)

export(EXPORT fung-targets FILE ${CMAKE_CURRENT_BINARY_DIR}/FunGTargets.cmake NAMESPACE FunG::)

#Register package in user's package registry
export(PACKAGE FunG)

option(BuildTest "BuildTest" OFF)
if(BuildTest)
    add_subdirectory(test)
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

