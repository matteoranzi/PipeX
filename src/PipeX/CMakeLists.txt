#TODO evaluate a header only library
add_library(PipeX STATIC PipeX.cpp
        Image/PPM_ImagePreset_Source.cpp
        Sound/WAV_SoundPreset_Source.cpp)

include(${CMAKE_SOURCE_DIR}/cmake/PrintDebug.cmake)

target_include_directories(PipeX PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(PipeX PRIVATE my_extended_cpp_standard)


#================================================================================================
set(PIPEX_RELEASE_PRINT_DEBUG_LEVEL ${RELEASE_PRINT_DEBUG_LEVEL} CACHE STRING "Debug level for PipeX in release builds")
set(SANDBOX_DEBUG_PRINT_DEBUG_LEVEL ${DEBUG_PRINT_DEBUG_LEVEL} CACHE STRING "Debug level for PipeX in debug builds")

define_print_debug_level_for_target(PipeX ${PIPEX_RELEASE_PRINT_DEBUG_LEVEL} ${SANDBOX_DEBUG_PRINT_DEBUG_LEVEL})

#================================================================================================
target_compile_definitions(PipeX PUBLIC PIPEX_PRINT_DEBUG_ENABLED)
#================================================================================================