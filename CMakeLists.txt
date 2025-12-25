cmake_minimum_required(VERSION 4.1)
project(PipeX)

enable_language(CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/PrintDebug.cmake)
include(cmake/my_extended_cpp_standard.cmake)
include(cmake/utils.cmake)
include(cmake/fetch_googletest.cmake)


add_subdirectory(src/PipeX)
add_subdirectory(tests)
add_subdirectory(sandbox)


add_executable(app src/main.cpp)


target_link_libraries(app PRIVATE PipeX)
target_link_libraries(app PRIVATE my_extended_cpp_standard)


#================================================================================================
#set(APP_PRINT_DEBUG_LEVEL ${GLOBAL_PRINT_DEBUG_LEVEL} STRING "Debug level for main")
set(APP_PRINT_DEBUG_LEVEL ${PRINT_DEBUG_LEVEL_WARN} STRING "Debug level for main")
define_print_debug_level_for_target(app ${APP_PRINT_DEBUG_LEVEL})
target_compile_definitions(app PRIVATE PIPEX_PRINT_DEBUG_ENABLED)
#================================================================================================
