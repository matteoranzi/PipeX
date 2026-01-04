cmake_minimum_required(VERSION 3.31.6)
project(PipeX)

enable_language(CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/PrintDebug.cmake)
include(cmake/my_extended_cpp_standard.cmake)
include(cmake/utils.cmake)
include(cmake/fetch_googletest.cmake)


add_subdirectory(src/PipeX)
option(PIPEX_BUILD_TESTS "Build tests" ON)
option(PIPEX_BUILD_SANDBOX "Build sandbox utilities" OFF)

if(PIPEX_BUILD_TESTS)
  add_subdirectory(tests)
endif()

if(PIPEX_BUILD_SANDBOX)
  add_subdirectory(sandbox)
endif()


add_executable(app src/main.cpp)


target_link_libraries(app PRIVATE PipeX)
target_link_libraries(app PRIVATE my_extended_cpp_standard)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/output)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/output/audio)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/output/image)


#================================================================================================
set(APP_PRINT_DEBUG_LEVEL ${GLOBAL_PRINT_DEBUG_LEVEL} STRING "Debug level for main")
#set(APP_PRINT_DEBUG_LEVEL ${PRINT_DEBUG_LEVEL_INFO} STRING "Debug level for main")
define_print_debug_level_for_target(app ${APP_PRINT_DEBUG_LEVEL})
#================================================================================================
