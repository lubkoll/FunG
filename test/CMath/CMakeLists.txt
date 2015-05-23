# Create a library called "Hello" which includes the source file "hello.cxx".
# The extension is already found. Any number of sources could be listed here.
add_library (arcsine_unittest arcsine_unittest.cpp)
add_library (cosine_unittest OBJECT cosine_unittest.cpp)
add_library (sine_unittest OBJECT sine_unittest.cpp)
add_library (exp_unittest OBJECT exp_unittest.cpp)
add_library (log_unittest OBJECT log_unittest.cpp)
add_library (pow_unittest OBJECT pow_unittest.cpp)
add_library (tan_unittest OBJECT tan_unittest.cpp)

# Make sure the compiler can find include files for our Hello library
# when other libraries or executables link to Hello
target_include_directories (arcsine_unittest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
#target_include_directories (cosine_unittest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
#target_include_directories (sine_unittest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
#target_include_directories (exp_unittest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
#target_include_directories (log_unittest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
#target_include_directories (pow_unittest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
#target_include_directories (tan_unittest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})