cmake_minimum_required(VERSION 4.1)
project(PipeX)

enable_language(CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/PrintDebug.cmake)
include(cmake/utils.cmake)

add_subdirectory(src/PipeX)

add_executable(PipeX_testing src/main.cpp)

target_link_libraries(PipeX_testing PRIVATE PipeX print_debug)


#================================ SET PRINT_DEBUG_LEVEL per module ==========================================

#------------- CUSTOM PRINT DEBUG LEVELS (for console print) -----------------
#[[
 Available log levels - set PRINT_DEBUG_LEVEL to one of:
    PRINT_DEBUG_LEVEL_NONE   → disable all logs
    PRINT_DEBUG_LEVEL_ERROR  → show only errors
    PRINT_DEBUG_LEVEL_WARN   → show warnings and errors
    PRINT_DEBUG_LEVEL_INFO   → show info, warnings, and errors
]]

#------------------------------------------------
# Global debug level (used as default for all modules)
set(PRINT_DEBUG_LEVEL_GLOBAL ${PRINT_DEBUG_LEVEL_INFO} CACHE STRING "Global debug level for all modules")

# Per-module debug levels (use global if not explicitly set)
set(PRINT_DEBUG_LEVEL_MAIN ${PRINT_DEBUG_LEVEL_GLOBAL} CACHE STRING "Debug level for main")
set(PRINT_DEBUG_LEVEL_PIPEX ${PRINT_DEBUG_LEVEL_GLOBAL} CACHE STRING "Debug level for benchmark")
#------------------------------------------------

define_print_debug_level_for_target(PipeX_testing ${PRINT_DEBUG_LEVEL_MAIN})
define_print_debug_level_for_target(PipeX ${PRINT_DEBUG_LEVEL_PIPEX})

#================================================================================================