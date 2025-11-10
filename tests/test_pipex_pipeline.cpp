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

TEST(PipelineTest, SimplePipeline) {
    std::cout << "\n======================================================================" << std::endl;
    std::cout << "Pipeline test: SimplePipeline" << std::endl;
    std::cout << "======================================================================" << std::endl;

    { // Create unnamed scope to compact destructor output in this test's output
        auto isEven = [](const int&& value) {
            return value > 20 && value < 70;
        };
        auto powerCube = [](const int&& value) {
            return value * value;
        };

        auto pipeline = Pipeline<int>("SimplePipeline")
                            .addNode<Transformer<int, int>>(powerCube)
                            .addNode<Filter<int>>(isEven);


        const std::vector<int> inputData = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        const std::vector<int> outputData = pipeline.run(inputData);
        const std::vector<int> expectedOutput = {25, 36, 49, 64};


        std::cout<< "Input vector: ";
        printVector(inputData);
        std::cout << "Pipeline output vector: ";
        printVector(outputData);

        // Check if the output matches the expected output
        EXPECT_EQ(outputData, expectedOutput);
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