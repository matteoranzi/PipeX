//
// Created by Matteo Ranzi on 20/12/25.
//

#ifndef PIPEX_DYNAMICTRANSFORMER_H
#define PIPEX_DYNAMICTRANSFORMER_H
#include <functional>

#include "DynamicNode.h"
#include "debug/print_debug.h"
#include "PipeX/dynamic/data/Data.h"
#include "PipeX/dynamic/data/Data_.h"
#include "extended_cpp_standard/memory.h"

// FIXME: check for memory leaks
namespace PipeX {
    class DynamicTransformer final: public DynamicNode {
    public:
        using Function = std::function<Data_*(Data_* data)>;

        explicit DynamicTransformer(Function _function) : function(std::move(_function)) {
            PRINT_DEBUG_INFO("[DynamicTransformer] {%p}.Constructor(Function)\n", this);
        }

    private:
        Function function;

        std::vector<Data_*> processImpl(std::vector<Data_*>& input) const override {
            PRINT_DEBUG_INFO("[DynamicTransformer] {%p}.processImpl(std::vector<Data_*>&)\n", this);
            std::vector<Data_*> output;
            output.reserve(input.size());
            for (const auto& data : input) {
                output.push_back(function(data));
            }
            return output;
        }
    };
}

#endif //PIPEX_DYNAMICTRANSFORMER_H