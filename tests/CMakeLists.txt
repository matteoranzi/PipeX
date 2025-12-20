enable_testing()

add_executable(PipeX_all_tests
        test_pipex_nodes.cpp
        test_pipex_pipeline.cpp
        test_pipex_dynamic_pipeline.cpp
)

target_link_libraries(PipeX_all_tests PRIVATE
        print_debug
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(PipeX_all_tests)


#================================================================================================
set(TESTING_PRINT_DEBUG_LEVEL ${GLOBAL_PRINT_DEBUG_LEVEL} CACHE STRING "Debug level for testing")
define_print_debug_level_for_target(PipeX_all_tests ${TESTING_PRINT_DEBUG_LEVEL})
target_compile_definitions(PipeX_all_tests PRIVATE PIPEX_PRINT_DEBUG_ENABLED)
#================================================================================================