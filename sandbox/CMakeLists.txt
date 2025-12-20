add_executable(sandbox data_dynamic_cast.cpp)
#add_executable(sandbox template_metaprogramming.cpp)

target_link_libraries(sandbox PRIVATE my_extended_cpp_standard)

#================================================================================================
set(SANDBOX_PRINT_DEBUG_LEVEL ${GLOBAL_PRINT_DEBUG_LEVEL} CACHE STRING "Debug level for sandbox")
define_print_debug_level_for_target(sandbox ${SANDBOX_PRINT_DEBUG_LEVEL})
#target_compile_definitions(sandbox PRIVATE PIPEX_PRINT_DEBUG_ENABLED)
#================================================================================================