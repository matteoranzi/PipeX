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

TEST(DynamicPipelineTest, BasicDynamicPipeline) {
    std::cout << "\n======================================================================" << std::endl;
    std::cout << "DynamicPipeline test: BasicDynamicPipeline" << std::endl;
    std::cout << "======================================================================" << std::endl;

    {

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