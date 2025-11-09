//
// Created by Matteo Ranzi on 09/11/25.
//

#include <gtest/gtest.h>

#include <vector>
#include <numeric>


#include "PipeX/Pipeline.h"
#include "PipeX/nodes/Filter.h"
#include "PipeX/nodes/Aggregator.h"
#include "PipeX/nodes/Transformer.h"


using namespace PipeX;

template <typename T>
void printVector(const std::vector<T>& vec);

/* ________________ Useful REGEX __________________

 * To filter Google Test debug info logs:
^\[DEBUG_INFO\] \[PipeX] (?=\[Node]).*$

 * ________________________________________________ */

// =========================================================================================================

TEST(PipelineTest, DebugTest) {
    std::cout << "======================================================================" << std::endl;

    { // Create unnamed scope to compact destructor output in this test's output

    }

    std::cout << "======================================================================" << std::endl;
}

// =========================================================================================================

template <typename T>
void printVector(const std::vector<T>& vec) {
    std::cout << "[";
    for (const auto& value : vec) {
        std::cout << value;
        if (&value != &vec.back()) {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;
}