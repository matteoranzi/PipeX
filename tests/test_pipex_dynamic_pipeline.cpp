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
        DynamicTransformer::Function halfFunction = [](const GenericData* data) {
            auto castedData = dynamic_cast<const Data<int>*>(data);
            if (!castedData) {
                PRINT_DEBUG_ERROR("Error in halfFunction while casting to Data<int>\n");
                throw std::bad_cast();
            }
            return make_unique<Data<float>>(*castedData / 2.0f);
        };

        float lowerBound = 5.5f;
        float upperBound = 10.5f;
        DynamicFilter::Predicate boundedPredicate = [lowerBound, upperBound](const GenericData* data) {
            auto castedData = dynamic_cast<const Data<float>*>(data);
            if (!castedData) {
                throw std::bad_cast();
            }
            return *castedData >= lowerBound && *castedData <= upperBound;
        };


        DynamicPipeline dynamicPipeline("BasicDynamicPipeline");
        dynamicPipeline.addNode(std::make_shared<DynamicTransformer>(halfFunction))
                        .addNode(std::make_shared<DynamicFilter>(boundedPredicate));



        constexpr int inputDataLength = 10;
        std::vector<std::shared_ptr<GenericData>> inputData;
        inputData.reserve(inputDataLength);
        for (int i = 1; i <= inputDataLength; ++i) {
            inputData.push_back(make_unique<Data<int>>(i * 3));
        }

        const auto outputData = dynamicPipeline.run(inputData);// std::cout << "outputData size: " << outputData.size() << std::endl;
        const std::vector<float> expectedOutput = {6.0f, 7.5f, 9.0f, 10.5f};
        for (int i = 0; i < outputData.size(); ++i) {
            auto castedData = dynamic_cast<Data<float>*>(outputData[i].get());
            ASSERT_NE(castedData, nullptr);
            std::cout << "Data[" << i << "] after transformation: " << *castedData << std::endl;
            EXPECT_FLOAT_EQ(static_cast<float>(*castedData), expectedOutput[i]);
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