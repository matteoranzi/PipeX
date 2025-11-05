enable_testing()

add_executable(PipeX_unit_tests PRIVATE
        PipeX_tests.cpp
)

target_link_libraries(PipeX_unit_tests PRIVATE
        PipeX
        print_debug
        GTest::gtest_main)