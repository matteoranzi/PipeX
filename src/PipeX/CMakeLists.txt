#TODO evaluate a header only library
add_library(PipeX STATIC PipeXEngine.cpp)

include(${CMAKE_SOURCE_DIR}/cmake/PrintDebug.cmake)

target_include_directories(PipeX PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(PipeX PRIVATE my_extended_cpp_standard)


#================================================================================================
set(PIPEX_PRINT_DEBUG_LEVEL ${GLOBAL_PRINT_DEBUG_LEVEL} CACHE STRING "Debug level for main")
define_print_debug_level_for_target(PipeX ${PIPEX_PRINT_DEBUG_LEVEL})
#target_compile_definitions(app PRIVATE PIPEX_PRINT_DEBUG_ENABLED)
#================================================================================================