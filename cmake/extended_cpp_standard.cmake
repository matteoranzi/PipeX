include_guard(DIRECTORY)

add_library(extended_cpp_standard INTERFACE)
target_include_directories(extended_cpp_standard INTERFACE ${CMAKE_SOURCE_DIR}/include)
