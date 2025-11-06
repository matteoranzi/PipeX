//
// Created by Matteo Ranzi on 05/11/25.
//

#include <gtest/gtest.h>

#include <vector>
#include <numeric>


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

TEST(NodesTest, Filter_evenNumbers) {
    // Create a Filter node that filters out even numbers
    auto isEven = [](int value) { return value % 2 == 0; };
    Filter<int> filter("Filter_evenNumbers", isEven);

    // Input data
    const std::vector<int> inputData = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Process and retrieve data
    const std::vector<int> outputData = filter.process(inputData);


    // Expected output data (only odd numbers)
    const std::vector<int> expectedOutput = {2, 4, 6, 8, 10};

    std::cout<< "Input vector: ";
    printVector(inputData);
    std::cout << "Filtered output (even numbers): ";
    printVector(outputData);

    // Check if the output matches the expected output
    EXPECT_EQ(outputData, expectedOutput);

    std::cout << "======================================================================\n" << std::endl;
}

TEST(NodesTest, Aggregator_sumNumbers) {
    // Create an Aggregator node that sums numbers
    auto sumFunction = [](const std::vector<int>& input) {
        return std::accumulate(input.begin(), input.end(), 0.0);

    };
    Aggregator<int, int> aggregator("Aggregator_sumNumbers", sumFunction);

    // Input data
    const std::vector<int> inputData = {1, 2, 3, 4, 5};

    // Process and retrieve data
    const std::vector<int> outputData = aggregator.process(inputData);

    // Expected output data (sum of inputData)
    const std::vector<int> expectedOutput = {15};

    std::cout<< "Input vector: ";
    printVector(inputData);
    std::cout << "Aggregated output (sum): ";
    printVector(outputData);

    // Check if the output matches the expected output
    EXPECT_EQ(outputData, expectedOutput);

    std::cout << "======================================================================\n" << std::endl;
}

TEST(NodesTest, Transformer_squareNumbers) {
    // Create a Transformer node that squares numbers
    auto squareFunction = [](int value) { return value * value; };
    Transformer<int, int> transformer("Transformer_squareNumbers", squareFunction);

    // Input data
    const std::vector<int> inputData = {1, 2, 3, 4, 5};

    // Process and retrieve data
    const std::vector<int> outputData = transformer.process(inputData);

    // Expected output data (squared values)
    const std::vector<int> expectedOutput = {1, 4, 9, 16, 25};

    std::cout<< "Input vector: ";
    printVector(inputData);
    std::cout << "Transformed output (x^2): ";
    printVector(outputData);

    // Check if the output matches the expected output
    EXPECT_EQ(outputData, expectedOutput);

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