//
// Created by Matteo Ranzi on 21/12/25.
////
// Created by Matteo Ranzi on 19/12/25.
//

#include <gtest/gtest.h>

#include "PipeX/Pipeline.h"
#include "PipeX/nodes/primitives/Filter.h"
#include "PipeX/nodes/primitives/Sink.h"
#include "PipeX/nodes/primitives/Transformer.h"
#include "PipeX/nodes/primitives/Source.h"


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

        constexpr int inputDataLength = 10;
        std::vector<int> inputData;
        std::vector<float> outputData;

        Pipeline pipeline("BasicPipeline");
            pipeline.addNode<Source<int>>([&]()
                            {

                                inputData.reserve(inputDataLength);
                                for (int i = 1; i <= inputDataLength; ++i) {
                                    inputData.push_back(i * 3);
                                }
                                return inputData;
                            })
                .addNode<Transformer<int, float>>(halfFunction)
                .addNode<Filter<float>>(boundedPredicate)
                .addNode<Sink<float>>([&](const std::vector<float>& data) {
                    outputData = data;
                });


        pipeline.run();

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

        const std::vector<int> inputData = {1, 2, 3, 4, 5};
        std::vector<int> outputData;

        Pipeline originalPipeline("OriginalPipeline");
        originalPipeline.addNode<Source<int>>([&]() {
            std::vector<int> data;
            data.reserve(inputData.size());
            for (const auto& item : inputData) {
                data.push_back(item);
            }
            return data;
        });
        originalPipeline.addNode<Filter<int>>(isOdd);
        originalPipeline.addNode<Transformer<int, int>>(addFive);
        originalPipeline.addNode<Sink<int>>([&](const std::vector<int>& data) {
            outputData = data;
        });

        Pipeline copiedPipeline = originalPipeline; // Copy the pipeline

        copiedPipeline.run();
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

// =========================================================================================================

TEST(PipelineTest, MovePipeline) {
    std::cout << "\n======================================================================" << std::endl;
    std::cout << "Pipeline test: MovePipeline" << std::endl;
    std::cout << "======================================================================" << std::endl;

    { // Create unnamed scope to compact destructor output in this test's output
        auto isOdd = [](const int& value) {
            return value % 2 != 0;
        };
        auto addFive = [](const int& value) {
            return value + 5;
        };

        const std::vector<int> inputData = {1, 2, 3, 4, 5};
        std::vector<int> outputData;

        Pipeline originalPipeline("OriginalPipeline");
        originalPipeline.addNode<Source<int>>([&]() {
            std::vector<int> data;
            data.reserve(inputData.size());
            for (const auto& item : inputData) {
                data.push_back(item);
            }
            return data;
        });
        originalPipeline.addNode<Filter<int>>(isOdd);
        originalPipeline.addNode<Transformer<int, int>>(addFive);
        originalPipeline.addNode<Sink<int>>([&](const std::vector<int>& data) {
            outputData = data;
        });

        Pipeline copiedPipeline = std::move(originalPipeline); // Copy the pipeline

        copiedPipeline.run();
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