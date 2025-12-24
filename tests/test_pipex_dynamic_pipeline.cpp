//
// Created by Matteo Ranzi on 21/12/25.
////
// Created by Matteo Ranzi on 19/12/25.
//

#include <gtest/gtest.h>

#include "PipeX/dynamic/DynamicPipeline.h"
#include "PipeX/dynamic/nodes/DynamicFilter.h"
#include "PipeX/dynamic/nodes/DynamicTransformer.h"
#include "PipeX/dynamic/data/Data.h"


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
        DynamicFilter<float>::Predicate boundedPredicate = [lowerBound, upperBound](const float& data) {
            return data >= lowerBound && data <= upperBound;
        };


        DynamicPipeline<int, float> dynamicPipeline("BasicDynamicPipeline");
        dynamicPipeline.addNode<DynamicTransformer<int, float>>(halfFunction)
                        .addNode<DynamicFilter<float>>(boundedPredicate);


        constexpr int inputDataLength = 10;
        std::vector<int> inputData;
        inputData.reserve(inputDataLength);
        for (int i = 1; i <= inputDataLength; ++i) {
            inputData.push_back(i * 3);
        }

        const auto outputData = dynamicPipeline.run(inputData);// std::cout << "outputData size: " << outputData.size() << std::endl;
        const std::vector<float> expectedOutput = {6.0f, 7.5f, 9.0f, 10.5f};
        for (int i = 0; i < outputData.size(); ++i) {
            std::cout << "Data[" << i << "] after transformation: " << outputData[i] << std::endl;
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