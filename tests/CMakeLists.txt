add_executable(PipeX_all_tests
#        _old_version/test_pipex_static_nodes.cpp
#        _old_version/test_pipex_static_pipeline.cpp
        test_pipex_pipeline.cpp
        test_pipex_nodes.cpp
)

target_link_libraries(PipeX_all_tests PRIVATE
        print_debug
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(PipeX_all_tests)


#================================================================================================
set(TESTS_RELEASE_PRINT_DEBUG_LEVEL ${RELEASE_PRINT_DEBUG_LEVEL} CACHE STRING "Debug level for tests in release builds")
set(TESTS_DEBUG_PRINT_DEBUG_LEVEL ${DEBUG_PRINT_DEBUG_LEVEL} CACHE STRING "Debug level for tests in debug builds")

define_print_debug_level_for_target(PipeX_all_tests ${TESTS_RELEASE_PRINT_DEBUG_LEVEL} ${TESTS_DEBUG_PRINT_DEBUG_LEVEL})
#================================================================================================