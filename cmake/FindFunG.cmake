# FindFunG.cmake
#
# Finds the FunG library
#
# This will define the following variables
#
#    FunG_FOUND
#    FunG_INCLUDE_DIRS
#
# and the following imported targets
#
#     FunG::FunG

find_package(PkgConfig)
pkg_check_modules(PC_FunG QUIET FunG)

find_path(FunG_INCLUDE_DIR
    NAMES fung/fung.hh
    PATHS ${PC_FunG_INCLUDE_DIRS} ${FunG_DIR}
    PATH_SUFFIXES include
)

set(FunG_VERSION ${PC_FunG_VERSION})

mark_as_advanced(FunG_FOUND FunG_INCLUDE_DIR FunG_VERSION)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(FunG
    REQUIRED_VARS FunG_INCLUDE_DIR
    VERSION_VAR FunG_VERSION
)

if(FunG_FOUND)
    set(FunG_INCLUDE_DIRS ${FunG_INCLUDE_DIR})
endif()

if(FunG_FOUND AND NOT TARGET FunG::FunG)
    add_library(FunG::FunG INTERFACE IMPORTED)
    set_target_properties(FunG::FunG PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${FunG_INCLUDE_DIRS}"
    )
endif()
