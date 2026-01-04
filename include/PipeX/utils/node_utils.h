//
// Created by Matteo Ranzi on 31/12/25.
//

#ifndef PIPEX_NODE_UTILS_H
#define PIPEX_NODE_UTILS_H


#include <string>
#include <vector>
#include <memory>
#include <typeinfo>

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
    std::unique_ptr<std::vector<T>> extractData(const std::unique_ptr<IData>& data, const std::string& sourceName) {
        if (!data) {
            PIPEX_PRINT_DEBUG_WARN("extractData -> nullptr data pointer when extracting data from source \"%s\"\n", sourceName.c_str());
            return nullptr;
        }

        auto* castedData = dynamic_cast<Data<std::unique_ptr<std::vector<T>>>*>(data.get());
        if (!castedData) {
            const auto& rawPtrTypeid = typeid(data.get());
            PIPEX_PRINT_DEBUG_ERROR("extractData -> Type mismatch when extracting data from source \"%s\"\n", sourceName.c_str());
            throw TypeMismatchException(sourceName, typeid(T), rawPtrTypeid);
        }

        return std::move(castedData->value);
    }

    template <typename T>
    std::unique_ptr<std::vector<T>> extractData(const std::unique_ptr<IData>& data) {
        return extractData<T>(std::move(data), "Unknown source");
    }

}

#endif //PIPEX_NODE_UTILS_H