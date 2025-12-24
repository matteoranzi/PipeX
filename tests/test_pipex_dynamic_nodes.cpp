//
// Created by Matteo Ranzi on 21/12/25.
////
// Created by Matteo Ranzi on 19/12/25.
//

#include <gtest/gtest.h>

#include "PipeX/dynamic/DynamicPipeline.h"
#include "PipeX/dynamic/nodes/DynamicAggregator.h"
#include "PipeX/dynamic/nodes/DynamicFilter.h"
#include "PipeX/dynamic/nodes/DynamicTransformer.h"
#include "PipeX/dynamic/data/Data.h"


using namespace PipeX;

template <typename T>
void printVector(const std::vector<T>& vec);

TEST(DynamicNodeTest, DynamicTransformer) {
    std::cout << "\n======================================================================" << std::endl;
    std::cout << "DynamicNodeTest test: DynamicTransformer" << std::endl;
    std::cout << "======================================================================" << std::endl;

    {
        auto halfFunction = [](const int& data) {
            return static_cast<float>(data) / 2.0f;
        };

        const DynamicTransformer<int, float> transformer(halfFunction);


        std::vector<std::unique_ptr<GenericData>> input;
        input.reserve(10);
        for (int i = 1; i <= 10; ++i) {
            input.push_back(make_unique<Data<int>>(i)); // Even numbers
        }

        const auto output = transformer.process(input);
        const std::vector<float> expectedOutput = {0.5, 1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5, 5};

        for (int i = 0; i < output.size(); ++i) {
            auto castedData = dynamic_cast<Data<float>*>(output[i].get());
            ASSERT_NE(castedData, nullptr);
            std::cout << "Data[" << i << "] after transformation: " << *castedData << std::endl;
            EXPECT_FLOAT_EQ(static_cast<float>(*castedData), expectedOutput[i]);
        }
    }

    std::cout << "======================================================================" << std::endl;

}

// =========================================================================================================

TEST(DynamicNodeTest, DynamicFilter) {
    std::cout << "\n======================================================================" << std::endl;
    std::cout << "DynamicNodeTest test: DynamicFilter" << std::endl;
    std::cout << "======================================================================" << std::endl;

    {
        auto evenIntegers = [](const int& data) {
            return data % 2 == 0;
        };

        const DynamicFilter<int> filter(evenIntegers);

        std::vector<std::unique_ptr<GenericData>> input;
        input.reserve(10);
        for (int i = 1; i <= 10; ++i) {
            input.push_back(make_unique<Data<int>>(i)); // Even numbers
        }


        const auto output = filter.process(input);
        const std::vector<int> expectedOutput = {2, 4, 6, 8, 10};

        for (int i = 0; i < output.size(); ++i) {
            auto castedData = dynamic_cast<Data<int>*>(output[i].get());
            ASSERT_NE(castedData, nullptr);
            std::cout << "Data[" << i << "] after transformation: " << *castedData << std::endl;
            EXPECT_EQ(castedData->value, expectedOutput[i]);
        }
    }

    std::cout << "======================================================================" << std::endl;

}

// =========================================================================================================

TEST(DynamicNodeTest, DynamicAggregator) {
    std::cout << "\n======================================================================" << std::endl;
    std::cout << "DynamicNodeTest test: DynamicAggregator" << std::endl;
    std::cout << "======================================================================" << std::endl;

    {
        DynamicAggregator::Function sumFunction = [](const std::vector<std::unique_ptr<GenericData>>&& input) -> std::unique_ptr<GenericData>  {
            int sum = 0;
            for (const auto& data : input) {
                const auto castedData = dynamic_cast<const Data<int>*>(data.get());
                if (!castedData) {
                    throw std::bad_cast();
                }
                sum += castedData->value;
            }
            return make_unique<Data<int>>(sum);
        };

        const DynamicAggregator aggregator(sumFunction);

        std::vector<std::unique_ptr<GenericData>> input;
        input.reserve(10);
        for (int i = 1; i <= 10; ++i) {
            input.push_back(make_unique<Data<int>>(i)); // Even numbers
        }


        const auto output = aggregator.process(input);
        const std::vector<int> expectedOutput = {55};

        for (int i = 0; i < output.size(); ++i) {
            auto castedData = dynamic_cast<Data<int>*>(output[i].get());
            ASSERT_NE(castedData, nullptr);
            std::cout << "Data[" << i << "] after transformation: " << *castedData << std::endl;
            EXPECT_EQ(castedData->value, expectedOutput[i]);
        }
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