add_library(PipeX
        Pipeline.cpp)

include(${CMAKE_SOURCE_DIR}/cmake/PrintDebug.cmake)

target_include_directories(PipeX PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(PipeX PUBLIC print_debug)