//
// Created by Matteo Ranzi on 21/12/25.
////
// Created by Matteo Ranzi on 19/12/25.
//

#include <gtest/gtest.h>

#include "../include/PipeX/dynamic/DynamicPipeline.h"
#include "PipeX/dynamic/nodes/DynamicTransformer.h"

using namespace PipeX;

template <typename T>
void printVector(const std::vector<T>& vec);

TEST(DynamicNodeTest, DynamicTransformer) {
    std::cout << "\n======================================================================" << std::endl;
    std::cout << "DynamicPipeline test: SimpleDynamicPipeline" << std::endl;
    std::cout << "======================================================================" << std::endl;

    {
        auto halfFunction = [](GenericData* data) {
            auto castedData = dynamic_cast<Data<int>*>(data);
            if (!castedData) {
                throw std::bad_cast();
            }
            return make_unique<Data<float>>(static_cast<float>(castedData->value) / 2.0f);
        };

        DynamicTransformer transformer(halfFunction);

        std::vector<std::unique_ptr<GenericData>> input;
        input.reserve(10);
        for (int i = 1; i <= 10; ++i) {
            input.push_back(make_unique<Data<int>>(i)); // Even numbers
        }

        const auto output = transformer.process(input);
        for (auto& data: output) {
            const auto castedData = dynamic_cast<Data<float>*>(data.get());
            std::cout << castedData->value << std::endl;
        }

        const std::vector<float> expectedOutput = {0.5, 1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5, 5};

        for (int i = 0; i < 10; ++i) {
            auto castedData = dynamic_cast<Data<float>*>(output[i].get());
            ASSERT_NE(castedData, nullptr);
            std::cout << "Data[" << i << "] after transformation: " << castedData->value << std::endl;
            EXPECT_FLOAT_EQ(static_cast<float>(castedData->value), expectedOutput[i]);
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