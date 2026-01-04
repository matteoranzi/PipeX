//
// Created by Matteo Ranzi on 21/12/25.
//

//TODO implement tests for Sink and Source nodes

#include <gtest/gtest.h>

#include <vector>
#include <algorithm>

#include "PipeX/Pipeline.h"
#include "PipeX/nodes/primitives/Aggregator.h"
#include "PipeX/nodes/primitives/Filter.h"
#include "PipeX/nodes/primitives/Transformer.h"
#include "PipeX/data/Data.h"
#include "PipeX/nodes/primitives/Processor.h"
#include "PipeX/utils/node_utils.h"
#include "my_extended_cpp_standard/my_memory.h"


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

        Transformer<int, float> transformer(halfFunction);


        std::vector<int> inputData;
        inputData.reserve(10);
        for (int i = 1; i <= 10; ++i) {
            inputData.push_back(i);
        }


        auto wrappedInput = wrapData<int>(extended_std::make_unique<std::vector<int>>(inputData));
        auto outputData = transformer.process(std::move(wrappedInput));
        const auto extractedOutput = PipeX::extractData<float>(outputData);

        std::cout<< "Input vector: ";
        printVector<int>(inputData);
        std::cout << "Pipelined output: ";
        printVector<float>(*extractedOutput);

        const std::vector<float> expectedOutput = {0.5, 1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5, 5};

        for (int i = 0; i < extractedOutput->size(); ++i) {
            EXPECT_FLOAT_EQ((*extractedOutput)[i], expectedOutput[i]);
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

        Processor<int> processor(sortingFunction);


        std::vector<int> inputData;

        inputData.emplace_back(5);
        inputData.emplace_back(2);
        inputData.emplace_back(9);
        inputData.emplace_back(4);

        auto wrappedInput = PipeX::wrapData<int>(extended_std::make_unique<std::vector<int>>(inputData));
        auto outputData = processor.process(std::move(wrappedInput));
        const auto extractedOutput = PipeX::extractData<int>(outputData);

        std::cout<< "Input vector: ";
        printVector<int>(inputData);
        std::cout << "Pipelined output: ";
        printVector<int>(*extractedOutput);

        const std::vector<float> expectedOutput = {2, 4, 5, 9};

        for (int i = 0; i < extractedOutput->size(); ++i) {
            EXPECT_FLOAT_EQ((*extractedOutput)[i], expectedOutput[i]);
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

        Filter<int> filter(evenIntegers);

        std::vector<int> inputData;
        inputData.reserve(10);
        for (int i = 1; i <= 10; ++i) {
            inputData.push_back(i);
        }


        auto wrappedInput = PipeX::wrapData<int>(extended_std::make_unique<std::vector<int>>(inputData));
        auto outputData = filter.process(std::move(wrappedInput));
        const auto extractedOutput = PipeX::extractData<int>(outputData);

        std::cout<< "Input vector: ";
        printVector<int>(inputData);
        std::cout << "Pipelined output: ";
        printVector<int>(*extractedOutput);

        const std::vector<int> expectedOutput = {2, 4, 6, 8, 10};
        EXPECT_EQ(*extractedOutput, expectedOutput);
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

        Aggregator<int, int> aggregator(sumFunction);

        std::vector<int> inputData;
        inputData.reserve(10);
        for (int i = 1; i <= 10; ++i) {
            inputData.push_back(i);
        }


        auto wrappedInput = PipeX::wrapData<int>(extended_std::make_unique<std::vector<int>>(inputData));
        auto outputData = aggregator.process(std::move(wrappedInput));
        const auto extractedOutput = PipeX::extractData<int>(outputData);

        std::cout<< "Input vector: ";
        printVector<int>(inputData);
        std::cout << "Pipelined output: ";
        printVector<int>(*extractedOutput);

        const std::vector<int> expectedOutput = {55};

        EXPECT_EQ(*extractedOutput, expectedOutput);
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