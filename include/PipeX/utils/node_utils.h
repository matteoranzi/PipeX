//
// Created by Matteo Ranzi on 31/12/25.
//

#ifndef PIPEX_NODE_UTILS_H
#define PIPEX_NODE_UTILS_H


#include <string>
#include <vector>

#include "PipeX/nodes/primitives/INode.h"
#include "my_extended_cpp_standard/my_memory.h"
#include "PipeX/data/Data.h"
#include "PipeX/errors/TypeMismatchExpection.h"

namespace PipeX {
    template <typename T>
    std::unique_ptr<IData> wrapData(std::unique_ptr<std::vector<T>>&& data) {
        return make_unique<Data<std::unique_ptr<std::vector<T>>>>(std::move(data));
    }

    template <typename T>
    std::unique_ptr<std::vector<T>> extractData(std::unique_ptr<IData>&& data, const std::string& sourceName) {
        auto* rawPtr = data.release();  // Release ownership
        auto castedData = dynamic_cast<Data<std::unique_ptr<std::vector<T>>>*>(rawPtr);
        if (!castedData) {
            delete rawPtr;  // Clean up on failure
            throw TypeMismatchException(sourceName, typeid(T), typeid(*rawPtr));
        }
        auto result = std::move(castedData->value);
        delete castedData;  // Clean up wrapper
        return result;
    }

    template <typename T>
    std::unique_ptr<std::vector<T>> extractData(std::unique_ptr<IData>&& data) {
       return extractData<T>(std::move(data), "Unknown source");
    }

}

#endif //PIPEX_NODE_UTILS_H