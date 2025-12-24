//
// Created by Matteo Ranzi on 21/12/25.
////
// Created by Matteo Ranzi on 19/12/25.
//

#include <gtest/gtest.h>

#include <vector>
#include <algorithm>

#include "../include/PipeX/Pipeline.h"
#include "PipeX/nodes/Aggregator.h"
#include "PipeX/nodes/Filter.h"
#include "PipeX/nodes/Transformer.h"
#include "PipeX/data/Data.h"
#include "PipeX/nodes/Processor.h"


using namespace PipeX;

template <typename T>
void printVector(const std::vector<T>& vec);

template <typename T>
void printVectorGenericData(const std::vector<std::unique_ptr<IData>>& vec);

TEST(NodeTest, Transformer) {
    std::cout << "\n======================================================================" << std::endl;
    std::cout << "NodeTest test: Transformer" << std::endl;
    std::cout << "======================================================================" << std::endl;

    {
        auto halfFunction = [](const int& data) {
            return static_cast<float>(data) / 2.0f;
        };

        const Transformer<int, float> transformer(halfFunction);


        std::vector<std::unique_ptr<IData>> inputData;
        inputData.reserve(10);
        for (int i = 1; i <= 10; ++i) {
            inputData.push_back(make_unique<Data<int>>(i)); // Even numbers
        }

        const auto outputData = transformer.process(inputData);

        std::cout<< "Input vector: ";
        printVectorGenericData<int>(inputData);
        std::cout << "Pipelined output: ";
        printVectorGenericData<float>(outputData);

        const std::vector<float> expectedOutput = {0.5, 1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5, 5};

        for (int i = 0; i < outputData.size(); ++i) {
            const auto castedData = dynamic_cast<Data<float>*>(outputData[i].get());
            ASSERT_NE(castedData, nullptr);
            EXPECT_FLOAT_EQ(static_cast<float>(*castedData), expectedOutput[i]);
        }
    }

    std::cout << "======================================================================" << std::endl;

}

// =========================================================================================================

TEST(NodeTest, Processor) {
    std::cout << "\n======================================================================" << std::endl;
    std::cout << "NodeTest test: Processor" << std::endl;
    std::cout << "======================================================================" << std::endl;

    {
        auto sortingFunction = [](std::vector<int>& data) {
            std::sort(data.begin(), data.end());
            return data;
        };

        const Processor<int> processor(sortingFunction);


        std::vector<std::unique_ptr<IData>> inputData;
        inputData.reserve(10);
        inputData.emplace_back(make_unique<Data<int>>(5));
        inputData.emplace_back(make_unique<Data<int>>(2));
        inputData.emplace_back(make_unique<Data<int>>(9));
        inputData.emplace_back(make_unique<Data<int>>(4));

        const auto outputData = processor.process(inputData);

        std::cout<< "Input vector: ";
        printVectorGenericData<int>(inputData);
        std::cout << "Pipelined output: ";
        printVectorGenericData<int>(outputData);

        const std::vector<float> expectedOutput = {2, 4, 5, 9};

        for (int i = 0; i < outputData.size(); ++i) {
            const auto castedData = dynamic_cast<Data<int>*>(outputData[i].get());
            ASSERT_NE(castedData, nullptr);
            EXPECT_FLOAT_EQ(static_cast<int>(*castedData), expectedOutput[i]);
        }
    }

    std::cout << "======================================================================" << std::endl;

}

// =========================================================================================================

TEST(NodeTest, Filter) {
    std::cout << "\n======================================================================" << std::endl;
    std::cout << "NodeTest test: Filter" << std::endl;
    std::cout << "======================================================================" << std::endl;

    {
        auto evenIntegers = [](const int& data) {
            return data % 2 == 0;
        };

        const Filter<int> filter(evenIntegers);

        std::vector<std::unique_ptr<IData>> inputData;
        inputData.reserve(10);
        for (int i = 1; i <= 10; ++i) {
            inputData.push_back(make_unique<Data<int>>(i)); // Even numbers
        }


        const auto outputData = filter.process(inputData);

        std::cout<< "Input vector: ";
        printVectorGenericData<int>(inputData);
        std::cout << "Pipelined output: ";
        printVectorGenericData<int>(outputData);

        const std::vector<int> expectedOutput = {2, 4, 6, 8, 10};

        for (int i = 0; i < outputData.size(); ++i) {
            auto castedData = dynamic_cast<Data<int>*>(outputData[i].get());
            ASSERT_NE(castedData, nullptr);
            EXPECT_EQ(castedData->value, expectedOutput[i]);
        }

    }

    std::cout << "======================================================================" << std::endl;

}

// =========================================================================================================

TEST(NodeTest, Aggregator) {
    std::cout << "\n======================================================================" << std::endl;
    std::cout << "NodeTest test: Aggregator" << std::endl;
    std::cout << "======================================================================" << std::endl;

    {
        auto sumFunction = [](const std::vector<int>& input)  {
            int sum = 0;
            for (const auto& data : input) {
                sum += data;
            }
            return sum;
        };

        const Aggregator<int, int> aggregator(sumFunction);

        std::vector<std::unique_ptr<IData>> inputData;
        inputData.reserve(10);
        for (int i = 1; i <= 10; ++i) {
            inputData.push_back(make_unique<Data<int>>(i)); // Even numbers
        }


        const auto outputData = aggregator.process(inputData);

        std::cout<< "Input vector: ";
        printVectorGenericData<int>(inputData);
        std::cout << "Pipelined output: ";
        printVectorGenericData<int>(outputData);

        const std::vector<int> expectedOutput = {55};

        for (int i = 0; i < outputData.size(); ++i) {
            auto castedData = dynamic_cast<Data<int>*>(outputData[i].get());
            ASSERT_NE(castedData, nullptr);
            EXPECT_EQ(castedData->value, expectedOutput[i]);
        }
    }

    std::cout << "======================================================================" << std::endl;

}

// =========================================================================================================

template <typename T>
void printVector(const std::vector<T>& vec) {
    std::cout << "[";
    for (const auto& element : vec) {
        std::cout << element;
        if (&element != &vec.back()) {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;
}

template <typename T>
void printVectorGenericData(const std::vector<std::unique_ptr<IData>>& vec) {
    std::cout << "[";
    for (const auto& element : vec) {
        auto castedData = dynamic_cast<Data<T>*>(element.get());
        ASSERT_NE(castedData, nullptr);
        std::cout << castedData->value;
        if (&element != &vec.back()) {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;
}