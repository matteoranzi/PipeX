//
// Created by Matteo Ranzi on 19/12/25.
//

#include <gtest/gtest.h>

#include "../include/PipeX/dynamic/DynamicPipeline.h"
#include "PipeX/dynamic/nodes/DynamicTransformer.h"
#include "PipeX/nodes/Filter.h"
#include "PipeX/nodes/Transformer.h"


using namespace PipeX;

template <typename T>
void printVector(const std::vector<T>& vec);

TEST(DynamicPipelineTest, SimpleDynamicPipeline) {
    std::cout << "\n======================================================================" << std::endl;
    std::cout << "DynamicPipeline test: SimpleDynamicPipeline" << std::endl;
    std::cout << "======================================================================" << std::endl;

    {
        auto halfFunction = [](Data_* data) {
            auto castedData = dynamic_cast<Data<int>*>(data);
            if (!castedData) {
                throw std::bad_cast();
            }
            return new Data<float>(static_cast<float>(castedData->value) / 2.0f);
        };

        DynamicTransformer transformer(halfFunction);

        std::vector<Data_*> input;
        input.reserve(10);
        for (int i = 1; i <= 10; ++i) {
            input.push_back(new Data<int>(i)); // Even numbers
        }

        auto output = transformer.process(input);
        for (auto data: output) {
            auto castedData = dynamic_cast<Data<float>*>(data);
            std::cout << castedData->value << std::endl;
        }

        std::vector<float> expectedOutput = {0.5, 1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5, 5};

        for (int i = 0; i < 10; ++i) {
            auto castedData = dynamic_cast<Data<float>*>(output[i]);
            ASSERT_NE(castedData, nullptr);
            std::cout << "Data[" << i << "] after transformation: " << castedData->value << std::endl;
            EXPECT_FLOAT_EQ(static_cast<float>(castedData->value), expectedOutput[i]);
            delete input[i]; // Clean up
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