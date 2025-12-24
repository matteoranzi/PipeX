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

TEST(DynamicPipelineTest, BasicDynamicPipeline) {
    std::cout << "\n======================================================================" << std::endl;
    std::cout << "DynamicPipeline test: BasicDynamicPipeline" << std::endl;
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


        Pipeline<int, float> dynamicPipeline("BasicDynamicPipeline");
        dynamicPipeline.addNode<Transformer<int, float>>(halfFunction)
                        .addNode<Filter<float>>(boundedPredicate);


        constexpr int inputDataLength = 10;
        std::vector<int> inputData;
        inputData.reserve(inputDataLength);
        for (int i = 1; i <= inputDataLength; ++i) {
            inputData.push_back(i * 3);
        }


        const auto outputData = dynamicPipeline.run(inputData);// std::cout << "outputData size: " << outputData.size() << std::endl;

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