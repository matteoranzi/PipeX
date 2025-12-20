include_guard(DIRECTORY)

add_library(my_extended_cpp_standard INTERFACE)
target_include_directories(my_extended_cpp_standard INTERFACE ${CMAKE_SOURCE_DIR}/include)
