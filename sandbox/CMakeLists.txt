add_executable(sandbox data_dynamic_cast.cpp)
#add_executable(sandbox template_metaprogramming.cpp)

target_link_libraries(sandbox PRIVATE my_extended_cpp_standard)

#================================================================================================
set(SANDBOX_RELEASE_PRINT_DEBUG_LEVEL ${RELEASE_PRINT_DEBUG_LEVEL} CACHE STRING "Debug level for sandbox in release builds")
set(SANDBOX_DEBUG_PRINT_DEBUG_LEVEL ${DEBUG_PRINT_DEBUG_LEVEL} CACHE STRING "Debug level for sandbox in debug builds")

define_print_debug_level_for_target(sandbox ${SANDBOX_RELEASE_PRINT_DEBUG_LEVEL} ${SANDBOX_DEBUG_PRINT_DEBUG_LEVEL})
#================================================================================================