//
// Created by Matteo Ranzi on 21/12/25.
////
// Created by Matteo Ranzi on 19/12/25.
//

#include <gtest/gtest.h>

#include "PipeX/Pipeline.h"
#include "PipeX/nodes/Filter.h"
#include "PipeX/nodes/Transformer.h"


using namespace PipeX;

template <typename T>
void printVector(const std::vector<T>& vec);

TEST(PipelineTest, BasicPipeline) {
    std::cout << "\n======================================================================" << std::endl;
    std::cout << "Pipeline test: BasicPipeline" << std::endl;
    std::cout << "======================================================================" << std::endl;

    {
        auto halfFunction= [](const int data) {
            return static_cast<float>(data) / 2.0f;
        };

        float lowerBound = 5.5f;
        float upperBound = 10.5f;
        Filter<float>::Predicate boundedPredicate = [lowerBound, upperBound](const float& data) {
            return data >= lowerBound && data <= upperBound;
        };


        Pipeline<int, float> pipeline("BasicPipeline");
        pipeline.addNode<Transformer<int, float>>(halfFunction)
                        .addNode<Filter<float>>(boundedPredicate);


        constexpr int inputDataLength = 10;
        std::vector<int> inputData;
        inputData.reserve(inputDataLength);
        for (int i = 1; i <= inputDataLength; ++i) {
            inputData.push_back(i * 3);
        }


        const auto outputData = pipeline.run(inputData);// std::cout << "outputData size: " << outputData.size() << std::endl;

        std::cout<< "Input vector: ";
        printVector(inputData);
        std::cout << "Pipelined output: ";
        printVector(outputData);

        const std::vector<float> expectedOutput = {6.0f, 7.5f, 9.0f, 10.5f};
        for (int i = 0; i < outputData.size(); ++i) {
            EXPECT_FLOAT_EQ(outputData[i], expectedOutput[i]);
        }
    }

    std::cout << "======================================================================" << std::endl;

}

// =========================================================================================================

TEST(PipelineTest, CopyPipeline) {
    std::cout << "\n======================================================================" << std::endl;
    std::cout << "Pipeline test: CopyPipeline" << std::endl;
    std::cout << "======================================================================" << std::endl;

    { // Create unnamed scope to compact destructor output in this test's output
        auto isOdd = [](const int& value) {
            return value % 2 != 0;
        };
        auto addFive = [](const int& value) {
            return value + 5;
        };

        Pipeline<int, int> originalPipeline("OriginalPipeline");
        originalPipeline.addNode<Filter<int>>(isOdd);
        originalPipeline.addNode<Transformer<int, int>>(addFive);

        Pipeline<int, int> copiedPipeline = originalPipeline; // Copy the pipeline

        const std::vector<int> inputData = {1, 2, 3, 4, 5};
        const std::vector<int> outputData = copiedPipeline.run(inputData);
        const std::vector<int> expectedOutput = {6, 8, 10};

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