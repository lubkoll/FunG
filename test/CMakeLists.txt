include(CTest)
enable_testing()
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
find_package(Eigen3)
if(EIGEN3_FOUND)
    # On travis the examples do not compile due to a strange ambiguity with functionality of Eigen:
    #  aux_source_directory(examples SRC_LIST)
    aux_source_directory(linear_algebra SRC_LIST)
endif()

aux_source_directory(cmath SRC_LIST)
aux_source_directory(fung SRC_LIST)
aux_source_directory(mathematical_operations SRC_LIST)
list(APPEND SRC_LIST
  cmath/texify/arccos.cpp
  cmath/texify/arcsine.cpp
  cmath/texify/cosine.cpp
  cmath/texify/exp.cpp
  cmath/texify/log.cpp
  cmath/texify/pow.cpp
  cmath/texify/sine.cpp
  cmath/texify/tan.cpp
  linear_algebra/texify/cofactor.cpp
  linear_algebra/texify/determinant.cpp
#  linear_algebra/texify/deviator.cpp
#  linear_algebra/texify/deviatoric_invariants.cpp
  linear_algebra/texify/frobenius_norm.cpp
#  linear_algebra/texify/mixed_invariants.cpp
#  linear_algebra/texify/principal_invariants.cpp
#  linear_algebra/texify/strain_tensor.cpp
  linear_algebra/texify/trace.cpp
  linear_algebra/texify/transpose.cpp
)

add_executable(tests ${SRC_LIST})
target_link_libraries(tests FunG::FunG GTest::GTest GTest::Main Threads::Threads)
if(EIGEN3_FOUND)
    target_include_directories(tests PRIVATE ${EIGEN3_INCLUDE_DIR})
endif()
add_custom_target(check COMMAND tests)
set_property(TARGET tests PROPERTY CXX_STANDARD 14)

